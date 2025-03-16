#pragma once

#include <SFML/Graphics.hpp>
#include "AnimUtil.h"

class AoE {

private:
    sf::Sprite sprite;
    AnimData animData;

public:
    AoE(AnimData animData, sf::Vector2f position);
    void UpdateAnimation(float deltaTime);
};