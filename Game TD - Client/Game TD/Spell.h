#pragma once

#include <SFML/Graphics.hpp>

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
    sf::Texture texture;
    sf::Vector2f position;
    SpellType type;

    void initSprite();  // Initialize the sprite based on texture

public:
    // Constructor
    Spell(std::string type, const sf::Texture& texture, float x, float y, float angle);

    // Accessors
    const sf::FloatRect getBounds() const;
    SpellType getType() const; // New accessor for spell type

    // Functions
    void setPosition(const sf::Vector2f& position);
    sf::Vector2f getPosition() const;
    void render(sf::RenderTarget& target);
};
