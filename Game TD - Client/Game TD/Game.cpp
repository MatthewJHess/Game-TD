#include "Game.h"
#include "Slime.h"
#include "Spell.h"
#include "Player.h"
#include "OtherPlayer.h"
#include "Data.h"
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
    this->textureManager.loadTexture("arrow", "GameMaps/Arrow.png");
    this->textureManager.loadTexture("archer", "GameMaps/Archer.png");
    this->textureManager.loadTexture("wizardRed", "GameMaps/WizardRed.png");
    this->textureManager.loadTexture("1", "GameMaps/WizardRed.png");
    this->textureManager.loadTexture("0", "GameMaps/Wizard.png");

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



void Game::updateGameData(const std::vector<OtherPlayer>& players, const std::vector<Spell>& spells, const std::vector<Turret>& turrets, const std::vector<Slime>& slimes)
{
    this->spells = spells;
    this->turrets = turrets;
    this->slimes = slimes;
    this->players = players;
}

Game::Game(const std::shared_ptr<sf::RenderWindow>& window, sf::TcpSocket& socket, int type)
    : window(window), socket(socket), player(textureManager, 50.0f, 50.0f, &socket, &largePacket, type)
{
    // Initialize other variables and resources
    sf::ContextSettings settings = window->getSettings();

    this->initVariables();
    this->initWindow();
    this->initFonts();
    this->initText();
    //this->startNewWave(); // Start the first wave immediately

    // Initialize spells container
    // You may want to adjust the initial capacity depending on expected usage
    this->spells.reserve(100); // Reserve space for 100 spells, for example
}

const sf::Vector2u& Game::getWindowSize() const {
    return windowSize;
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



void Game::handleTurretPlacement() {
    if (placingTurret) {
        // Check if the mouse button is released to place the turret
        if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
            // Get the mouse position and adjust it for window coordinates
            sf::Vector2f mousePos = window->mapPixelToCoords(sf::Mouse::getPosition(*window));
            sf::Texture texture = textureManager.getTexture("archer");
            // Create a new turret at the placement position
            this->turrets.emplace_back("", texture, mousePos.x, mousePos.y, 0.f);
            sf::Packet packet;
            packet << "Turret" << "archer" << mousePos.x << mousePos.y << 0.f;
            largePacket.append(packet.getData(), packet.getDataSize());
            placingTurret = false; // Reset the flag
        }
    }
}

void Game::updatePlayer()
{
    this->player.update(*this->window);

    this->player.updateSpellCooldown();

    if (this->player.getHp() <= 0)
        this->endGame = true;
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

void Game::receiveAndProcessPackets() {
    sf::Packet largePacket;
    std::vector<Slime> slimes;
    std::vector<OtherPlayer> players;
    std::vector<Spell> spells;
    std::vector<Turret> turrets;
    TextureManager textureManager;

    sf::Socket::Status status = socket.receive(largePacket);
    if (status == sf::Socket::Done)
    {
        Data::processLargePacket(largePacket, players, textureManager, spells, turrets, slimes);
    }
    else if (status == sf::Socket::Disconnected)
    {
        // Handle disconnection
    }
    else if (status == sf::Socket::Error)
    {
        // Handle error
    }
}

void Game::setPacket(sf::Packet packet)
{
    this->largePacket = packet;
}

void Game::update()
{
    static sf::Clock clock;
    float dt = clock.restart().asSeconds();
    if (socket.receive(largePacket) == sf::Socket::Done) {
        Data::processLargePacket(largePacket, this->players, this->textureManager, this->spells, this->turrets, this->slimes);
        this->updateGameData(this->players, this->spells, this->turrets, this->slimes);
        largePacket.clear();
    }

    this->pollEvents();

    if (!this->endGame && !this->victory)
    {
        this->updatePlayer();
        this->handleTurretPlacement();
        this->updateGui();
        socket.send(largePacket);
        largePacket.clear();
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
    this->window->clear(sf::Color::Blue);
    this->window->draw(this->backgroundSprite);

    for (auto& slime : this->slimes)
    {
        slime.render(*this->window);
    }

    for (auto& spell : this->spells)
    {
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

    this->renderGui(this->window.get());

    if (this->endGame)
    {
        this->window->draw(this->endGameText);
    }

    if (this->victory)
    {
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
