#include "entities/effects/OnFire.h"
#include "util/AnimUtil.h"
#include "entities/Character.h"
#include <cmath>

OnFire::OnFire(Character& character)
: StatusEffect(character),
    sprite(*AnimUtil::onFireTexture),
    animData(AnimUtil::StatusFxAnim::onFire),
    dps(0),
    damageTimer(0.f),
    flamethrowerDmgCd(0.f),
    updatedColour(false)
{
    sprite.setPosition(character.GetPosition());
    sprite.setOrigin({animData.textureFrame.size.x*.5f,animData.textureFrame.size.y*.5f});
    sprite.setTextureRect(sf::IntRect(animData.textureFrame.position, animData.textureFrame.size));
    sprite.setScale({2.6f,2.4f});
    sf::Angle angle = sf::radians(M_PI/2);
    sprite.setRotation(angle);
}

void OnFire::UpdateColour() {
    if(!updatedColour&&character.health <= 0.f){
        auto updateColour = character.GetSprite().getColor();
        updateColour.r = updateColour.r * 0.15f;
        updateColour.g = updateColour.g * 0.15f;
        updateColour.b = updateColour.b * 0.15f;
        character.GetSprite().setColor(updateColour);
        
        updatedColour = true;
    }
}

bool OnFire::UpdateStatusEffect(float deltaTime) {
    sprite.setPosition(character.GetPosition());
    UpdateColour();
    
    if(UpdateTimeLeft(deltaTime)) { // returns true if timer is empty and no effect on
        // for other statuses, can reset to defaults here after effect ends
        return true;
    }
    if (flamethrowerDmgCd > 0.f) {
        flamethrowerDmgCd -= deltaTime;
    }
    if(damageTimer >= .25f) {
        character.TakeDamage(20);
        damageTimer = 0.f;
    }
    damageTimer += deltaTime;
    AnimUtil::UpdateSpriteXYAnim(sprite, animData, deltaTime);
    return false;
}