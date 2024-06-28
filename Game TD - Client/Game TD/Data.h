#pragma once

#include <SFML/Network.hpp>
#include <vector>
#include "OtherPlayer.h"
#include "Spell.h"
#include "Turret.h"
#include "Slime.h"
#include "TextureManager.h"

class Data {
public:
    static void processLargePacket(sf::Packet& largePacket, std::vector<OtherPlayer>& players, TextureManager& textureManager, std::vector<Spell>& spells, std::vector<Turret>& turrets, std::vector<Slime>& slimes);
    static void rPlayerData(sf::Packet& packet, TextureManager& textureManager, std::vector<OtherPlayer>& players);
    static void rSpellData(sf::Packet& packet, TextureManager& textureManager, std::vector<Spell>& spells);
    static void rTurretData(sf::Packet& packet, TextureManager& textureManager, std::vector<Turret>& turrets);
    static void rSlimeData(sf::Packet& packet, TextureManager& textureManager, std::vector<Slime>& slimes);
    sf::Packet& createLargePacket(sf::Packet& largePacket, sf::Packet& send);
};
