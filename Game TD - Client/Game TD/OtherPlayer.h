#pragma once

#include <SFML/Graphics.hpp>
#include <vector>
#include "Spell.h"
#include "TextureManager.h"

class OtherPlayer
{
private:
    sf::Sprite sprite;
    sf::Texture* texture;
    sf::Color type;
    std::string color;
    float angle;
    int itype = 0;

    // Private functions
    void initTexture(std::string color, TextureManager& textureManager);

public:
    OtherPlayer(std::string color, float x, float y, float angle, TextureManager& textureManager);
    std::string getColor() { return color; };
    int getType() { return itype; };

    // Accessors
    const sf::Sprite& getSprite() const;

    // Functions
    void update(float x, float y, float angle);
    void render(sf::RenderTarget& target);
};
