#pragma once

#include <SFML/Graphics.hpp>
#include "Spell.h"
#include "Slime.h"
#include "TextureManager.h"

class Turret {
private:

    sf::Sprite sprite;
    sf::Texture texture;

    void initSprite();

public:
    Turret(std::string type, const sf::Texture& texture, float x, float y, float angle);
    sf::Vector2f position;
    void setPosition(float x, float y);
    void render(sf::RenderTarget& target);

};
