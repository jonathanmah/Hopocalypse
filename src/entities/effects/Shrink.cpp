#include "entities/effects/Shrink.h"
#include "util/AnimUtil.h"
#include "entities/Character.h"
#include "entities/monster/Monster.h"
#include <cmath>

constexpr float INTERPOLATION_FACTOR = 0.1f;

Shrink::Shrink(Character& character)
: StatusEffect(character),
    originalScale({0,0}),
    sizeFactor(1.f),
    updatedSize(false),
    initialTime(0.f)
{}

void Shrink::ApplyShrink(float duration, float newSizeFactor) {
    ApplyEffect(duration);
    if(!updatedSize&&character.health > 0.f){
        initialTime = duration;
        originalScale = character.GetSprite().getScale();
        sizeFactor = newSizeFactor;
        character.GetSprite().setScale(originalScale*sizeFactor);
        updatedSize = true;
    }
}

bool Shrink::UpdateStatusEffect(float deltaTime) {
    Monster* monster = dynamic_cast<Monster*>(&character);
    if(UpdateTimeLeft(deltaTime)) { // returns true if timer is empty and no effect on
        // for other statuses, can reset to defaults here after effect ends
        if(monster->xAxisInverted) {
            monster->GetSprite().setScale({-abs(originalScale.x), originalScale.y});
        } else {
            monster->GetSprite().setScale({abs(originalScale.x), originalScale.y});
        }
        updatedSize = false;
        return true;
    }
    if(!monster->IsDead() && timeLeft < .5f){
        auto currScale = monster->GetSprite().getScale();
        sf::Vector2f scaleAbs = sf::Vector2f{abs(currScale.x),currScale.y} + sf::Vector2f{(abs(originalScale.x) - abs(currScale.x)), originalScale.y-currScale.y} * INTERPOLATION_FACTOR;
        if(monster->xAxisInverted) {
            monster->GetSprite().setScale({-scaleAbs.x, scaleAbs.y});
        } else {
            monster->GetSprite().setScale({scaleAbs.x, scaleAbs.y});
        }
    }
        
    return false;
}