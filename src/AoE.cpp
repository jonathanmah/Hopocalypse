#include "AoE.h"
#include "GameState.h"

    // FUCK THE BUG WITH SPRITE TEXTURE ORIGINS. USE THE FUCKING SUB RECT 
AoE::AoE(AnimData animData, sf::Vector2f position) : sprite(*animData.texture), animData(animData) {
    sprite.setPosition(position);
    sprite.setOrigin({static_cast<float>(animData.textureFrame.size.x/2),static_cast<float>(animData.textureFrame.size.y/2)});
    sprite.setTextureRect(sf::IntRect(animData.textureFrame.position, animData.textureFrame.size));
    sprite.setScale({3.f,3.f});
    float radians =  RandomUtil::GetRandomFloat(0,6.23f);
    sf::Angle angle = sf::radians(radians);
    sprite.setRotation(angle);
}
//256w x 128h
void AoE::UpdateAoE(GameState& state, float deltaTime) {
    for(auto it = state.aoe.begin(); it != state.aoe.end();){
        if(AnimUtil::UpdateSpriteXYAnim((*it)->GetSprite(), (*it)->animData, deltaTime)){
            it = state.aoe.erase(it);
        } else {
            ++it;
        }
    }
}

void AoE::RenderAoE(GameState& state, bool drawHitbox) {    
    state.batchRenderer->BatchRenderSprites(state.aoe);
    if(drawHitbox) {
        for(auto& effect : state.aoe) {
            HitboxDebugger::DrawSpriteGlobalBoundsHitbox(state.window, effect->sprite);
        }
    }
}