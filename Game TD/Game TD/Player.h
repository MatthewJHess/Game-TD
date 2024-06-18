#pragma once

#include <SFML/Graphics.hpp>
#include <vector>
#include "Spell.h"
#include "TextureManager.h"

class Player
{
private:
    sf::Sprite sprite;
    sf::Texture texture;
    float movementSpeed;
    int hp;
    int hpMax;
    float spellCooldownDuration;
    sf::Clock spellCooldownTimer;
    std::vector<Spell>& spells;
    TextureManager& textureManager; // Reference to TextureManager

    // Damage cooldown variables
    sf::Clock damageCooldownClock;
    float damageCooldownTime = 1.0f; // Cooldown time in seconds

    // Private functions
    void initVariables();
    void initTexture();
    void initSprite();
    void shootSpell(const sf::Vector2f& targetPosition);

public:
    Player(std::vector<Spell>& spells, TextureManager& textureManager, float x = 0.f, float y = 0.f);
    virtual ~Player();

    // Accessors
    const sf::Sprite& getSprite() const;
    const int& getHp() const;
    const int& getHpMax() const;
    const std::vector<Spell>& getSpells() const;
    std::vector<Spell>& getSpells();

    // Functions
    void takeDamage(const int damage);
    void instantDamage(const int damage);
    void gainHealth(const int health);
    void updateSpellCooldown();
    bool canCastSpell() const;
    void updateInput();
    void updateWindowBoundsCollision(const sf::RenderWindow& window);
    void update(const sf::RenderWindow& window);
    void updateShooting(const sf::RenderWindow& window);
    void rotateTowards(const sf::Vector2f& targetPosition);
    void render(sf::RenderTarget& target);
};
