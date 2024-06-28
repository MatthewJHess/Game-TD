#include "Turret.h"
#include <cmath>
#include <iostream>

constexpr float PI = 3.14159265f;


void Turret::initSprite()
{
    if (this->texture) {
        // Set the texture to the sprite
        this->sprite.setTexture(*this->texture);
        this->sprite.setScale(0.2f, 0.2f); // Example scaling, adjust as needed
        this->sprite.setOrigin(this->sprite.getLocalBounds().width / 2.f, this->sprite.getLocalBounds().height / 2.f); // Set the origin to the center
    }
}

Turret::Turret(std::string type, TextureManager& textureManager, float x, float y, float angle)
    : position(x, y)
{
    this->texture = &textureManager.getTexture("archer");
    this->initSprite();
    this->sprite.setPosition(position);
    this->sprite.setRotation(angle);
}


void Turret::setPosition(float x, float y) {
    position.x = x;
    position.y = y;
    sprite.setPosition(position);
}

void Turret::render(sf::RenderTarget& target) {
    target.draw(this->sprite);
}
