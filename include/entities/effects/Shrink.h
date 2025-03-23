#pragma once
#include <SFML/Graphics.hpp>
#include "entities/effects/StatusEffect.h"

class Character;

class Shrink : public StatusEffect {

public:
    sf::Vector2f originalScale;
    float sizeFactor;
    bool updatedSize;
    float initialTime;
    
    Shrink(Character* character);
    void ApplyShrink(float duration, float newSizeFactor);
    bool UpdateStatusEffect(float deltaTime) override;
};