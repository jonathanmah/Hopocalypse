#include "fx/Explosion.h"
#include "core/GameState.h"

Explosion::Explosion(AnimData animData, sf::Vector2f position, ExplosionData explosionData) :
    AoE(animData, position), explosionData(explosionData) {
        sprite.setScale({3.f,3.f});
        float radians =  RandomUtil::GetRandomFloat(0,6.23f);
        sf::Angle angle = sf::radians(radians);
        sprite.setRotation(angle);
    }

void Explosion::DamageNeighbours(GameState& state) {
    for(auto& monster : state.monsters) {
        if((monster->GetPosition()-sprite.getPosition()).length() < explosionData.radius){
            monster->TakeDamage(explosionData.damage);
        }
    }
    isActive = false;
}

void Explosion::DrawHitbox(sf::RenderWindow& window) {
    HitboxDebugger::DrawCircle(window, sprite.getPosition(), explosionData.radius);
}

bool Explosion::Update(GameState& state, float deltaTime) {

    if(isActive && explosionData.delay <= 0.f) {
        DamageNeighbours(state);
    }
    explosionData.delay = std::max(0.f, explosionData.delay - deltaTime);

    return AnimUtil::UpdateSpriteXYAnim(sprite, animData, deltaTime);
}