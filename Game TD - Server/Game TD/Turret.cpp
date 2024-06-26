#include "Turret.h"
#include <cmath>
#include <iostream>

constexpr float PI = 3.14159265f;


void Turret::initSprite()
{
    // Set the texture to the sprite
    this->sprite.setTexture(this->texture);
    this->sprite.setScale(0.2f, 0.2f); // Example scaling, adjust as needed
    this->sprite.setOrigin(this->sprite.getLocalBounds().width / 2.f, this->sprite.getLocalBounds().height / 2.f); // Set the origin to the center
}

Turret::Turret(std::string type, float x, float y, TextureManager& textureManager)
    : position(x, y), health(100.f), shootCooldownMax(2.f), shootCooldown(0.f), type(type)
{
    this->texture = textureManager.getTexture("archer");
    this->initSprite();
    this->sprite.setPosition(position);
}

Turret::~Turret() {}

std::vector<Spell>& Turret::getSpells() {
    return spells;
}

void Turret::setPosition(float x, float y) {
    position.x = x;
    position.y = y;
    sprite.setPosition(position);
}

void Turret::takeDamage(float damage) {
    health -= damage;
    if (health < 0) health = 0;
}

void Turret::update(float dt, const std::vector<Slime>& slimes, TextureManager& textureManager) {
    // Update cooldown
    shootCooldown -= dt;

    if (!slimes.empty()) {
        // Calculate direction towards the first slime
        sf::Vector2f targetPos = slimes[0].getShape().getPosition();
        sf::Vector2f direction = targetPos - position;
        float length = std::sqrt(direction.x * direction.x + direction.y * direction.y);
        if (length != 0) direction /= length;

        // Rotate the sprite to face the first slime
        float angle = std::atan2(direction.y, direction.x);
        float angleDegrees = angle * 180.f / PI;
        sprite.setRotation(angleDegrees + 90.f);

        // If cooldown is finished, shoot at the first slime in range
        if (shootCooldown <= 0.f) {
            // Create a spell and set its position, damage, movement speed, size, and color
            spells.emplace_back(SpellType::Arrow, textureManager.getTexture("arrow")); // Example values, adjust as needed
            spells.back().setPosition(position);
            spells.back().setVelocity(direction * spells.back().getMovementSpeed());

            // Reset cooldown
            shootCooldown = shootCooldownMax;
        }
    }
}

void Turret::render(sf::RenderTarget& target) {
    target.draw(this->sprite);
    for (auto& spell : spells) {
        spell.render(target); // Pass pointer to render method
    }
}
