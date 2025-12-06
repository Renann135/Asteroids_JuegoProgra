#pragma once
#include <SFML/Graphics.hpp>

// Asteroid with basic circular collision and wrap-around.
struct Asteroid {
    sf::Vector2f position;
    sf::Vector2f velocity;
    float radius = 24.f;
    int size = 2; // 2 = large, 1 = medium, 0 = small
    const sf::Texture* texture = nullptr;
    bool alive = true;

    void update(float dt);
    void draw(sf::RenderTarget& target) const;
};
