#pragma once

#include <SFML/Graphics.hpp>
#include "Spell.h"
#include "Slime.h"
#include "TextureManager.h"

class Turret {
private:

    sf::Sprite sprite;
    const sf::Texture* texture;

    void initSprite();

public:
    Turret(std::string type, TextureManager& textureManager, float x, float y, float angle);
    sf::Vector2f position;
    void setPosition(float x, float y);
    void render(sf::RenderTarget& target);

};
