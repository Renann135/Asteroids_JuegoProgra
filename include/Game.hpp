#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <string>
#include <vector>
#include <map>
#include <memory>
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
    
    // Sistema de música
    void loadMusicTracks();
    void playRandomTrack();
    void updateMusic();
    void toggleMusicPlayPause();
    void playNextTrack();
    void playPrevTrack();

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
    int maxScore = 0;
    int lives = 3;
    float timeSinceLastShot = 0.f;
    float asteroidTimer = 0.f;
    // Registra el tiempo total de juego (segundos) para aumentar la dificultad con el tiempo
    float elapsedTime = 0.f;
    
    // Sistema de música
    // Sistema de música
    std::vector<std::string> musicPaths;
    std::vector<std::unique_ptr<sf::Music>> musicTracks;
    int currentTrackIndex = -1;
    float musicVolume = 50.f; // 0-100
    float musicFadeTimer = 0.f;
    
    // Sonido de disparo
    sf::SoundBuffer shootSoundBuffer;
    sf::Sound shootSound;
    
    // Barra de volumen - propiedades para interacción con mouse
    sf::FloatRect volumeBarRect;
    // Botones de control de música (anterior / reproducir/pausa / siguiente)
    sf::FloatRect prevButtonRect;
    sf::FloatRect playPauseButtonRect;
    sf::FloatRect nextButtonRect;
};
