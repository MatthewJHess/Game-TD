#pragma once

#include <SFML/Graphics.hpp>
#include "Spell.h"
#include "Slime.h"
#include "TextureManager.h"

class Turret {
private:
    sf::Vector2f position;
    float health;
    sf::Sprite sprite;
    sf::Texture texture;
    float shootCooldown;
    float shootCooldownMax;
    std::vector<Spell> spells;

    void initSprite();

public:
    Turret(float x, float y, TextureManager& textureManager);
    virtual ~Turret();
    std::vector<Spell>& getSpells();
    void setPosition(float x, float y);
    void takeDamage(float damage);
    void update(float dt, const std::vector<Slime>& slimes, TextureManager& textureManager);
    void render(sf::RenderTarget& target);
};
