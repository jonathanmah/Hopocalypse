#pragma once
#include <SFML/Graphics.hpp>
#include "util/AnimUtil.h"

class GameState;
class Character;
class Monster;
class BatchRenderer;

// passive effect is defined as not changing behaviour, but applying some effect to the character.
// like a parameter update, damage taken over time, make slower, but still trying to run and not completely still
// with a different animation. // with slow, could update animation to look slower. 


class StatusEffect {
    // shrink, burning, slowed

public:
    Character* character;
    float timeLeft;

    StatusEffect(Character* character);
    virtual ~StatusEffect() = default;
    virtual bool UpdateStatusEffect(float deltaTime) = 0; // returns true when effect is done
    bool UpdateTimeLeft(float deltaTime);
    void ApplyEffect(float duration);
    inline bool IsActive(){return timeLeft > 0.f;}
    static void RenderStatusEffects(std::vector<std::unique_ptr<Monster>>& monsters, sf::RenderWindow& window, BatchRenderer batchRenderer);

};