#pragma once
#include <SFML/Graphics.hpp>

class TextureUtil {

public:
    static sf::Texture* GetTexture(const std::string& key);
    static void SetStaticMemberTextures();
};