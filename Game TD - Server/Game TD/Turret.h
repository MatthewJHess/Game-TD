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
    std::string type;

    void initSprite();

public:
    Turret(std::string type, float x, float y, TextureManager& textureManager);

    std::string getType() const {
        return type;
    }

    float getX() const {
        return position.x;
    }

    float getY() const {
        return position.y;
    }

    float getDirection() const {
        return sprite.getRotation();
    }

    virtual ~Turret();
    std::vector<Spell>& getSpells();
    void setPosition(float x, float y);
    void takeDamage(float damage);
    void update(float dt, const std::vector<Slime>& slimes, TextureManager& textureManager);
    void render(sf::RenderTarget& target);
};
