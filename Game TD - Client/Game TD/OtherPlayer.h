#pragma once



#include <SFML/Graphics.hpp>
#include <vector>
#include "Spell.h"
#include "TextureManager.h"

class OtherPlayer
{
private:
    sf::Sprite sprite;
    sf::Texture texture;
    sf::Color type;
    std::string color;
    float angle;
    // Private functions
    void initTexture(std::string color);

public:
    OtherPlayer(std::string color, float x, float y, float angle);
    std::string getColor() {return color;};
    // Accessors
    const sf::Sprite& getSprite() const;

    // Functions
    void update(float x, float y, float angle);
    void render(sf::RenderTarget& target);
};
