#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Network.hpp>
#include <vector>
#include <functional>
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
    TextureManager& textureManager; // Reference to TextureManager
    int type;

    // Damage cooldown variables
    sf::Clock damageCooldownClock;
    float damageCooldownTime = 1.0f; // Cooldown time in seconds

    // Private functions
    void initVariables();
    void initTexture(int type);
    void initSprite();
    sf::Packet shootSpell(const sf::Vector2f& targetPosition);
    sf::TcpSocket* socket; // Pointer to sf::TcpSocket
    sf::Packet* largePacket = nullptr;
    sf::Packet smallPacket;


public:
    Player(TextureManager& textureManager, float x = 0.f, float y = 0.f, sf::TcpSocket* socket = nullptr, sf::Packet* largePacket = nullptr, int type = 0);
    virtual ~Player();

    // Accessors
    const sf::Sprite& getSprite() const;
    const int& getHp() const;
    const int& getHpMax() const;
    const int& getType() const;

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
