#pragma once

#include <SFML/Graphics.hpp>
#include <map>
#include <string>
#include <sstream>

class TextureManager
{
private:
    std::map<std::string, sf::Texture> textures;

public:
    TextureManager() = default;
    ~TextureManager() = default;

    static TextureManager& getInstance() {
        static TextureManager instance;
        return instance;
    }

    bool loadTexture(const std::string& name, const std::string& filename);
    sf::Texture& getTexture(const std::string& textureName);
};
