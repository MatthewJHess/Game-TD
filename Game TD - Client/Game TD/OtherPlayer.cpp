#include "OtherPlayer.h"
#include <iostream>

int counter = 0;

void OtherPlayer::initTexture(std::string color, TextureManager& textureManager)
{
    // Load a texture from the TextureManager based on the color
    if (color == "1")
    {
        this->texture = &textureManager.getTexture("wizardRed");
        itype = 1;
    }
    else if (color == "0" || color == "Blue")
    {
        this->texture = &textureManager.getTexture("wizard");
        itype = 0;
    }
    this->sprite.setTexture(*this->texture);
    this->sprite.setScale(.3f, .3f);
    this->sprite.setOrigin(this->sprite.getLocalBounds().width / 3.f, this->sprite.getLocalBounds().height * 2.f / 3.f);
    std::cout << this->sprite.getScale().x << ", " << this->sprite.getScale().x << std::endl;
}

// Constructor
OtherPlayer::OtherPlayer(std::string color, float x, float y, float angle, TextureManager& textureManager) : texture(nullptr)
{
    this->color = color;
    this->initTexture(color, textureManager);
    this->sprite.setOrigin(this->sprite.getGlobalBounds().width / 2, this->sprite.getGlobalBounds().height / 2);
    this->sprite.setPosition(x, y);
    this->angle = angle;
    this->sprite.setRotation(this->angle + 180.f);
}

// Accessors
const sf::Sprite& OtherPlayer::getSprite() const
{
    return this->sprite;
}

// Functions
void OtherPlayer::update(float x, float y, float angle)
{
    // Update logic for OtherPlayer, if any
    this->sprite.setPosition(x, y);
    this->sprite.setRotation(angle);
}

void OtherPlayer::render(sf::RenderTarget& target)
{
    target.draw(this->sprite);
}
