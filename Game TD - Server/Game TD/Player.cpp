#include "Player.h"
#include <iostream>
#include <cmath>

void Player::initVariables()
{
    this->movementSpeed = 5.f;
    this->hpMax = 10;
    this->hp = hpMax;
    this->spellCooldownDuration = 0.25f;
    this->spellCooldownTimer.restart();
}

void Player::initTexture()
{
    // Load a texture from file
    if (!this->texture.loadFromFile("GameMaps/Wizard.png"))
    {
        std::cerr << "ERROR::PLAYER::INITTEXTURE::Could not load texture file." << std::endl;
    }
}

void Player::initSprite()
{
    // Set the texture to the sprite
    this->sprite.setTexture(this->texture);
    this->sprite.setScale(.3f, .3f); // Example scaling, adjust as needed
    this->sprite.setOrigin(this->sprite.getLocalBounds().width / 3.f, this->sprite.getLocalBounds().height * 2.f / 3.f); // Set the origin to the center
}

void Player::shootSpell(const sf::Vector2f& targetPosition)
{
    // Calculate direction towards the mouse position
    sf::Vector2f direction = targetPosition - this->sprite.getPosition();
    // Normalize the direction vector
    float magnitude = sqrt(pow(direction.x, 2) + pow(direction.y, 2));

    if (magnitude != 0) {
        direction /= magnitude;
    }

    // Create a new spell object and add it to the container
    //spells.push_back(Spell(SpellType::Fire, textureManager.getTexture("fire"))); // Use TextureManager to get the texture
    spells.emplace_back(SpellType::Fire, textureManager);

    // Set the position of the spell to the player's position
    spells.back().setPosition(this->sprite.getPosition());

    // Set the velocity of the spell based on the direction and movement speed
    spells.back().setVelocity(direction * spells.back().getMovementSpeed());
}

Player::Player(std::vector<Spell>& spells, TextureManager& textureManager, float x, float y)
    : spells(spells), textureManager(textureManager)
{
    this->initVariables();
    this->initTexture();
    this->initSprite();
    this->sprite.setPosition(x, y);
}

Player::~Player()
{
}

// Accessors
const sf::Sprite& Player::getSprite() const
{
    return this->sprite;
}

const int& Player::getHp() const
{
    return this->hp;
}

const int& Player::getHpMax() const
{
    return this->hpMax;
}

const std::vector<Spell>& Player::getSpells() const
{
    return this->spells;
}

std::vector<Spell>& Player::getSpells() {
    return this->spells;
}

// Functions
void Player::takeDamage(const int damage)
{
    if (damageCooldownClock.getElapsedTime().asSeconds() >= damageCooldownTime)
    {
        this->hp -= damage;
        if (this->hp <= 0)
        {
            this->hp = 0;
            // Handle player death (e.g., end game)
        }
        damageCooldownClock.restart(); // Restart the cooldown clock
    }
}

void Player::instantDamage(const int damage)
{
    this->hp -= damage;
    if (this->hp <= 0)
    {
        this->hp = 0;
        // Handle player death (e.g., end game)
    }
}

void Player::gainHealth(const int health)
{
    if (this->hp < this->hpMax)
        this->hp += health;

    if (this->hp > this->hpMax)
        this->hp = this->hpMax;
}

void Player::updateSpellCooldown() {
    if (spellCooldownTimer.getElapsedTime().asSeconds() >= spellCooldownDuration) {
        // Cooldown expired, allow spell casting
        // Reset the cooldown timer
        spellCooldownTimer.restart();
    }
}

bool Player::canCastSpell() const {
    // Check if enough time has passed since the last spell cast
    return spellCooldownTimer.getElapsedTime().asSeconds() >= spellCooldownDuration;
}

void Player::updateInput()
{
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
    {
        this->sprite.move(-this->movementSpeed, 0.f);
    }
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
    {
        this->sprite.move(this->movementSpeed, 0.f);
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
    {
        this->sprite.move(0.f, -this->movementSpeed);
    }
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
    {
        this->sprite.move(0.f, this->movementSpeed);
    }
}

void Player::updateWindowBoundsCollision(const sf::RenderWindow& window)
{
    // Left
    if (this->sprite.getGlobalBounds().left <= 0.f)
        this->sprite.setPosition(0.f, this->sprite.getGlobalBounds().top);
    // Right
    if (this->sprite.getGlobalBounds().left + this->sprite.getGlobalBounds().width >= window.getSize().x)
        this->sprite.setPosition(window.getSize().x - this->sprite.getGlobalBounds().width, this->sprite.getGlobalBounds().top);
    // Top
    if (this->sprite.getGlobalBounds().top <= 0.f)
        this->sprite.setPosition(this->sprite.getGlobalBounds().left, 0.f);
    // Bottom
    if (this->sprite.getGlobalBounds().top + this->sprite.getGlobalBounds().height >= window.getSize().y)
        this->sprite.setPosition(this->sprite.getGlobalBounds().left, window.getSize().y - this->sprite.getGlobalBounds().height);
}

void Player::update(const sf::RenderWindow& window)
{
    this->updateInput();
    sf::Vector2i mousePosition = sf::Mouse::getPosition(window);
    sf::Vector2f worldMousePosition = window.mapPixelToCoords(mousePosition);
    this->rotateTowards(worldMousePosition);
    this->updateShooting(window); // Pass the window object to updateShooting
    //this->updateWindowBoundsCollision(window);
}

void Player::updateShooting(const sf::RenderWindow& window)
{
    // Check if the left mouse button is pressed
    if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && this->canCastSpell())
    {
        // Get the mouse position relative to the window
        sf::Vector2i mousePosition = sf::Mouse::getPosition(window);
        // Convert mouse position to world coordinates
        sf::Vector2f worldMousePosition = window.mapPixelToCoords(mousePosition);

        // Shoot a spell towards the mouse position
        shootSpell(worldMousePosition);
        this->updateSpellCooldown();
    }
}

void Player::rotateTowards(const sf::Vector2f& targetPosition)
{
    // Calculate the direction vector from the player to the target
    sf::Vector2f direction = targetPosition - this->sprite.getPosition();

    // Calculate the angle in radians
    float angle = std::atan2(direction.y, direction.x);

    // Convert the angle to degrees
    float angleDegrees = angle * 180.f / 3.14159265f;

    // Rotate the sprite (subtract 90 degrees to make the top of the sprite point towards the target)
    this->sprite.setRotation(angleDegrees + 90.f);
}

void Player::render(sf::RenderTarget& target)
{
    target.draw(this->sprite);
    // Render spells
}
