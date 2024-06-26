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
    float damage;
    float movementSpeed;
    float size;
    sf::Color color;
    sf::Sprite sprite;
    sf::Texture texture;
    sf::Vector2f velocity;
    bool isActive;
    SpellType type;

    //void initTexture(); // Initialize the texture based on spell type
    void initSprite();  // Initialize the sprite based on texture

public:
    // Constructor
    Spell(SpellType type, const sf::Texture& texture);

    // Destructor
    ~Spell();

    // Accessors
    const sf::FloatRect getBounds() const;
    const float& getMovementSpeed() const;
    float getDamage() const; // New accessor for damage
    SpellType getType() const; // New accessor for spell type
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

    // Functions
    void setPosition(const sf::Vector2f& position);
    void setVelocity(const sf::Vector2f& velocity);
    bool getIsActive() const;
    void setIsActive(bool active);
    sf::Vector2f getPosition() const;

    void update(const sf::Vector2u& windowSize);
    void render(sf::RenderTarget& target);
};
