#include "entities/monster/Monster.h"
#include "entities/effects/StatusEffect.h"
#include "entities/effects/OnFire.h"

StatusEffect::StatusEffect(Character& character) : character(character), timeLeft(0){
}

bool StatusEffect::UpdateTimeLeft(float deltaTime) {
    timeLeft -= deltaTime;
    if(timeLeft <= 0) {
        timeLeft = std::max(0.f, timeLeft);
        return true;
    }
    return false;
}

void StatusEffect::ApplyEffect(float duration) {
    timeLeft = duration;
}

void StatusEffect::RenderStatusEffects(std::vector<std::unique_ptr<Monster>>& monsters, sf::RenderWindow& window, BatchRenderer batchRenderer){
    for(auto& monster : monsters) {
        if(monster->IsDead()) continue;
        // #TO DO LATER IS BATCH RENDER EFFECTS
        if(monster->onFire.IsActive()) {
            window.draw(monster->onFire.sprite);
        }
        if(monster->paralyzed.IsActive()) {
            window.draw(monster->paralyzed.sprite);
        }

    }
}