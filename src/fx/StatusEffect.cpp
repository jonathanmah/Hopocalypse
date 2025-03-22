#include "fx/StatusEffect.h"
#include "core/GameState.h"
#include "entities/Character.h"

    // REMEMBER TO USE SUB RECT FOR SETTING ORIGIN, TEXTURE FRAME, NOT SPRITE SIZE
StatusEffect::StatusEffect(AnimData animData, sf::Vector2f position, Character& character, float timeActive) : sprite(*animData.texture), animData(animData), character(character), timeActive(timeActive) {
    sprite.setPosition(position);
    sprite.setOrigin({static_cast<float>(animData.textureFrame.size.x/2),static_cast<float>(animData.textureFrame.size.y/2)});
    sprite.setTextureRect(sf::IntRect(animData.textureFrame.position, animData.textureFrame.size));
}
void StatusEffect::UpdateStatusEffect(GameState& state, float deltaTime) { // explosion
    for(auto it = state.statusEffects.begin(); it != state.statusEffects.end();){
        // update StatusEffect to see if it's done or not
        (*it)->Update(deltaTime);
        if((*it)->timeActive <= 0.f){
            std::cout << "erasing" << std::endl;
            it = state.statusEffects.erase(it);
        } else {
            ++it;
        }
    }
}

void StatusEffect::RenderStatusEffects(GameState& state, bool drawHitbox) {    // shared 
    state.batchRenderer->BatchRenderSprites(state.statusEffects);
    if(drawHitbox) {
        for(auto& effect : state.statusEffects) {
            HitboxDebugger::DrawSpriteGlobalBoundsHitbox(state.window, effect->GetSprite());
        }
    }
}

// need to extend StatusEffect and make derived implementation of Explosion, vs status effect like ice.
// can create different ones 