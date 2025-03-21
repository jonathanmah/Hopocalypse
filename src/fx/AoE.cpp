#include "fx/AoE.h"
#include "core/GameState.h"

    // REMEMBER TO USE SUB RECT FOR SETTING ORIGIN, TEXTURE FRAME, NOT SPRITE SIZE
AoE::AoE(AnimData animData, sf::Vector2f position) : sprite(*animData.texture), animData(animData), isActive(true) {
    sprite.setPosition(position);
    sprite.setOrigin({static_cast<float>(animData.textureFrame.size.x/2),static_cast<float>(animData.textureFrame.size.y/2)});
    sprite.setTextureRect(sf::IntRect(animData.textureFrame.position, animData.textureFrame.size));
}
void AoE::UpdateAoE(GameState& state, float deltaTime) { // explosion
    for(auto it = state.aoe.begin(); it != state.aoe.end();){
        // update aoe to see if it's done or not
        if((*it)->Update(deltaTime)){
            it = state.aoe.erase(it);
        } else {
            ++it;
        }
    }
}

void AoE::RenderAoE(GameState& state, bool drawHitbox) {    // shared 
    state.batchRenderer->BatchRenderSprites(state.aoe);
    if(drawHitbox) {
        for(auto& effect : state.aoe) {
            HitboxDebugger::DrawSpriteGlobalBoundsHitbox(state.window, effect->sprite);
        }
    }
}

// need to extend AOE and make derived implementation of Explosion, vs status effect like ice.
// can create different ones 