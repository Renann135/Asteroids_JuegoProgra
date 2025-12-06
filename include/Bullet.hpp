#pragma once
#include <SFML/Graphics.hpp>

struct Bullet {
    sf::Vector2f position;
    sf::Vector2f velocity;
    float lifetime = 2.0f;
    float radius = 3.f;
    bool alive = true;
    void update(float dt);
    void draw(sf::RenderTarget& target) const;
};
