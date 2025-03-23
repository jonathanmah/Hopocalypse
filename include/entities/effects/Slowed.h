#pragma once
#include <SFML/Graphics.hpp>
#include "entities/effects/StatusEffect.h"

class Character;

class Slowed : public StatusEffect {

public:
    float slowFactor;
    bool updatedColour;
    
    Slowed(Character* character);
    void ApplySlow(float duration, float newSlowFactor);
    bool UpdateStatusEffect(float deltaTime) override;
    void UpdateColour();
};