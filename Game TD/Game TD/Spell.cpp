#include "Spell.h"
#include <iostream>

// Constructor
Spell::Spell(SpellType type, const sf::Texture& texture)
    : type(type), texture(texture)
{
    float mvtsp=1.f;
    float sdamage= 1.f;
    float ssize= 10.f;
    switch (this->type)
    {
    case SpellType::Fire:
        sdamage = 1;
        mvtsp = 5.f;
        ssize = 160.f;
        break;
    case SpellType::Ice:
        sdamage = 1;
        mvtsp = 5.f;
        ssize = 8.f;
        break;
    case SpellType::Arrow:
        sdamage = 1;
        mvtsp = 5.f;
        ssize = 80.f;
        break;
    }
    this->velocity = sf::Vector2f(0.f, -mvtsp);
    this->damage = sdamage;
    this->size = ssize;
    this->movementSpeed = mvtsp;
    //this->initTexture();
    this->initSprite();
    this->isActive = true;
}

// Destructor
Spell::~Spell()
{
}

void Spell::initSprite()
{
    this->sprite.setTexture(this->texture);
    this->sprite.setScale(this->size / this->texture.getSize().x, this->size / this->texture.getSize().y);
    this->sprite.setOrigin(this->texture.getSize().x / 2.f, this->texture.getSize().y / 2.f);
}

// Accessors
const sf::FloatRect Spell::getBounds() const
{
    return this->sprite.getGlobalBounds();
}

const float& Spell::getMovementSpeed() const
{
    return this->movementSpeed;
}

float Spell::getDamage() const
{
    return this->damage;
}

SpellType Spell::getType() const
{
    return this->type;
}

// Functions
void Spell::setPosition(const sf::Vector2f& position)
{
    this->sprite.setPosition(position);
}

void Spell::setVelocity(const sf::Vector2f& velocity)
{
    this->velocity = velocity;
}

void Spell::update(const sf::Vector2u& windowSize)
{
    // Move the spell based on its velocity
    this->sprite.move(this->velocity);

    // Debug output
     //std::cout << "Spell position: (" << this->sprite.getPosition().x << ", " << this->sprite.getPosition().y << ")" << std::endl;

    // Check if the spell is out of bounds
    if (this->sprite.getPosition().x < 0 ||
        this->sprite.getPosition().y < 0 ||
        this->sprite.getPosition().x > windowSize.x ||
        this->sprite.getPosition().y > windowSize.y)
    {
         //std::cout << "Spell out of bounds, deactivating" << std::endl;
        isActive = false; // Deactivate the spell if it's out of bounds
    }
    else
    {
        isActive = true; // Ensure the spell is active if it's within bounds
    }
}

bool Spell::getIsActive() const
{
    return isActive;
}

void Spell::render(sf::RenderTarget& target)
{
    target.draw(this->sprite);
}

sf::Vector2f Spell::getPosition() const
{
    return this->sprite.getPosition();
}

void Spell::setIsActive(bool active)
{
    this->isActive = active;
}
