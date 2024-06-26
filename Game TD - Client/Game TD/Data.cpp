#include "Data.h"
#include <SFML/Network.hpp>
#include <iostream>
#include <string>

void Data::processLargePacket(sf::Packet& largePacket, std::vector<OtherPlayer>& players, TextureManager& textureManager, std::vector<Spell>& spells, std::vector<Turret>& turrets, std::vector<Slime>& slimes) {
    sf::Packet playerPacket, spellPacket, turretPacket, slimePacket;

    while (!largePacket.endOfPacket()) {
        std::string type;
        largePacket >> type;

        if (type == "Player") {
            sf::Packet temp;
            std::string t;
            float posX, posY, direction;
            largePacket >> t >> posX >> posY >> direction;
            temp << t << posX << posY << direction;
            playerPacket.append(temp.getData(), temp.getDataSize());
            temp.clear();
        }
        else if (type == "Spell") {
            sf::Packet temp;
            std::string t;
            float posX, posY;
            sf::Vector2f direction;
            largePacket >> t >> posX >> posY >> direction.x >> direction.y;
            temp << t << posX << posY << direction.x << direction.y;
            spellPacket.append(temp.getData(), temp.getDataSize());
            temp.clear();
        }
        else if (type == "Turret") {
            sf::Packet temp;
            std::string t;
            float posX, posY, direction;
            largePacket >> t >> posX >> posY >> direction;
            temp << t << posX << posY << direction;
            turretPacket.append(temp.getData(), temp.getDataSize());
            temp.clear();
        }
        else if (type == "Slime") {
            sf::Packet temp;
            std::string t;
            float posX, posY;
            largePacket >> t >> posX >> posY;
            temp << t << posX << posY;
            slimePacket.append(temp.getData(), temp.getDataSize());
        }
        else {
            // Handle unknown type or error
            std::cerr << "Unknown packet type: " << type << std::endl;
            break;
        }
    }

    rPlayerData(playerPacket, players);
    rSpellData(spellPacket, textureManager, spells);
    rTurretData(turretPacket, textureManager, turrets);
    rSlimeData(slimePacket, textureManager, slimes);
}

void Data::rPlayerData(sf::Packet& packet, std::vector<OtherPlayer>& players) {
    while (!packet.endOfPacket()) {
        std::string type;
        float posX, posY, health, direction;

        if (packet >> type >> posX >> posY >> direction) {
            bool found = false;
            for (auto& x : players) {
                if (x.getColor() == type) {
                    x.update(posX, posY, direction);
                    found = true;
                    break;
                }
            }
            if (!found) {
                OtherPlayer player(type, posX, posY, direction);  // Assuming a suitable constructor
                players.push_back(player);
            }
        }
        else {
            // Handle error (e.g., corrupted packet data)
            break;
        }
    }
}

void Data::rSpellData(sf::Packet& packet, TextureManager& textureManager, std::vector<Spell>& spells) {
    spells.clear();
    while (!packet.endOfPacket()) {
        std::string type;
        float posX, posY, direction;
        if (packet >> type >> posX >> posY >> direction) {
            sf::Texture texture = textureManager.getTexture(type);
            Spell spell(type, texture, posX, posY, direction);
            spells.push_back(spell);
        }
        else {
            // Handle error (e.g., corrupted packet data)
            break;
        }
    }
}

sf::Packet& Data::createLargePacket(sf::Packet& largePacket, sf::Packet& send)
{
    largePacket.append(send.getData(), send.getDataSize());
    return largePacket;
}

void Data::rTurretData(sf::Packet& packet, TextureManager& textureManager, std::vector<Turret>& turrets) {
    while (!packet.endOfPacket()) {
        std::string type;
        float posX, posY, direction;
        if (packet >> type >> posX >> posY >> direction) {
            bool found = false;
            sf::Vector2f v = { posX, posY };
            for (const auto& t : turrets) {
                if (t.position == v) {
                    found = true;
                    break;
                }
            }
            if (!found) {
                sf::Texture texture = textureManager.getTexture(type);
                Turret turret(type, texture, posX, posY, direction);
                turrets.push_back(turret);
            }
        }
        else {
            // Handle error (e.g., corrupted packet data)
            break;
        }
    }
}

void Data::rSlimeData(sf::Packet& packet, TextureManager& textureManager, std::vector<Slime>& slimes) {
    slimes.clear();
    while (!packet.endOfPacket()) {
        int type;
        float posX, posY;
        if (packet >> type >> posX >> posY) {
            Slime slime(type, posX, posY);
            slimes.push_back(slime);
        }
        else {
            // Handle error (e.g., corrupted packet data)
            break;
        }
    }
}
