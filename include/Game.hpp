#pragma once
#include <SFML/Graphics.hpp>
#include <string>
#include <vector>
#include <map>
#include "Ship.hpp"
#include "Asteroid.hpp"
#include "Bullet.hpp"

// Simple Asteroids-style game core. Public API is minimal:
// create Game and call run().
class Game {
public:
    Game();
    void run();

    // Attempts to load a texture and store it under id. Returns pointer or nullptr on failure.
    const sf::Texture* loadTexture(const std::string& id, const std::string& path);

private:
    void processEvents();
    void update(float dt);
    void render();
    void spawnAsteroids(unsigned n);
    void resetLevel();
    bool circleCollision(const sf::Vector2f& aPos, float aR, const sf::Vector2f& bPos, float bR);

    // Menu and states
    enum class State { Menu, Playing, GameOver };
    State state = State::Menu;
    int menuSelection = 0; // 0 = Play, 1 = Exit
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
    // Tracks total elapsed play time (seconds) to raise difficulty over time
    float elapsedTime = 0.f;
};
