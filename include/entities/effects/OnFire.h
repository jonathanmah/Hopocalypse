#pragma once
#include <SFML/Graphics.hpp>
#include "entities/effects/StatusEffect.h"

class Character;

class OnFire : public StatusEffect {

public:
    sf::Sprite sprite;
    AnimData animData;
    float dps;
    float damageTimer;
    float flamethrowerDmgCd;
    bool updatedColour;

    OnFire(Character* character);
    bool UpdateStatusEffect(float deltaTime) override;
    void UpdateColour();
    inline bool FlamethrowerDamageOnCooldown() {return flamethrowerDmgCd > 0.f;}
    inline void SetFlameThrowerDamageCooldown(float cooldown) {flamethrowerDmgCd = cooldown;}
};