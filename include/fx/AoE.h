#pragma once

#include <SFML/Graphics.hpp>
#include "util/AnimUtil.h"

class GameState;

// should make pointer for polymorphism
class AoE {

private:
    sf::Sprite sprite;
    bool isActive;

public:
    AnimData animData;
    AoE(AnimData animData, sf::Vector2f position, bool isActive=true);
    static void UpdateAoE(GameState& state, float deltaTime);
    static void RenderAoE(GameState& state, bool drawHitbox = false);
    inline sf::Sprite& GetSprite() { return sprite;}
};