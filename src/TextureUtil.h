#ifndef TEXTURE_UTIL_H
#define TEXTURE_UTIL_H
#include <SFML/Graphics.hpp>

class TextureUtil {

public:
    static sf::Texture* LoadTexture(const std::string& texturePath);
};

#endif