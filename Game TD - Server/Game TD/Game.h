#pragma once

#include <iostream>
#include <ctime>
#include <vector>
#include <sstream>
#include <memory>
#include <SFML/Graphics.hpp>

#include "Player.h"
#include "Slime.h"
#include "Spell.h"
#include "Turret.h"
#include "TextureManager.h"
#include "OtherPlayer.h"
#include "Data.h"

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
    bool endGame;
    bool victory = false;
    bool allSlimesSpawned;
    sf::Event sfmlEvent;
    sf::Vector2u windowSize;
    bool placingTurret; // Flag to indicate if the player is placing a turret
    sf::Vector2f turretPlacementPosition; // Position where the turret will be placed
    void handleTurretPlacement();
    std::vector<Turret> turrets;

    TextureManager textureManager;

    Player player;

    int points;
    int wave;
    int slimesToSpawn;
    int slimesSpawned;
    int coins;
    std::vector<Spell> spells;

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

    sf::Texture archer;

    std::vector<Slime> slimes;
    std::vector<OtherPlayer> players;
    float spawnTimerMax;
    float spawnTimer;

    std::vector<WaveConfig> waveConfigs;
    WaveConfig currentWaveConfig;
    sf::Packet largePacket;

    void updateSpells();
    void renderSpells(sf::RenderTarget& target);

    void initVariables();
    void initWindow();
    void initFonts();
    void initText();
    void initWaveConfigs();

public:
    Game(const std::shared_ptr<sf::RenderWindow>& window);

    const bool& getEndGame() const;
    const bool running() const;
    void pollEvents();
    const sf::Vector2u& getWindowSize() const;
    sf::Packet getPacket();
    void recPacket(sf::Packet packet);

    void spawnSlimes();
    void startNewWave();
    void updatePlayer();
    void updateCollision();
    void updateGui();
    void update();
    void updateSpellCollisions();

    void renderGui(sf::RenderTarget* target);
    void render();
};
