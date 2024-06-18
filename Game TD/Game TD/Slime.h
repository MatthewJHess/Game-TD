#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Network.hpp>
#include <vector>

enum SlimeTypes { BLUE = 0, RED, GREEN, NROFTYPES };

struct MovementStep {
    sf::Vector2f offset;
    float duration;
};

class Slime
{
private:
    sf::CircleShape shape;
    int type;

    float movementSpeed;
    void initShape(const sf::RenderWindow& window);

    // Movement path related
    std::vector<MovementStep> path;
    int currentStepIndex;
    float elapsedTime;

    void initPath(); // Function to initialize the path

    // Health related
    float health;
    float maxHealth;
    void initHealth();

public:
    Slime(const sf::RenderWindow& window, int type);
    virtual ~Slime();

    //Accessor
    const sf::CircleShape getShape() const;
    const int& getType() const;
    float getHealth() const; // Health accessor

    // Functions
    float takeDamage(float damage); // Function to apply damage to the slime
    void route(float dt);
    bool isTouchingWindowEdge(const sf::RenderTarget* target) const;
    void update(float dt);
    void render(sf::RenderTarget& target);
};
