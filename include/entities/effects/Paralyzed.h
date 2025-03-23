#pragma once
#include <SFML/Graphics.hpp>
#include "entities/effects/StatusEffect.h"

class Character;

class Paralyzed : public StatusEffect {

public:
    sf::Sprite sprite;
    AnimData animData;
    float disabledCd;

    Paralyzed(Character* character);
    bool UpdateStatusEffect(float deltaTime) override;
    void UpdateDisabledCd(float deltaTime);
    void AttemptApplyEffect(float duration);
    inline bool OnCooldown() {return disabledCd > 0.f;}
};