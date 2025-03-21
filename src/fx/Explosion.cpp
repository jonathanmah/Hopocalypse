#include "fx/Explosion.h"
#include "core/GameState.h"

Explosion::Explosion(AnimData animData, sf::Vector2f position) :
    AoE(animData, position) {
        sprite.setScale({3.f,3.f});
        float radians =  RandomUtil::GetRandomFloat(0,6.23f);
        sf::Angle angle = sf::radians(radians);
        sprite.setRotation(angle);
    }

bool Explosion::Update(float deltaTime) {
    return AnimUtil::UpdateSpriteXYAnim(sprite, animData, deltaTime);
}