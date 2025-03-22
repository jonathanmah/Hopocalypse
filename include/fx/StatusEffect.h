#pragma once
#include <SFML/Graphics.hpp>
#include "util/AnimUtil.h"

class GameState;
class Character;

class StatusEffect {

protected:
    sf::Sprite sprite;
    Character& character;

public:
    
    AnimData animData;
    float timeActive;

    StatusEffect(AnimData animData, sf::Vector2f position, Character& character, float timeActive);
    virtual ~StatusEffect() = default;
    static void UpdateStatusEffect(GameState& state, float deltaTime);
    
    virtual bool Update(float deltaTime) = 0; // returns true when effect is over

    static void RenderStatusEffects(GameState& state, bool drawHitbox = false);
    inline sf::Sprite& GetSprite() { return sprite;}
};