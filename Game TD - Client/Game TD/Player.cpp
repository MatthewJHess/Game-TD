#include "Player.h"
#include "Spell.h"
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

void Player::initTexture(int type)
{
    if(type==0)
    {
        if (!this->texture.loadFromFile("GameMaps/Wizard.png"))
        {
            std::cerr << "ERROR::PLAYER::INITTEXTURE::Could not load texture file." << std::endl;
        }
    }
    if (type == 1)
    {
        if (!this->texture.loadFromFile("GameMaps/WizardRed.png"))
        {
            std::cerr << "ERROR::PLAYER::INITTEXTURE::Could not load texture file." << std::endl;
        }
    }
}

void Player::initSprite()
{
    // Set the texture to the sprite
    this->sprite.setTexture(this->texture);
    this->sprite.setScale(.3f, .3f); // Example scaling, adjust as needed
    this->sprite.setOrigin(this->sprite.getLocalBounds().width / 3.f, this->sprite.getLocalBounds().height * 2.f / 3.f); // Set the origin to the center
}

sf::Packet Player::shootSpell(const sf::Vector2f& targetPosition)
{
    // Calculate direction towards the target position
    sf::Vector2f direction = targetPosition - this->sprite.getPosition();
    // Normalize the direction vector
    float magnitude = std::sqrt(direction.x * direction.x + direction.y * direction.y);

    if (magnitude != 0) {
        direction /= magnitude;
    }

    // Calculate angle in radians

    // Create a SpellPacket and populate it
    sf::Packet packet;
    packet << "Spell";
    std::string type = "Fire"; // Set the spell type, adjust this as needed
    packet << type << this->sprite.getPosition().x << this->sprite.getPosition().y << direction.x << direction.y;
    std::cout << "Direction: ( " << direction.x << ", " << direction.y << ") Type: "<<type<<" x: "<< this->sprite.getPosition().x<< " y: " << this->sprite.getPosition().y << std::endl;
    return packet;
}

Player::Player(TextureManager& textureManager, float x, float y, sf::TcpSocket* socket, sf::Packet* largePacket, int type)
    : textureManager(textureManager), socket(socket), largePacket(largePacket), type(type)
{
    this->initVariables();
    this->initTexture(type);
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

const int& Player::getType() const
{
    return this->type;
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
    largePacket->clear();
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
    sf::Packet packet;
    packet << "Player";
    std::string type = "1";
    packet << type << this->sprite.getPosition().x << this->sprite.getPosition().y << this->sprite.getRotation();
    //std::cout << "(" << this->sprite.getPosition().x << " ," << this->sprite.getPosition().y << ") " << this->sprite.getRotation() << std::endl;
    largePacket->append(packet.getData(), packet.getDataSize());
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
    largePacket->append(smallPacket.getData(), smallPacket.getDataSize());
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
        sf::Packet packet;
        packet = shootSpell(worldMousePosition);
        largePacket->append(packet.getData(), packet.getDataSize());
        this->updateSpellCooldown();
        std::cout << "Sent spell!" << std::endl;
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
}
