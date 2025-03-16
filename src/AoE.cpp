#include "AoE.h"
    // FUCK THE BUG WITH SPRITE TEXTURE ORIGINS. USE THE FUCKING SUB RECT 
AoE::AoE(AnimData animData, sf::Vector2f position) : sprite(*animData.texture), animData(animData) {
    sprite.setPosition(position);
    sprite.setOrigin({static_cast<float>(animData.textureFrame.size.x/2),static_cast<float>(animData.textureFrame.size.y/2)});
    sprite.setTextureRect(sf::IntRect(animData.textureFrame.position, animData.textureFrame.size));
    sprite.setScale({3.f,3.f});
}

void AoE::UpdateAoE(std::vector<std::unique_ptr<AoE>>& aoe, float deltaTime) {
    for(auto it = aoe.begin(); it != aoe.end();){
        if(AnimUtil::UpdateSpriteXYAnim((*it)->GetSprite(), (*it)->animData, deltaTime)){
            it = aoe.erase(it);
        } else {
            ++it;
        }
    }
}

void AoE::RenderAoE(std::vector<std::unique_ptr<AoE>>& aoe, BatchRenderer& batchRenderer, sf::RenderWindow& window, bool drawHitbox) {    
    batchRenderer.BatchRenderSprites(aoe);
    if(drawHitbox) {
        for(auto& effect : aoe) {
            HitboxDebugger::DrawSpriteGlobalBoundsHitbox(window, effect->sprite);
        }
    }
}