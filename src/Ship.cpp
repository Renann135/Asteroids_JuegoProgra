#include "Ship.hpp"
#include <SFML/Window/Keyboard.hpp>
#include <cmath>

static const int WIDTH = 1280;
static const int HEIGHT = 720;

Ship::Ship() {
    position = {WIDTH/2.f, HEIGHT/2.f};
    velocity = {0.f,0.f};
}

void Ship::reset(const sf::Vector2f& pos) {
    position = pos; velocity = {0.f,0.f}; angle = -90.f; alive = true;
}

void Ship::handleInput() {
    // rotation
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) angle -= rotationSpeed * (1.f/60.f);
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) angle += rotationSpeed * (1.f/60.f);
    // thrust
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
        float rad = angle * 3.14159f/180.f;
        velocity.x += std::cos(rad) * thrust * (1.f/60.f);
        velocity.y += std::sin(rad) * thrust * (1.f/60.f);
    }
}

void Ship::update(float dt) {
    // simple damping
    velocity *= 0.995f;
    position += velocity * dt;
    // wrap
    if (position.x < 0) position.x += WIDTH;
    if (position.x > WIDTH) position.x -= WIDTH;
    if (position.y < 0) position.y += HEIGHT;
    if (position.y > HEIGHT) position.y -= HEIGHT;
    timeSinceShot += dt;
}

Bullet Ship::shoot() {
    Bullet b;
    float rad = angle * 3.14159f/180.f;
    b.position = position + sf::Vector2f(std::cos(rad)*radius, std::sin(rad)*radius);
    b.velocity = velocity + sf::Vector2f(std::cos(rad)*400.f, std::sin(rad)*400.f);
    b.lifetime = 2.0f;
    b.alive = true;
    timeSinceShot = 0.f;
    return b;
}

void Ship::draw(sf::RenderTarget& target) const {
    // Render the ship as a solid white triangle regardless of texture, per request.
    sf::ConvexShape tri; tri.setPointCount(3);
    tri.setPoint(0, sf::Vector2f(0.f, -radius));
    tri.setPoint(1, sf::Vector2f(-radius*0.6f, radius));
    tri.setPoint(2, sf::Vector2f(radius*0.6f, radius));
    tri.setOrigin(0.f, 0.f);
    tri.setPosition(position);
    tri.setRotation(angle+90.f);
    tri.setFillColor(sf::Color::White);
    tri.setOutlineColor(sf::Color(180,180,180));
    tri.setOutlineThickness(1.f);
    target.draw(tri);
}
