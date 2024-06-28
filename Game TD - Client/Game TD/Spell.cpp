#include "Spell.h"

// Private functions

void Spell::initSprite()
{
    this->sprite.setTexture(*this->texture);
    this->sprite.setRotation(this->sprite.getRotation()); // Initial rotation
    this->sprite.setScale(this->size / this->texture->getSize().x, this->size / this->texture->getSize().y);
    this->sprite.setOrigin(this->texture->getSize().x / 2.f, this->texture->getSize().y / 2.f);
}

// Constructor
Spell::Spell(std::string type, TextureManager& textureManager, float x, float y, float angle)
    : texture(this->texture) // Initialize the texture reference
{
    if (type == "arrow" || type == "Arrow") stype = "arrow", this->size = 50.f, this->texture = &textureManager.getTexture("arrow");
    else if (type == "fire" || type == "Fire") stype = "fire", this->size = 100.f, this->texture = &textureManager.getTexture("fire");
    this->initSprite();
    this->sprite.setPosition(x, y);
    this->sprite.setRotation(angle);
}

// Accessors
const sf::FloatRect Spell::getBounds() const
{
    return this->sprite.getGlobalBounds();
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

sf::Vector2f Spell::getPosition() const
{
    return this->sprite.getPosition();
}

void Spell::render(sf::RenderTarget& target)
{
    target.draw(this->sprite);
}
