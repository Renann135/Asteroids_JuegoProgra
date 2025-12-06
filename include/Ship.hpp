#pragma once
#include <SFML/Graphics.hpp>
#include "Bullet.hpp"

// Player ship. Keeps shape/texture and simple physics.
class Ship {
public:
    Ship();
    void update(float dt);
    void handleInput();
    void draw(sf::RenderTarget& target) const;
    Bullet shoot();
    void reset(const sf::Vector2f& pos);

    sf::Vector2f position;
    sf::Vector2f velocity;
    float angle = -90.f; // up
    float radius = 16.f;

    const sf::Texture* texture = nullptr; // optional texture
    bool alive = true;
private:
    float thrust = 200.f;
    float rotationSpeed = 180.f; // degrees per second
    float shootCooldown = 0.2f;
    float timeSinceShot = 0.f;
};
