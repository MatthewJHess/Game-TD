#include "Game.h"
#include "Slime.h"
#include "Spell.h"
#include "Player.h"
#include "OtherPlayer.h"
#include <sstream>

void Game::initVariables()
{
    this->endGame = false;
    this->allSlimesSpawned = false;
    this->spawnTimerMax = 0.5f; // Set to half a second
    this->spawnTimer = 0.f;
    this->points = 0;
    this->wave = 1;
    this->slimesToSpawn = 0;
    this->slimesSpawned = 0;
    this->coins = 50;
    this->initWaveConfigs();
    this->placingTurret = false;
}

void Game::initWindow()
{
    // Get desktop video mode
    this->videoMode = sf::VideoMode::getDesktopMode();

    // Load textures
    this->textureManager.loadTexture("backgroundTexture", "GameMaps/ForestMap.png");
    this->textureManager.loadTexture("victoryTexture", "GameMaps/Victory.png");
    this->textureManager.loadTexture("fire", "GameMaps/Fireball.png");
    this->textureManager.loadTexture("Fire", "GameMaps/Fireball.png");
    this->textureManager.loadTexture("wizard", "GameMaps/Wizard.png");
    //this->textureManager.loadTexture("wizardRed", "GameMaps/WizardRed.png");
    this->textureManager.loadTexture("arrow", "GameMaps/Arrow.png");
    this->textureManager.loadTexture("archer", "GameMaps/Archer.png");

    // Set background sprite
    sf::Texture& backgroundTexture = this->textureManager.getTexture("backgroundTexture");
    if (backgroundTexture.getSize() != sf::Vector2u(0, 0)) {
        this->backgroundSprite.setTexture(backgroundTexture);

        sf::Vector2u textureSize = backgroundTexture.getSize();
        sf::Vector2u windowSize = this->window->getSize();
        backgroundSprite.setScale(
            float(windowSize.x) / textureSize.x,
            float(windowSize.y) / textureSize.y
        );
    }
    else {
        std::cerr << "Failed to load background texture." << std::endl;
    }

    // Set victory sprite
    sf::Texture& victoryTexture = this->textureManager.getTexture("victoryTexture");
    if (victoryTexture.getSize() != sf::Vector2u(0, 0)) {
        this->victorySprite.setTexture(victoryTexture);

        sf::Vector2u textureSize = victoryTexture.getSize();
        sf::Vector2u windowSize = this->window->getSize();
        victorySprite.setScale(
            float(windowSize.x) / textureSize.x,
            float(windowSize.y) / textureSize.y
        );
    }
    else {
        std::cerr << "Failed to load victory texture." << std::endl;
    }
}


void Game::initFonts()
{
    if (!this->font.loadFromFile("Fonts/OLDENGL.TTF"))
    {
        std::cout << "ERROR::GAME::INITFONTS::COULD NOT LOAD OLDENGL.TTF" << "\n";
    }
}

void Game::initText()
{
    this->guiText.setFont(this->font);
    this->guiText.setFillColor(sf::Color::White);
    this->guiText.setCharacterSize(32);

    this->endGameText.setFont(this->font);
    this->endGameText.setFillColor(sf::Color::Red);
    this->endGameText.setCharacterSize(60);
    this->endGameText.setString("YOU ARE DEAD, EXIT THE GAME!");

    windowSize = window->getSize();
    sf::FloatRect textRect = this->endGameText.getGlobalBounds();
    this->endGameText.setOrigin(textRect.left + textRect.width / 2.0f, textRect.top + textRect.height / 2.0f);
    this->endGameText.setPosition(sf::Vector2f(windowSize.x / 2.0f, windowSize.y / 2.0f));

    this->waveText.setFont(this->font);
    this->waveText.setFillColor(sf::Color::White);
    this->waveText.setCharacterSize(32);
    this->waveText.setString("Wave: 1");

    this->scoreText.setFont(this->font);
    this->scoreText.setFillColor(sf::Color::Black);
    this->scoreText.setCharacterSize(72);

    this->coinsText.setFont(this->font);
    this->coinsText.setFillColor(sf::Color::White);
    this->coinsText.setCharacterSize(32);
    this->coinsText.setString("Coins: 0");

}

void Game::initWaveConfigs()
{
    this->waveConfigs = {
        {20, 0, 0},  // Wave 1
        {35, 0, 0},  // Wave 2
        {25, 5, 0},  // Wave 3
        {35, 18, 0}, // Wave 4
        {5, 27, 0},  // Wave 5
        {15, 15, 4}, // Wave 6
        {20, 20, 5}, // Wave 7
        {10, 20, 14},// Wave 8
        {0, 0, 30},  // Wave 9
        {0, 102, 0}  // Wave 10


    };
}

void Game::updateSpells()
{
    //auto& spells = this->player.getSpells(); // Store reference to spells vector
    std::cout << this->spells.size();
    for (auto& spell : this->spells)
    {
        // Update the spell's position and other properties
        if (spell.getIsActive()) {
            spell.update(this->windowSize);

            // Check if the spell is out of the window bounds
            if (spell.getPosition().x < 0 || spell.getPosition().x > this->windowSize.x ||
                spell.getPosition().y < 0 || spell.getPosition().y > this->windowSize.y) {
                spell.setIsActive(false); // Deactivate the spell
            }
        }
    }

    // Remove inactive spells from the container
    this->spells.erase(std::remove_if(this->spells.begin(), this->spells.end(), [](const Spell& spell) {
        return !spell.getIsActive();
        }), this->spells.end());
}





void Game::renderSpells(sf::RenderTarget& target)
{
    // Debug log to check if spells container is empty
    //std::cout << "Number of spells to render: " << spells.size() << std::endl;

    for (auto& spell : this->spells)
    {
        // Render the spell on the screen
        spell.render(target); // Pass a pointer to sf::RenderTarget

        // Debug log for rendering
        //std::cout << "Spell rendered at position: (" << spell.getPosition().x << ", " << spell.getPosition().y << ")" << std::endl;
    }
}


Game::Game(const std::shared_ptr<sf::RenderWindow>& window) : player(this->spells, textureManager, 50.0f, 50.0f)
{
    // Initialize other variables and resources
    sf::ContextSettings settings = window->getSettings();

    this->window = window;
    this->initVariables();
    this->initWindow();
    this->initFonts();
    this->initText();
    this->startNewWave(); // Start the first wave immediately

    // Initialize spells container
    // You may want to adjust the initial capacity depending on expected usage
    this->spells.reserve(100); // Reserve space for 100 spells, for example

}

const sf::Vector2u& Game::getWindowSize() const {
    return windowSize;
}

sf::Packet Game::getPacket()
{
    return this->largePacket;
}

void Game::recPacket(sf::Packet packet)
{
    Data::processLargePacket(packet, this->players, this->textureManager, this->spells, this->turrets);
}

const bool& Game::getEndGame() const
{
    return this->endGame;
}

const bool Game::running() const
{
    return this->window->isOpen();
}

void Game::pollEvents()
{
    while (this->window->pollEvent(this->sfmlEvent))
    {
        switch (this->sfmlEvent.type)
        {
        case sf::Event::Closed:
            this->window->close();
            break;
        case sf::Event::KeyPressed:
            if (this->sfmlEvent.key.code == sf::Keyboard::Escape)
                this->window->close();
            break;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::E) && coins >= 50 && !placingTurret) {
            placingTurret = true;
            turretPlacementPosition = sf::Vector2f(sf::Mouse::getPosition(*window));
            coins -= 50; // Subtract score when placing a turret
        }
    }
}


void Game::startNewWave()
{
    if (this->wave > waveConfigs.size())
    {
        this->endGame = true; // No more waves defined
        return;
    }

    this->currentWaveConfig = this->waveConfigs[this->wave - 1];
    this->slimesToSpawn = this->currentWaveConfig.blueSlimes + this->currentWaveConfig.redSlimes + this->currentWaveConfig.greenSlimes;
    this->slimesSpawned = 0;
    this->spawnTimer = this->spawnTimerMax; // Ensure immediate spawn

    std::stringstream ss;
    ss << "Wave: " << this->wave;
    this->waveText.setString(ss.str());

    this->wave++;
    this->allSlimesSpawned = false; // Set to false to start spawning
}

void Game::handleTurretPlacement() {
    if (placingTurret) {
        // Check if the mouse button is released to place the turret
        if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
            // Get the mouse position and adjust it for window coordinates
            sf::Vector2f mousePos = window->mapPixelToCoords(sf::Mouse::getPosition(*window));

            // Create a new turret at the placement position
            this->turrets.emplace_back("archer", mousePos.x, mousePos.y, textureManager);
            placingTurret = false; // Reset the flag
        }
    }
}

void Game::spawnSlimes()
{
    if (this->spawnTimer >= this->spawnTimerMax && this->slimesSpawned < this->slimesToSpawn)
    {
        if (this->slimesSpawned < this->currentWaveConfig.blueSlimes)
        {
            this->slimes.emplace_back(*this->window, SlimeTypes::BLUE);
        }
        else if (this->slimesSpawned < this->currentWaveConfig.redSlimes + this->currentWaveConfig.blueSlimes)
        {
            this->slimes.emplace_back(*this->window, SlimeTypes::RED);
        }
        else if (this->slimesSpawned < this->slimesToSpawn)
        {
            this->slimes.emplace_back(*this->window, SlimeTypes::GREEN);
        }
        this->slimesSpawned++;
        this->spawnTimer = 0.f;
    }
    else if (this->slimesSpawned == this->slimesToSpawn) this->allSlimesSpawned = true;
}

void Game::updatePlayer()
{
    this->player.update(*this->window);

    this->player.updateSpellCooldown();

    if (this->player.getHp() <= 0)
        this->endGame = true;
}


void Game::updateCollision()
{
    for (size_t i = 0; i < this->slimes.size(); i++)
    {
        if (this->player.getSprite().getGlobalBounds().intersects(this->slimes[i].getShape().getGlobalBounds()))
        {
            this->player.takeDamage(1);
            //this->slimes.erase(this->slimes.begin() + i);
        }
        for (size_t i = 0; i < this->slimes.size(); i++)
        {
            if (this->slimes[i].isTouchingWindowEdge(this->window.get()))
            {
                this->player.takeDamage(1);
                break; // Apply damage only once per cooldown
            }
        }
    }
}

void Game::updateGui()
{
    std::stringstream ss;
    ss << " - Points: " << this->points << "\n"
        << " - Health: " << this->player.getHp() << " / " << this->player.getHpMax() << "\n";
    this->guiText.setString(ss.str());

    sf::Vector2u windowSize = window->getSize();
    sf::FloatRect waveTextRect = this->waveText.getLocalBounds();
    this->waveText.setPosition(windowSize.x - waveTextRect.width - 20.0f, windowSize.y - waveTextRect.height - 52.0f);

    std::stringstream coinsSs;
    coinsSs << "Coins: " << this->coins;
    this->coinsText.setString(coinsSs.str());
    sf::FloatRect coinsTextRect = this->coinsText.getLocalBounds();
    this->coinsText.setPosition(windowSize.x - coinsTextRect.width - 20.0f, 20.0f);
}

void Game::updateSpellCollisions()
{
    //auto& spells = this->player.getSpells(); // Get the player's spells

    for (auto& spell : this->spells)
    {
        if (spell.getIsActive()) // Only check active spells
        {
            for (auto& slime : this->slimes)
            {
                if (spell.getBounds().intersects(slime.getShape().getGlobalBounds()))
                {
                    points += slime.takeDamage(spell.getDamage()); // Apply damage to the slime
                    coins += spell.getDamage();
                    spell.setIsActive(false); // Deactivate the spell
                    break; // Move to the next spell
                }
            }
        }
    }

    // Check collisions for turret spells
    for (auto& turret : this->turrets)
    {
        auto& turretSpells = turret.getSpells();
        for (auto& spell : turretSpells)
        {
            if (spell.getIsActive())
            {
                for (auto& slime : this->slimes)
                {
                    if (spell.getBounds().intersects(slime.getShape().getGlobalBounds()))
                    {
                        points += slime.takeDamage(spell.getDamage()); // Apply damage to the slime
                        coins += spell.getDamage();
                        spell.setIsActive(false); // Deactivate the spell
                        break; // Move to the next spell
                    }
                }

                // Check if the turret spell is out of the window bounds
                if (spell.getPosition().x < 0 || spell.getPosition().x > this->windowSize.x ||
                    spell.getPosition().y < 0 || spell.getPosition().y > this->windowSize.y)
                {
                    spell.setIsActive(false); // Deactivate the spell
                }
            }
        }

        // Remove inactive spells from turret
        turretSpells.erase(std::remove_if(turretSpells.begin(), turretSpells.end(), [](const Spell& spell) {
            return !spell.getIsActive();
            }), turretSpells.end());
    }

    // Remove inactive slimes
    this->slimes.erase(std::remove_if(this->slimes.begin(), this->slimes.end(), [this](Slime& slime) {
        if (slime.getHealth() <= 0.f) {
            return true;
        }
        return false;
        }), this->slimes.end());

    // Remove inactive spells
    spells.erase(std::remove_if(spells.begin(), spells.end(), [](const Spell& spell) {
        return !spell.getIsActive();
        }), spells.end());
}


void Game::update()
{
    static sf::Clock clock;
    float dt = clock.restart().asSeconds();

    this->pollEvents();

    if (!this->endGame && !this->victory)
    {
        this->spawnTimer += dt; // Update the spawn timer
        this->spawnSlimes();    // Spawn slimes based on the timer

        for (auto& slime : this->slimes)
        {
            slime.update(dt);
        }

        for (auto& turret : this->turrets)
        {
            turret.update(dt, slimes, this->textureManager);
            for (auto& spell : turret.getSpells())
            {
                spell.update(this->windowSize);
            }
        }

        this->updatePlayer();
        this->updateSpells(); // Update spells
        this->handleTurretPlacement();
        this->updateSpellCollisions(); // Check and handle spell collisions with slimes
        this->updateCollision();
        this->updateGui();

        // Check if all slimes are dead and spawned
        if (this->allSlimesSpawned && this->slimes.empty())
        {
            if (this->wave == 10) {
                this->victory = true; // Set victory state
                return; // Don't start a new wave
            }
            this->allSlimesSpawned = false;
            this->startNewWave();
        }
        Data::createLargePacket(largePacket, players, spells, turrets, slimes, this->player);
    }
}

void Game::renderGui(sf::RenderTarget* target)
{
    target->draw(this->guiText);
    target->draw(this->waveText); // Draw the wave text
    target->draw(this->coinsText);
}

void Game::render()
{
    largePacket.clear();
    this->window->clear(sf::Color::Blue);
    this->window->draw(this->backgroundSprite);

    for (auto& slime : this->slimes)
    {
        slime.render(*this->window);
    }
    //std::cout << "RENDERING..." << std::endl;
    for (auto& spell : this->spells)
    {
        std::cout << this->spells.size() << " Size"<< std::endl;
        spell.render(*this->window);
    }

    for (auto& turret : this->turrets)
    {
        turret.render(*this->window);
    }

    for (auto& player : this->players)
    {
        player.render(*this->window);
    }

    this->player.render(*this->window);

    this->renderSpells(*this->window); // Render spells

    this->renderGui(this->window.get());

    if (this->endGame)
        this->window->draw(this->endGameText);

    if (this->victory) {
        // Draw the victory sprite and score text
        this->window->clear(sf::Color::Blue);
        this->window->draw(this->victorySprite); // Draw victory image
        std::stringstream ss;
        ss << "Score: " << this->points;
        this->scoreText.setString(ss.str());
        sf::FloatRect scoreTextRect = this->scoreText.getLocalBounds();
        this->scoreText.setOrigin(scoreTextRect.left + scoreTextRect.width / 2.0f, scoreTextRect.top + scoreTextRect.height / 2.0f);
        this->scoreText.setPosition(sf::Vector2f(windowSize.x / 2.0f, windowSize.y - 100.0f)); // Adjust Y position as needed
        this->window->draw(this->scoreText); // Draw score text
    }
}

