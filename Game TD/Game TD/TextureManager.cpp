#include "TextureManager.h"
#include <iostream>
#include <map>
#include <string>
#include <stdexcept>

bool TextureManager::loadTexture(const std::string& name, const std::string& filename) {
    sf::Texture texture;
    if (texture.loadFromFile(filename)) {
        textures[name] = texture;
        std::cout << "Number of txts: " << textures.size() << std::endl;
        return true;
    }
    std::cerr << "Failed to load texture from file: " << filename << std::endl;
    return false;
}

sf::Texture& TextureManager::getTexture(const std::string& textureName) {
    auto it = textures.find(textureName);
    if (it != textures.end()) {
        //std::cout << "LOADED TEXTURE: " << textureName << std::endl;
        return it->second;
    }
    else {
        std::cerr << "Texture not found: " << textureName << std::endl;
        std::cout << "Number of txts: " << textures.size() << std::endl;
        throw std::out_of_range("Texture not found: " + textureName);
    }
}
