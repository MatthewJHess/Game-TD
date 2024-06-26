#include "Data.h"
#include <SFML/Network.hpp>
#include <iostream>
#include <string>
#include <cmath>

void Data::processLargePacket(sf::Packet& largePacket, std::vector<OtherPlayer>& players, TextureManager& textureManager, std::vector<Spell>& spells, std::vector<Turret>& turrets) {
    sf::Packet playerPacket, spellPacket, turretPacket;
    //std::cout << largePacket.getDataSize() << " is the size of the rec pack" << std::endl;
    while (!largePacket.endOfPacket()) {
        std::string type;
        largePacket >> type;
        //std::cout << type.size() << std::endl;
        if (type == "Player") {
            sf::Packet temp;
            std::string t;
            float posX, posY, direction;
            largePacket >> t >> posX >> posY >> direction;
            //std::cout << t << std::endl;
            //std::cout << "(" << posX << " ," << posY << ") " << direction << std::endl;
            temp << t << posX << posY << direction;
            playerPacket.append(temp.getData(), temp.getDataSize());
            temp.clear();
            //std::cout << "Player packet received." << std::endl;
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
            //std::cout << "Spell packet received." << std::endl;
        }
        else if (type == "Turret") {
            sf::Packet temp;
            std::string t;
            float posX, posY, direction;
            largePacket >> t >> posX >> posY >> direction;
            temp << t << posX << posY << direction;
            turretPacket.append(temp.getData(), temp.getDataSize());
            temp.clear();
            std::cout << "Turret packet received." << std::endl;
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
}

void Data::rPlayerData(sf::Packet& packet, std::vector<OtherPlayer>& players) {
    while (!packet.endOfPacket()) {
        std::string type;
        float posX, posY, health, direction;
        //std::cout << "Player data"<< std::endl;
        if (packet >> type >> posX >> posY >> direction) {
            bool found = false;
            for (auto& x : players) {
                if (x.getColor() == type) {
                    x.update(posX, posY, direction);
                    found = true;
                    //std::cout << "FOUND!" << std::endl;
                    break;
                }
            }
            if (!found) {
                OtherPlayer player(type, posX, posY, direction);  // Assuming a suitable constructor
                players.push_back(player);
                //std::cout << "NOT FOUND!" << std::endl;
            }
        }
        else {
            // Handle error (e.g., corrupted packet data)
            break;
        }
    }
}

void Data::rSpellData(sf::Packet& packet, TextureManager& textureManager, std::vector<Spell>& spells) {
    while (!packet.endOfPacket()) {
        std::string type;
        float posX, posY;
        sf::Vector2f direction;
        if (packet >> type >> posX >> posY >> direction.x >> direction.y) {
            sf::Texture texture = textureManager.getTexture(type);
            SpellType stype = SpellType::Fire; //default
            if (type == "Fire") stype = SpellType::Fire;
            Spell spell(stype, texture);
            sf::Vector2f v = { posX, posY };
            spell.setPosition(v);
            spell.setVelocity(direction * spell.getMovementSpeed());
            sf::Vector2f vel = direction * spell.getMovementSpeed();
            std::cout << vel.x << ", " << vel.y << std::endl;
            spells.push_back(spell);
        }
        else {
            // Handle error (e.g., corrupted packet data)
            break;
        }
    }
}

void Data::rTurretData(sf::Packet& packet, TextureManager& textureManager, std::vector<Turret>& turrets) {
    while (!packet.endOfPacket()) {
        std::string type;
        float posX, posY, direction;
        if (packet >> type >> posX >> posY >> direction) {
            Turret turret(type, posX, posY, textureManager);
            turrets.push_back(turret);
        }
        else {
            // Handle error (e.g., corrupted packet data)
            break;
        }
    }
}

void Data::createLargePacket(sf::Packet& largePacket, std::vector<OtherPlayer>& players, std::vector<Spell>& spells, std::vector<Turret>& turrets, std::vector<Slime>& slimes, Player p1)
{
    sf::Packet pPacket;
    std::string type = "Player";
    std::string color = "Blue";
    float posX = p1.getX();
    float posY = p1.getY();
    float direction = p1.getDirection();

    pPacket << type << color << posX << posY << direction;
    largePacket.append(pPacket.getData(), pPacket.getDataSize());
    for (auto& player : players) {
        sf::Packet playerPacket;
        std::string type = "Player";
        std::string color = player.getColor();
        float posX = player.getX();
        float posY = player.getY();
        float direction = player.getDirection();

        playerPacket << type << color << posX << posY << direction;
        largePacket.append(playerPacket.getData(), playerPacket.getDataSize());
    }

    for (auto& spell : spells) {
        sf::Packet spellPacket;
        std::string type = "Spell";
        std::string spellType = spell.getTypeAsString();
        float posX = spell.getPosition().x;
        float posY = spell.getPosition().y;

        spellPacket << type << spellType << posX << posY << 0.f;
        largePacket.append(spellPacket.getData(), spellPacket.getDataSize());
    }

    for (auto& turret : turrets) {
        sf::Packet turretPacket;
        std::string type = "Turret";
        std::string turretType = turret.getType();
        float posX = turret.getX();
        float posY = turret.getY();
        float direction = turret.getDirection();
        std::cout << "Turret type: "<<turretType << std::endl;
        turretPacket << type << turretType << posX << posY << direction;
        largePacket.append(turretPacket.getData(), turretPacket.getDataSize());
    }

    for (auto& slime : slimes) {
        sf::Packet slimePacket;
        std::string type = "Slime";
        std::string slimeType = slime.getSType();
        float posX = slime.getX();
        float posY = slime.getY();

        slimePacket << type << slimeType << posX << posY;
        largePacket.append(slimePacket.getData(), slimePacket.getDataSize());
    }
    std::cout << largePacket.getDataSize() << std::endl;
}
