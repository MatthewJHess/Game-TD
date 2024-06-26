#include "Spell.h"

// Private functions
void Spell::initSprite()
{
    this->sprite.setTexture(this->texture);
    this->sprite.setOrigin(this->sprite.getGlobalBounds().width / 2, this->sprite.getGlobalBounds().height / 2);
    this->sprite.setRotation(this->sprite.getRotation()); // Initial rotation
}

// Constructor
Spell::Spell(std::string type, const sf::Texture& texture, float x, float y, float angle)
    : texture(texture)
{
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
