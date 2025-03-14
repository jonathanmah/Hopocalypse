#pragma once
#include <SFML/Graphics.hpp>

class TextureUtil {

public:
    static sf::Texture* LoadTexture(const std::string& texturePath);
    static std::unordered_map<std::string, sf::IntRect> LoadTextureAtlas(const std::string& xmlPath);
};