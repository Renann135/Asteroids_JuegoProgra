#pragma once
#include <SFML/Graphics.hpp>

// Asteroide con colisión circular básica y envolvimiento de pantalla (wrap-around).
struct Asteroid {
    sf::Vector2f position;
    sf::Vector2f velocity;
    float radius = 24.f;
    int size = 2; // 2 = grande, 1 = mediano, 0 = pequeño
    const sf::Texture* texture = nullptr;
    bool alive = true;

    void update(float dt);
    void draw(sf::RenderTarget& target) const;
};
