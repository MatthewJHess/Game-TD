#pragma once

#include <SFML/Graphics.hpp>
#include "Spell.h"
#include "Slime.h"
#include "TextureManager.h"
#include <iostream>

class Turret {
private:
    sf::Vector2f position;
    float health;
    sf::Sprite sprite;
    const sf::Texture* texture; // Use a pointer to the texture
    float shootCooldown;
    float shootCooldownMax;
    std::vector<Spell>& spells;
    std::string type;

    void initSprite();

public:
    Turret(std::vector<Spell>& spells, std::string type, float x, float y, TextureManager& textureManager);

    std::string getType() const;
    float getX() const;
    float getY() const;
    float getDirection() const;

    virtual ~Turret();
    std::vector<Spell>& getSpells();
    void setPosition(float x, float y);
    void takeDamage(float damage);
    void update(float dt, const std::vector<Slime>& slimes, TextureManager& textureManager);
    void render(sf::RenderTarget& target);
};
