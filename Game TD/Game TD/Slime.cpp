#include "Slime.h"
#include <iostream>
#include <cmath>

using namespace sf;

// Constructor
Slime::Slime(const sf::RenderWindow& window, int type)
    : type(type), currentStepIndex(0), elapsedTime(0.f)
{
    this->initShape(window);
    this->initPath();
}

// Destructor
Slime::~Slime()
{
}

// Initialize the slime's shape based on its type
void Slime::initShape(const sf::RenderWindow& window)
{
    this->shape.setRadius(20.f);
    this->movementSpeed = 1.0f / 3.0f; // Adjust speed as needed

    Color color;
    switch (this->type)
    {
    case BLUE:
        color = Color::Blue;
        this->health = 1.f;
        break;
    case RED:
        color = Color::Red;
        this->health = 2.f;
        break;
    case GREEN:
        color = Color::Green;
        this->health = 3.f;
        break;
    }
    this->maxHealth = health;
    this->shape.setFillColor(color);
    this->shape.setOutlineColor(Color::White);
    this->shape.setOutlineThickness(1.f);

    this->shape.setPosition(
        Vector2f(
            0.f,
            static_cast<float>(window.getSize().y * 2 / 3)
        )
    );
}

// Initialize the path for slime movement
void Slime::initPath()
{
    this->path = {
        {{1.0f / 8.0f, 0}, 0.f},
        {{0, -1.0f / 2.0f}, 0.f}, // up
        {{3.0f / 10.0f, 0}, 0.f},
        {{0, 5.0f / 20.0f}, 0.f}, // down
        {{-2.0f / 10.0f, 0}, 0.f},
        {{0, 9.0f / 20.0f}, 0.f}, // down
        {{7.0f / 20.0f, 0}, 0.f},
        {{0, -14.0f / 20.0f}, 0.f}, // up
        {{6.0f / 20.0f, 0}, 0.f},
        {{0, 9.0f / 40.0f}, 0.f}, // down
        {{-3.0f / 20.0f, 0}, 0.f},
        {{0, 3.0f / 10.0f}, 0.f}, // down
        {{1.0f / 3.0f, 0}, 0.f}
    };

    // Calculate durations based on movement speed and offset
    for (auto& step : path)
    {
        float distance = std::sqrt(step.offset.x * step.offset.x + step.offset.y * step.offset.y);
        step.duration = distance / movementSpeed * 3.0f;
    }
}

// Update the slime's position based on its movement path
void Slime::route(float dt)
{
    VideoMode vsize = sf::VideoMode::getDesktopMode();

    if (currentStepIndex >= path.size())
        return;

    MovementStep& step = path[currentStepIndex];
    Vector2f movement(step.offset.x * vsize.width, step.offset.y * vsize.height);
    movement *= dt / step.duration;

    shape.move(movement);

    elapsedTime += dt;
    if (elapsedTime >= step.duration) {
        elapsedTime -= step.duration;
        currentStepIndex++;
    }
}

bool Slime::isTouchingWindowEdge(const sf::RenderTarget* target) const
{
    // Get the global bounds of the slime's shape
    sf::FloatRect slimeBounds = this->shape.getGlobalBounds();

    // Get the size of the render target (window)
    sf::Vector2u windowSize = target->getSize();

    // Check if any edge of the slime is touching or outside the window boundaries
    return (slimeBounds.left <= 0.f ||                      // Left edge touching or outside
        slimeBounds.left + slimeBounds.width >= windowSize.x ||   // Right edge touching or outside
        slimeBounds.top <= 0.f ||                       // Top edge touching or outside
        slimeBounds.top + slimeBounds.height >= windowSize.y);   // Bottom edge touching or outside
}



// Update the slime's state
void Slime::update(float dt)
{
    this->route(dt);
    // Uncomment this line if you want to check for window bounds collisions
    // this->updateWindowBoundsCollision(target);
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

// Get the slime's health
float Slime::getHealth() const
{
    return this->health;
}

// Apply damage to the slime
float Slime::takeDamage(float damage)
{
    this->health -= damage;
    if (this->health <= 0)
    {
        return this->maxHealth;
        std::cout << "Slime defeated!" << std::endl;
    }
}
