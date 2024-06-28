#pragma once

#include <SFML/Graphics.hpp>
#include "TextureManager.h"

enum class SpellType { Fire, Ice, Arrow };

class Spell {
private:
    sf::Sprite sprite;
    const sf::Texture* texture; // Use a pointer to the texture
    SpellType type;
    float movementSpeed;
    float damage;
    float size;
    sf::Vector2f velocity;
    bool isActive;

public:
    Spell(SpellType type, TextureManager& textureManager);
    Spell(const Spell& other); // Copy constructor
    Spell(Spell&& other) noexcept; // Move constructor
    Spell& operator=(const Spell& other); // Copy assignment operator
    Spell& operator=(Spell&& other) noexcept; // Move assignment operator
    std::string getTypeAsString() const { //type to string
        switch (type) {
        case SpellType::Fire: return "Fire";
        case SpellType::Ice: return "Ice";
        case SpellType::Arrow: return "Arrow";
        default: return "Unknown";
        }
    }
    sf::Vector2f getVelocity() const {
        return velocity;
    }

    const sf::FloatRect getBounds() const;
    const float& getMovementSpeed() const;
    float getDamage() const;
    SpellType getType() const;
    void setPosition(const sf::Vector2f& position);
    void setVelocity(const sf::Vector2f& velocity);
    void update(const sf::Vector2u& windowSize);
    bool getIsActive() const;
    void render(sf::RenderTarget& target);
    sf::Vector2f getPosition() const;
    void setIsActive(bool active);
};

