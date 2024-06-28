#pragma once

#include <SFML/Graphics.hpp>
#include "TextureManager.h"

enum class SpellType {
    Fire,
    Ice,
    Arrow
};

class Spell
{
private:
    float size;
    sf::Sprite sprite;
    const sf::Texture* texture; // Use a reference to the texture
    sf::Vector2f position;
    SpellType type;
    std::string stype = "";

    void initSprite();  // Initialize the sprite based on texture

public:
    // Constructor
    Spell(std::string type, TextureManager& textureManager, float x, float y, float angle);

    // Accessors
    const sf::FloatRect getBounds() const;
    SpellType getType() const; // New accessor for spell type

    // Functions
    void setPosition(const sf::Vector2f& position);
    sf::Vector2f getPosition() const;
    void render(sf::RenderTarget& target);
};
