#pragma once

#include <iostream>
#include <ctime>
#include <vector>
#include <sstream>
#include <memory>
#include <SFML/Graphics.hpp>
#include <SFML/Network.hpp>

#include "Player.h"
#include "OtherPlayer.h"
#include "Slime.h"
#include "Spell.h"
#include "Turret.h"
#include "TextureManager.h"

class Game
{
private:
    struct WaveConfig {
        int blueSlimes;
        int redSlimes;
        int greenSlimes;
    };

    sf::VideoMode videoMode;
    std::shared_ptr<sf::RenderWindow> window;
    bool endGame = false;
    bool victory = false;
    bool allSlimesSpawned = false;
    sf::Event sfmlEvent;
    sf::Vector2u windowSize;
    bool placingTurret = false; // Flag to indicate if the player is placing a turret
    sf::Vector2f turretPlacementPosition; // Position where the turret will be placed
    std::vector<Turret> turrets;

    TextureManager textureManager;

    Player player;

    int points = 0;
    int wave = 0;
    int slimesToSpawn = 0;
    int slimesSpawned = 0;
    int coins = 0;
    std::vector<Spell> spells;
    sf::TcpSocket& socket;

    sf::Font font;
    sf::Text guiText;
    sf::Text endGameText;
    sf::Text waveText;
    sf::Text scoreText;
    sf::Text coinsText;

    sf::Texture backgroundTexture;
    sf::Sprite backgroundSprite;
    sf::Texture victoryTexture;
    sf::Sprite victorySprite;

    sf::Packet largePacket;
    int type;
    sf::Texture archer;

    std::vector<Slime> slimes;
    std::vector<OtherPlayer> players;
    float spawnTimerMax = 0.0f;
    float spawnTimer = 0.0f;

    void handleTurretPlacement();

    void initVariables();
    void initWindow();
    void initFonts();
    void initText();

public:
    Game(const std::shared_ptr<sf::RenderWindow>& window, sf::TcpSocket& socket, int type);
    void updateGameData(const std::vector<OtherPlayer>& players, const std::vector<Spell>& spells, const std::vector<Turret>& turrets, const std::vector<Slime>& slimes);
    const bool& getEndGame() const;
    const bool running() const;
    void pollEvents();
    const sf::Vector2u& getWindowSize() const;
    void updatePlayer();
    // void updateCollision();
    void updateGui();
    void update();
    void receiveAndProcessPackets();
    void setPacket(sf::Packet packet);

    void renderGui(sf::RenderTarget* target);
    void render();
};
