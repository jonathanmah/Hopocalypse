#include "entities/effects/Slowed.h"
#include "util/AnimUtil.h"
#include "entities/Character.h"
#include <cmath>

Slowed::Slowed(Character& character)
: StatusEffect(character), slowFactor(1.f), updatedColour(false)
{}

void Slowed::UpdateColour() {
    if(!updatedColour&&character.health > 0.f){
        character.GetSprite().setColor(sf::Color(60, 220, 255));
        updatedColour = true;
    }
}

void Slowed::ApplySlow(float duration, float newSlowFactor){
    slowFactor = newSlowFactor;
    ApplyEffect(duration);    
    if(!updatedColour) {
        UpdateColour();
    }
}

bool Slowed::UpdateStatusEffect(float deltaTime) {
    if(UpdateTimeLeft(deltaTime)) { // returns true if timer is empty and no effect on
        // for other statuses, can reset to defaults here after effect ends
        slowFactor = 1.f;
        character.GetSprite().setColor(sf::Color{255,255,255});
        updatedColour = false;
        return true;
    }
    return false;
}