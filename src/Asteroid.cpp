#include "Asteroid.hpp"
#include <SFML/Graphics.hpp>

static const int WIDTH = 1280;
static const int HEIGHT = 720;

void Asteroid::update(float dt) {
    position += velocity * dt;
    if (position.x < -radius) position.x = WIDTH + radius;
    if (position.x > WIDTH + radius) position.x = -radius;
    if (position.y < -radius) position.y = HEIGHT + radius;
    if (position.y > HEIGHT + radius) position.y = -radius;
}

void Asteroid::draw(sf::RenderTarget& target) const {
    if (texture) {
        sf::Sprite s(*texture);
        s.setOrigin(texture->getSize().x/2.f, texture->getSize().y/2.f);
        s.setPosition(position);
        float scale = (radius*2.f) / std::max(1u, texture->getSize().x);
        // aumentar ligeramente las imágenes de asteroide para que sean más visibles
        const float EXTRA_SCALE = 7.f;
        s.setScale(scale * EXTRA_SCALE, scale * EXTRA_SCALE);
        target.draw(s);
        return;
    }
    sf::CircleShape c(radius);
    c.setOrigin(radius, radius);
    c.setPosition(position);
    c.setFillColor(sf::Color::Transparent);
    c.setOutlineColor(sf::Color(200,200,200));
    c.setOutlineThickness(1.f);
    target.draw(c);
}
