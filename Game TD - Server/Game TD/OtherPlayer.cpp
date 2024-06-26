#include "OtherPlayer.h"
#include "TextureManager.h"
#include <iostream>

int counter = 0;

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
    else std::cout << "NO TEXTURE!!!!" << std::endl;
    this->sprite.setTexture(this->texture);
    this->sprite.setScale(.3f, .3f);
    this->sprite.setOrigin(this->sprite.getLocalBounds().width / 3.f, this->sprite.getLocalBounds().height * 2.f / 3.f);
}

// Constructor
OtherPlayer::OtherPlayer(std::string color, float x, float y, float angle)
{
    this->color = color;
    this->initTexture(color);
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
    if(counter == 0) initTexture(this->color), counter = 1;
    target.draw(this->sprite);
    //std::cout << "Drawing " << this->color << " at (" << this->sprite.getPosition().x << ", " << this->sprite.getPosition().y << ") " << std::endl;
}