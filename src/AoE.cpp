#include "AoE.h"

AoE::AoE(AnimData animData, sf::Vector2f position) : sprite(*animData.texture), animData(animData) {
    sprite.setPosition(position);
}

void AoE::UpdateAnimation(float deltaTime) {
    AnimUtil::UpdateSpriteXYAnim(sprite, animData, deltaTime);
}