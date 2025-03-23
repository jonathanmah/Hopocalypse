#include "fx/FootprintManager.h"
#include "core/GameState.h"

static constexpr float FOOT_COLLIDER_X_OFFSET = 12.f;

FootprintManager::FootprintManager() :
    createLeftFootNext(true),
    footprintDecayTimer(0.f),
    footprintDtSumFrame(0.f)
    {}


sf::FloatRect FootprintManager::GetFootCollider(Character* character) {
    sf::FloatRect origBounds = character->GetGlobalBounds();
    float posX = origBounds.position.x + FOOT_COLLIDER_X_OFFSET;
    float posY = origBounds.position.y + origBounds.size.y*0.8f;
    float width = origBounds.size.x - (FOOT_COLLIDER_X_OFFSET*2);
    float height = origBounds.size.y*0.2f;
    return sf::FloatRect{{posX, posY}, {width, height}};
}

void FootprintManager::UpdateFootprints(Character* character, sf::Vector2f nextMoveNormalized, GameState& state, float deltaTime) {
    bool groundBloodCollision = GroundBlood::HasGroundBloodCollision(GetFootCollider(character),state.groundBlood);
    if((groundBloodCollision || footprintDecayTimer > 0.01f) && footprintDtSumFrame >= FOOTPRINT_DT_RATE){
        AnimData footprintData;
        if(createLeftFootNext){
            footprintData = AnimUtil::BloodAnim::leftFootprint;
            createLeftFootNext = false;
        } else {
            footprintData = AnimUtil::BloodAnim::rightFootprint;
            createLeftFootNext = true;
        }
        if (groundBloodCollision){
            footprintDecayTimer =  FOOTPRINT_DECAY_TIME; // refresh footprint timer after stepping in ground blood
        }
        state.footprints.push_back(Footprint{footprintData, character->GetGlobalBounds(), nextMoveNormalized, 
            !createLeftFootNext, footprintDecayTimer});
        
        footprintDtSumFrame = 0.f; // reset time for next frame
    }
    footprintDtSumFrame += deltaTime;
    if(footprintDecayTimer > 0){
        footprintDecayTimer -= deltaTime;
    }
}
