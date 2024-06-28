#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Network.hpp>
#include <vector>

enum SlimeTypes { BLUE = 0, RED, GREEN, NROFTYPES };

class Slime
{
private:
    sf::CircleShape shape;
    int type;
    float movementSpeed;
    void initShape(float x, float y);


public:
    Slime(int type, float x, float y);
    virtual ~Slime();

    //Accessor
    const sf::CircleShape getShape() const;
    const int& getType() const;
    void render(sf::RenderTarget& target);
};
