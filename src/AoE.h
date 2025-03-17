#pragma once

#include <SFML/Graphics.hpp>
#include "AnimUtil.h"
#include "BatchRenderer.h"
#include "HitboxDebugger.h"

class GameState;
// should make pointer for polymorphism
class AoE {

private:
    sf::Sprite sprite;
    

public:
    AnimData animData;
    AoE(AnimData animData, sf::Vector2f position);
    static void UpdateAoE(GameState& state, float deltaTime);
    static void RenderAoE(GameState& state, bool drawHitbox = false);
    inline sf::Sprite& GetSprite() { return sprite;}
};