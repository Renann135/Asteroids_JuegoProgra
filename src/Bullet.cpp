#include "Bullet.hpp"
#include <SFML/Graphics.hpp>

static const int WIDTH = 1280;
static const int HEIGHT = 720;

void Bullet::update(float dt) {
    position += velocity * dt;
    lifetime -= dt;
    if (lifetime <= 0.f) alive = false;
    if (position.x < -10) position.x = WIDTH + 10;
    if (position.x > WIDTH + 10) position.x = -10;
    if (position.y < -10) position.y = HEIGHT + 10;
    if (position.y > HEIGHT + 10) position.y = -10;
}

void Bullet::draw(sf::RenderTarget& target) const {
    sf::CircleShape c(radius);
    c.setOrigin(radius, radius);
    c.setPosition(position);
    c.setFillColor(sf::Color::White);
    target.draw(c);
}
