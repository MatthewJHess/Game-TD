#include "Slime.h"
#include <iostream>
#include <cmath>

using namespace sf;

// Constructor
Slime::Slime(int type, float x, float y)
    : type(type)
{
    this->initShape(x, y);
}

// Destructor
Slime::~Slime()
{
}

// Initialize the slime's shape based on its type
void Slime::initShape(float x, float y)
{
    this->shape.setRadius(20.f);

    Color color;
    switch (this->type)
    {
    case BLUE:
        color = Color::Blue;
        break;
    case RED:
        color = Color::Red;
        break;
    case GREEN:
        color = Color::Green;
        break;
    }
    this->shape.setFillColor(color);
    this->shape.setOutlineColor(Color::White);
    this->shape.setOutlineThickness(1.f);
    this->shape.setPosition(x, y);
}

// Render the slime
void Slime::render(sf::RenderTarget& target)
{
    target.draw(this->shape);
}

// Get the slime's shape
const sf::CircleShape Slime::getShape() const
{
    return this->shape;
}

// Get the slime's type
const int& Slime::getType() const
{
    return this->type;
}
