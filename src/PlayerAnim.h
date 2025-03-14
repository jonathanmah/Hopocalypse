#pragma once
#include <SFML/Graphics.hpp>
#include "TextureUtil.h"

class PlayerAnim {

public:
    sf::Texture* texture;
    std::unordered_map<std::string, sf::IntRect> animations;
    PlayerAnim();
    void InitTexture();
    sf::IntRect GetAnim(const std::string& name);
    

};