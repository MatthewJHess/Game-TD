#include "OtherPlayer.h"
#include <iostream>

void OtherPlayer::initTexture(std::string color)
{
    // Load a texture from the TextureManager based on the color
    if (color == "1")
    {
        if (!this->texture.loadFromFile("GameMaps/WizardRed.png"))
        {
            std::cerr << "ERROR::PLAYER::INITTEXTURE::Could not load texture file." << std::endl;
        }
    }
    else if (color == "0")
    {
        if (!this->texture.loadFromFile("GameMaps/Wizard.png"))
        {
            std::cerr << "ERROR::PLAYER::INITTEXTURE::Could not load texture file." << std::endl;
        }
    }
    this->sprite.setTexture(this->texture);
}

// Constructor
OtherPlayer::OtherPlayer(std::string color, float x, float y, float angle)
{
    this->color = color;
    this->initTexture(color);
    this->sprite.setOrigin(this->sprite.getGlobalBounds().width / 2, this->sprite.getGlobalBounds().height / 2);
    this->sprite.setPosition(x, y);
    this->angle = angle;
    this->sprite.setRotation(this->angle);
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