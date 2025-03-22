#include "fx/Paralysis.h"
#include "core/GameState.h"

Paralysis::Paralysis(AnimData animData, sf::Vector2f position, Character& character) :
    StatusEffect(animData, position, character, 3.f) {
        sprite.setScale({.9f,.9f});
    }

bool Paralysis::Update(float deltaTime) {
    timeActive -= deltaTime;
    return AnimUtil::UpdateSpriteAnim(sprite, animData, deltaTime);
}