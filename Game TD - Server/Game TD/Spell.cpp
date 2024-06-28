#include "Spell.h"
#include <iostream>

Spell::Spell(SpellType type, TextureManager& textureManager)
    : type(type), texture(nullptr), isActive(true) // Initialize texture to nullptr
{
    float mvtsp = 1.f;
    float sdamage = 1.f;
    float ssize = 10.f;

    switch (this->type) {
    case SpellType::Fire:
        sdamage = 1;
        mvtsp = 5.f;
        ssize = 100.f;
        this->texture = &textureManager.getTexture("fire");
        break;
    case SpellType::Ice:
        sdamage = 1;
        mvtsp = 5.f;
        ssize = 8.f;
        this->texture = &textureManager.getTexture("ice");
        break;
    case SpellType::Arrow:
        sdamage = 1;
        mvtsp = 5.f;
        ssize = 50.f;
        this->texture = &textureManager.getTexture("arrow");
        break;
    }
    this->size = ssize;
    if (this->texture) {
        this->sprite.setTexture(*this->texture);
        this->sprite.setScale(this->size / this->texture->getSize().x, this->size / this->texture->getSize().y);
        this->sprite.setOrigin(this->texture->getSize().x / 2.f, this->texture->getSize().y / 2.f);
    }

    this->velocity = sf::Vector2f(0.f, -mvtsp);
    this->damage = sdamage;
    this->movementSpeed = mvtsp;
}

// Copy constructor
Spell::Spell(const Spell& other)
    : sprite(other.sprite), texture(other.texture), type(other.type),
    movementSpeed(other.movementSpeed), damage(other.damage), size(other.size),
    velocity(other.velocity), isActive(other.isActive) {}

// Move constructor
Spell::Spell(Spell&& other) noexcept
    : sprite(std::move(other.sprite)), texture(other.texture), type(other.type),
    movementSpeed(other.movementSpeed), damage(other.damage), size(other.size),
    velocity(other.velocity), isActive(other.isActive) {
    other.texture = nullptr;
}

// Copy assignment operator
Spell& Spell::operator=(const Spell& other) {
    if (this != &other) {
        this->sprite = other.sprite;
        this->texture = other.texture;
        this->type = other.type;
        this->movementSpeed = other.movementSpeed;
        this->damage = other.damage;
        this->size = other.size;
        this->velocity = other.velocity;
        this->isActive = other.isActive;
    }
    return *this;
}

// Move assignment operator
Spell& Spell::operator=(Spell&& other) noexcept {
    if (this != &other) {
        this->sprite = std::move(other.sprite);
        this->texture = other.texture;
        this->type = other.type;
        this->movementSpeed = other.movementSpeed;
        this->damage = other.damage;
        this->size = other.size;
        this->velocity = other.velocity;
        this->isActive = other.isActive;
        other.texture = nullptr;
    }
    return *this;
}

// Accessors
const sf::FloatRect Spell::getBounds() const {
    return this->sprite.getGlobalBounds();
}

const float& Spell::getMovementSpeed() const {
    return this->movementSpeed;
}

float Spell::getDamage() const {
    return this->damage;
}

SpellType Spell::getType() const {
    return this->type;
}

// Functions
void Spell::setPosition(const sf::Vector2f& position) {
    this->sprite.setPosition(position);
}

void Spell::setVelocity(const sf::Vector2f& velocity) {
    this->velocity = velocity;
}

void Spell::update(const sf::Vector2u& windowSize) {
    // Move the spell based on its velocity
    this->sprite.move(this->velocity);

    // Check if the spell is out of bounds
    if (this->sprite.getPosition().x < 0 ||
        this->sprite.getPosition().y < 0 ||
        this->sprite.getPosition().x > windowSize.x ||
        this->sprite.getPosition().y > windowSize.y) {
        isActive = false; // Deactivate the spell if it's out of bounds
    }
    else {
        isActive = true; // Ensure the spell is active if it's within bounds
    }
}

bool Spell::getIsActive() const {
    return isActive;
}

void Spell::render(sf::RenderTarget& target) {
    target.draw(this->sprite);
}

sf::Vector2f Spell::getPosition() const {
    return this->sprite.getPosition();
}

void Spell::setIsActive(bool active) {
    this->isActive = active;
}
