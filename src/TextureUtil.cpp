#include "TextureUtil.h"
#include <iostream>
#include <typeinfo>


sf::Texture* TextureUtil::LoadTexture(const std::string& texturePath) { 

    static std::unordered_map<std::string, sf::Texture> textures;

    if (textures.find(texturePath) != textures.end()) {
        return &textures[texturePath];
    }
    sf::Texture texture;
    if(!texture.loadFromFile(texturePath))
        throw std::runtime_error("Failed to load texture : " + texturePath);

    textures.emplace(texturePath, texture);
    return &textures[texturePath];
}