#include "entities/effects/Paralyzed.h"
#include "util/AnimUtil.h"
#include "entities/Character.h"
#include <cmath>

Paralyzed::Paralyzed(Character& character)
: StatusEffect(character),
    sprite(*AnimUtil::statusEffectsTexture),
    animData(AnimUtil::StatusFxAnim::paralyze),
    disabledCd(0.f)
{
    sprite.setPosition(character.GetPosition());
    sprite.setOrigin({animData.textureFrame.size.x*.5f,animData.textureFrame.size.y*.5f});
    sprite.setTextureRect(sf::IntRect(animData.textureFrame.position, animData.textureFrame.size));
    //sprite.setScale({2.6f,2.4f});
    sf::Angle angle = sf::radians(M_PI/2);
    sprite.setRotation(angle);
}

bool Paralyzed::UpdateStatusEffect(float deltaTime) {
    sprite.setPosition(character.GetPosition());
    
    if(UpdateTimeLeft(deltaTime)) { // returns true if timer is empty and no effect on
        // for other statuses, can reset to defaults here after effect ends
        disabledCd = 3.f;
        return true;
    }
    AnimUtil::UpdateSpriteXYAnim(sprite, animData, deltaTime);
    return false;
}

void Paralyzed::AttemptApplyEffect(float duration) {
    if(!IsActive() && !OnCooldown()) {
        ApplyEffect(duration);
    }
}

void Paralyzed::UpdateDisabledCd(float deltaTime) {
    disabledCd = std::max(0.f, disabledCd-deltaTime);
}