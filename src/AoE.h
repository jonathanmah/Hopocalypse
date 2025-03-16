#pragma once

#include <SFML/Graphics.hpp>
#include "AnimUtil.h"
#include "BatchRenderer.h"
#include "HitboxDebugger.h"

// should make pointer for polymorphism
class AoE {

private:
    sf::Sprite sprite;
    

public:
    AnimData animData;
    AoE(AnimData animData, sf::Vector2f position);
    static void UpdateAoE(std::vector<std::unique_ptr<AoE>>& aoe, float deltaTime);
    static void RenderAoE(std::vector<std::unique_ptr<AoE>>& aoe, BatchRenderer& batchRenderer, sf::RenderWindow& window, bool drawHitbox = false);
    inline sf::Sprite& GetSprite() { return sprite;}
};