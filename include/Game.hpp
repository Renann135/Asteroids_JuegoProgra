#pragma once
#include <SFML/Graphics.hpp>
#include <string>
#include <vector>
#include <map>
#include "Ship.hpp"
#include "Asteroid.hpp"
#include "Bullet.hpp"

// Núcleo simple de un juego estilo Asteroides. La API pública es mínima:
// crea `Game` y llama a `run()`.
class Game {
public:
    Game();
    void run();

    // Intenta cargar una textura y guardarla bajo `id`. Devuelve puntero o `nullptr` en caso de fallo.
    const sf::Texture* loadTexture(const std::string& id, const std::string& path);

private:
    void processEvents();
    void update(float dt);
    void render();
    void spawnAsteroids(unsigned n);
    void resetLevel();
    bool circleCollision(const sf::Vector2f& aPos, float aR, const sf::Vector2f& bPos, float bR);

    // Menú y estados
    enum class State { Menu, Playing, GameOver };
    State state = State::Menu;
    int menuSelection = 0; // 0 = Jugar, 1 = Salir.
    const std::vector<std::string> menuOptions = {"Jugar", "Salir"};
    const sf::Texture* backgroundTexture = nullptr;

    sf::RenderWindow window;
    std::map<std::string,sf::Texture> textures;

    Ship player;
    std::vector<Asteroid> asteroids;
    std::vector<Bullet> bullets;

    sf::Font font;
    sf::Text hud;

    int score = 0;
    int lives = 3;
    float timeSinceLastShot = 0.f;
    float asteroidTimer = 0.f;
    // Registra el tiempo total de juego (segundos) para aumentar la dificultad con el tiempo
    float elapsedTime = 0.f;
};
