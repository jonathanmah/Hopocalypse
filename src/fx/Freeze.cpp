#include "fx/Freeze.h"
#include "core/GameState.h"

Freeze::Freeze(AnimData animData, sf::Vector2f position) :
    AoE(animData, position) {
        sprite.setScale({.9f,.9f});
    }

bool Freeze::Update(float deltaTime) {
    return AnimUtil::UpdateSpriteAnim(sprite, animData, deltaTime);
}