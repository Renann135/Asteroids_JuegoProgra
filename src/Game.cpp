#include "Game.hpp"
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <sstream>
#include <algorithm>

static const int WIDTH = 1280;
static const int HEIGHT = 720;

Game::Game(): window(sf::VideoMode(WIDTH, HEIGHT), "Asteroides"), player() {
    window.setFramerateLimit(60);
    // intentar cargar una fuente (opcional)
    if (font.loadFromFile("assets/arial.ttf")) {
        hud.setFont(font);
        hud.setCharacterSize(18);
        hud.setFillColor(sf::Color::White);
    }
    // intentar cargar algunas texturas de ejemplo - si estos archivos no existen
    // el juego seguirá dibujando formas simples en lugar de fallar.
    const sf::Texture* t = nullptr;
    t = loadTexture("ship", "assets/imagenes/ship.png");
    if (t) player.texture = t;

    // textura de asteroide (compartida)
    loadTexture("asteroid", "assets/asteroid_2.png");

    // textura opcional de bala
    loadTexture("bullet", "assets/imagenes/bullet.png");

    // fondo para menú y partida
    backgroundTexture = loadTexture("background", "assets/EspacioFondoJugando.jpg");

    // textura de corazón para vidas
    loadTexture("heart", "assets/imagenes/heart.png");

    // inicializar semilla aleatoria
    std::srand((unsigned)std::time(nullptr));

    // iniciar en estado de menú
    state = State::Menu;

    resetLevel();
}

const sf::Texture* Game::loadTexture(const std::string& id, const std::string& path) {
    sf::Texture tex;
    if (tex.loadFromFile(path)) {
        textures[id] = std::move(tex);
        return &textures[id];
    }
    return nullptr;
}

void Game::run() {
    sf::Clock clock;
    while (window.isOpen()) {
        float dt = clock.restart().asSeconds();
        processEvents();
        update(dt);
        render();
    }
}

void Game::processEvents() {
    sf::Event ev;
    while (window.pollEvent(ev)) {
        if (ev.type == sf::Event::Closed) window.close();
        if (ev.type == sf::Event::KeyPressed) {
            if (ev.key.code == sf::Keyboard::Escape) {
                window.close();
            }
            if (state == State::Menu) {
                if (ev.key.code == sf::Keyboard::Up || ev.key.code == sf::Keyboard::W) {
                    menuSelection = (menuSelection - 1 + (int)menuOptions.size()) % (int)menuOptions.size();
                } else if (ev.key.code == sf::Keyboard::Down || ev.key.code == sf::Keyboard::S) {
                    menuSelection = (menuSelection + 1) % (int)menuOptions.size();
                } else if (ev.key.code == sf::Keyboard::Enter || ev.key.code == sf::Keyboard::Space) {
                    if (menuSelection == 0) {
                        // Jugar
                        score = 0; lives = 3; elapsedTime = 0.f; asteroidTimer = 0.f;
                        resetLevel();
                        state = State::Playing;
                    } else {
                        // Salir
                        window.close();
                    }
                }
            } else {
                if (ev.key.code == sf::Keyboard::R && lives<=0) {
                    score = 0; lives = 3; elapsedTime = 0.f; asteroidTimer = 0.f; resetLevel(); state = State::Playing;
                }
                // Volver al menú desde GameOver
                if (ev.key.code == sf::Keyboard::M && state == State::GameOver) {
                    state = State::Menu;
                    menuSelection = 0;
                }
            }
        }

        // Manejo del ratón en el menú: pasar por encima y clic
        if (ev.type == sf::Event::MouseMoved && state == State::Menu) {
            sf::Vector2i mpos = sf::Mouse::getPosition(window);
            // calcular rectángulos de opciones de menú y detectar si el cursor está encima
            float spacing = 72.f; // espacio aumentado
            for (size_t i=0;i<menuOptions.size();++i) {
                if (font.getInfo().family != "") {
                    sf::Text opt(menuOptions[i], font, 32);
                    auto ob = opt.getLocalBounds();
                    float x = WIDTH/2.f - ob.width/2.f;
                    float y = HEIGHT*0.5f + (float)i*spacing - ob.height/2.f;
                    sf::FloatRect r(x, y, ob.width, ob.height + 8.f);
                    if (r.contains((float)mpos.x, (float)mpos.y)) {
                        menuSelection = (int)i;
                        break;
                    }
                } else {
                    // rectángulo alternativo
                    float w = 220.f, h = 44.f;
                    float x = WIDTH/2.f - w/2.f;
                    float y = HEIGHT*0.5f + (float)i*spacing - h/2.f;
                    sf::FloatRect r(x,y,w,h);
                    if (r.contains((float)mpos.x, (float)mpos.y)) { menuSelection = (int)i; break; }
                }
            }
        }

        if (ev.type == sf::Event::MouseButtonPressed && state == State::Menu) {
            if (ev.mouseButton.button == sf::Mouse::Left) {
                sf::Vector2i mpos = sf::Mouse::getPosition(window);
                float spacing = 72.f;
                for (size_t i=0;i<menuOptions.size();++i) {
                    if (font.getInfo().family != "") {
                        sf::Text opt(menuOptions[i], font, 32);
                        auto ob = opt.getLocalBounds();
                        float x = WIDTH/2.f - ob.width/2.f;
                        float y = HEIGHT*0.5f + (float)i*spacing - ob.height/2.f;
                        sf::FloatRect r(x, y, ob.width, ob.height + 8.f);
                        if (r.contains((float)mpos.x, (float)mpos.y)) {
                            if (i==0) { score = 0; lives = 3; elapsedTime = 0.f; asteroidTimer = 0.f; resetLevel(); state = State::Playing; }
                            else { window.close(); }
                        }
                    } else {
                        float w = 220.f, h = 44.f;
                        float x = WIDTH/2.f - w/2.f;
                        float y = HEIGHT*0.5f + (float)i*spacing - h/2.f;
                        sf::FloatRect r(x,y,w,h);
                        if (r.contains((float)mpos.x, (float)mpos.y)) {
                            if (i==0) { score = 0; lives = 3; elapsedTime = 0.f; asteroidTimer = 0.f; resetLevel(); state = State::Playing; }
                            else { window.close(); }
                        }
                    }
                }
            }
        }
    }
}

void Game::update(float dt) {
    if (lives <= 0) return; // fin del juego: esperar reinicio

    // registro de tiempo usado para ajustar la dificultad
    elapsedTime += dt;
    timeSinceLastShot += dt;
    asteroidTimer += dt;

    player.handleInput();
    // disparo (enfriamiento manejado por el temporizador de Game)
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space) && timeSinceLastShot >= 0.18f) {
        bullets.push_back(player.shoot());
        timeSinceLastShot = 0.f;
    }

    player.update(dt);
    player.update(dt);

    // actualizar balas
    for (auto& b : bullets) b.update(dt);
    bullets.erase(std::remove_if(bullets.begin(), bullets.end(), [](const Bullet& b){return !b.alive;}), bullets.end());

    // actualizar asteroides
    for (auto& a : asteroids) a.update(dt);
    asteroids.erase(std::remove_if(asteroids.begin(), asteroids.end(), [](const Asteroid& a){return !a.alive;}), asteroids.end());

    // colisiones: balas vs asteroides
    for (auto& b : bullets) {
        for (auto& a : asteroids) {
            if (!b.alive || !a.alive) continue;
            if (circleCollision(b.position, b.radius, a.position, a.radius)) {
                b.alive = false;
                a.alive = false;
                score += 100 * (a.size+1);
                // dividir en asteroides más pequeños
                if (a.size > 0) {
                    for (int i=0;i<2;i++) {
                        Asteroid child = a;
                        child.size = a.size - 1;
                        child.radius = a.radius*0.6f;
                        float ang = (std::rand()%360) * 3.14159f/180.f;
                        float speed = 50.f + std::rand()%100;
                        child.velocity = {std::cos(ang)*speed, std::sin(ang)*speed};
                        child.alive = true;
                        asteroids.push_back(child);
                    }
                }
            }
        }
    }

    // nave vs asteroide
    for (auto& a : asteroids) {
        if (!a.alive || !player.alive) continue;
        if (circleCollision(player.position, player.radius, a.position, a.radius)) {
            player.alive = false;
            // Daño: 1 vida hasta 1500 puntos; a partir de 1500, el impacto hace 2 de daño
            int daño = (score >= 1500) ? 2 : 1;
            lives -= daño;
            if (lives>0) {
                player.reset(sf::Vector2f(WIDTH/2.f, HEIGHT/2.f));
                player.alive = true;
            } else {
                // sin vidas -> estado GameOver
                state = State::GameOver;
            }
            break;
        }
    }

    // generar más asteroides si se han eliminado todos
    if (asteroids.empty()) {
        // aumentar la cantidad inicial lentamente con el tiempo
        unsigned base = 4u;
        unsigned extra = static_cast<unsigned>(elapsedTime / 20.0f); // +1 cada 20s
        spawnAsteroids(base + extra);
    }

    // Generación periódica: conforme avanza el juego, el intervalo entre generados se acorta, aumentando la dificultad.
    float spawnInterval = std::max(1.0f, 5.0f - elapsedTime / 60.0f); // de 5s hasta mínimo 1s
    if (asteroidTimer > spawnInterval) {
        // generar una pequeña cantidad escalada por el tiempo
        unsigned count = 1 + static_cast<unsigned>(elapsedTime / 60.0f);
        spawnAsteroids(count);
        asteroidTimer = 0.f;
    }
}

void Game::render() {
    window.clear(sf::Color::Black);

    // Si está en Menú, dibujar fondo y opciones
    if (state == State::Menu) {
        if (backgroundTexture) {
            sf::Sprite bg(*backgroundTexture);
            auto tx = backgroundTexture->getSize();
            float sx = (float)WIDTH / (float)tx.x;
            float sy = (float)HEIGHT / (float)tx.y;
            bg.setScale(sx, sy);
            window.draw(bg);
        } else {
            // color de fondo alternativo
            window.clear(sf::Color(10,10,30));
        }

        if (font.getInfo().family != "") {
            sf::Text title("Asteroides", font, 64);
            title.setFillColor(sf::Color::Yellow);
            auto tb = title.getLocalBounds();
            title.setOrigin(tb.width/2.f, tb.height/2.f);
            title.setPosition(WIDTH/2.f, HEIGHT*0.25f);
            window.draw(title);

            for (size_t i=0;i<menuOptions.size();++i) {
                sf::Text opt(menuOptions[i], font, 32);
                if ((int)i==menuSelection) opt.setFillColor(sf::Color::Yellow);
                else opt.setFillColor(sf::Color::White);
                auto ob = opt.getLocalBounds();
                opt.setOrigin(ob.width/2.f, ob.height/2.f);
                opt.setPosition(WIDTH/2.f, HEIGHT*0.5f + (float)i*48.f);
                window.draw(opt);
            }
        }

        window.display();
        return;
    }

    // Jugando o GameOver
    // dibujar fondo para la partida
    if (backgroundTexture) {
        sf::Sprite bg(*backgroundTexture);
        auto tx = backgroundTexture->getSize();
        float sx = (float)WIDTH / (float)tx.x;
        float sy = (float)HEIGHT / (float)tx.y;
        bg.setScale(sx, sy);
        window.draw(bg);
    } else {
        // fondo alternativo para la partida
        // mantener el color limpio (negro) o dibujar un relleno oscuro sutil
    }

    // dibujar balas
    for (auto& b : bullets) b.draw(window);
    // dibujar asteroides
    for (auto& a : asteroids) a.draw(window);
    // dibujar jugador
    if (player.alive) player.draw(window);

    // HUD: puntos y corazones para vidas
    if (font.getInfo().family != "") {
    std::ostringstream ss;
    ss << "Puntos: " << score;
        hud.setString(ss.str());
        hud.setPosition(8.f, 8.f);
        window.draw(hud);

        // dibujar corazones en la esquina superior derecha
        const float heartSize = 20.f;
        for (int i=0;i<lives;i++) {
            float x = WIDTH - 8.f - (i+1)*(heartSize+6.f);
            float y = 8.f;
            auto it = textures.find("heart");
            if (it != textures.end()) {
                sf::Sprite hs(it->second);
                auto ts = it->second.getSize();
                float sx = heartSize / (float)ts.x;
                float sy = heartSize / (float)ts.y;
                hs.setScale(sx, sy);
                hs.setPosition(x, y);
                window.draw(hs);
            } else {
                // alternativa: dibujar una forma de corazón simple usando ConvexShape
                sf::ConvexShape heart;
                heart.setPointCount(6);
                float s = heartSize;
                // los puntos aproximan un corazón centrado en (0,0)
                heart.setPoint(0, sf::Vector2f(0.f, -s*0.15f));
                heart.setPoint(1, sf::Vector2f(s*0.25f, -s*0.5f));
                heart.setPoint(2, sf::Vector2f(s*0.5f, -s*0.15f));
                heart.setPoint(3, sf::Vector2f(0.f, s*0.5f));
                heart.setPoint(4, sf::Vector2f(-s*0.5f, -s*0.15f));
                heart.setPoint(5, sf::Vector2f(-s*0.25f, -s*0.5f));
                heart.setFillColor(sf::Color::Red);
                // posicionarlo en el área superior derecha
                heart.setPosition(x + heartSize/2.f, y + heartSize/2.f + 4.f);
                // escalarlo un poco para que encaje bien
                float scale = (heartSize / (s));
                heart.setScale(scale, scale);
                heart.setOrigin(0.f, 0.f);
                window.draw(heart);
            }
        }
    }

    if (lives<=0) {
        if (font.getInfo().family != "") {
            sf::Text go("FIN DEL JUEGO - Pulsa R para reiniciar o M para menú", font, 28);
            go.setFillColor(sf::Color::Red);
            auto b = go.getLocalBounds();
            go.setOrigin(b.width/2.f, b.height/2.f);
            go.setPosition(WIDTH/2.f, HEIGHT/2.f);
            window.draw(go);
        }
    }

    window.display();
}

void Game::spawnAsteroids(unsigned n) {
    for (unsigned i=0;i<n;i++) {
        Asteroid a;
        // colocar alrededor de los bordes de la pantalla
        int side = std::rand()%4;
        switch(side) {
            case 0: a.position = {float(std::rand()%WIDTH), -20.f}; break;
            case 1: a.position = {float(std::rand()%WIDTH), float(HEIGHT+20)}; break;
            case 2: a.position = {-20.f, float(std::rand()%HEIGHT)}; break;
            default: a.position = {float(WIDTH+20), float(std::rand()%HEIGHT)}; break;
        }
        float ang = (std::rand()%360) * 3.14159f/180.f;
        // Dificultad: aumentar la velocidad de los asteroides lentamente con el tiempo
        float difficultyMultiplier = 1.0f + (elapsedTime / 60.0f); // +100% de velocidad por minuto
        float speed = (30.f + std::rand()%80) * difficultyMultiplier;
        a.velocity = {std::cos(ang)*speed, std::sin(ang)*speed};
        a.size = 2;
        a.radius = 32.f + std::rand()%16;
        a.alive = true;
        // asignar textura si está disponible
        auto it = textures.find("asteroid");
        if (it != textures.end()) a.texture = &it->second;
        asteroids.push_back(a);
    }
}

void Game::resetLevel() {
    asteroids.clear(); bullets.clear();
    player.reset(sf::Vector2f(WIDTH/2.f, HEIGHT/2.f));
    player.alive = true;
    spawnAsteroids(6);
}

bool Game::circleCollision(const sf::Vector2f& aPos, float aR, const sf::Vector2f& bPos, float bR) {
    float dx = aPos.x - bPos.x;
    float dy = aPos.y - bPos.y;
    float r = aR + bR;
    return (dx*dx + dy*dy) <= (r*r);
}
