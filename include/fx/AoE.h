#pragma once
#include <SFML/Graphics.hpp>
#include "util/AnimUtil.h"

class GameState;

class AoE {

protected:
    sf::Sprite sprite;
    bool isActive;

public:
    
    AnimData animData;
    AoE(AnimData animData, sf::Vector2f position);
    virtual ~AoE() = default;
    static void UpdateAoE(GameState& state, float deltaTime);
    virtual void DrawHitbox(sf::RenderWindow& window) = 0;
    virtual bool Update(GameState& state, float deltaTime) = 0; // returns true when effect is over

    static void RenderAoE(GameState& state, bool drawHitbox = false);
    inline sf::Sprite& GetSprite() { return sprite;}
};