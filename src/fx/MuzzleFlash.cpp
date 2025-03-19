#include "fx/MuzzleFlash.h"
#include "util/RandomUtil.h"
#include <iostream>
static constexpr float PI = 3.141592;
static constexpr float MIRROR_NEG_Y_AXIS_BOUND = PI/2.f; // pi/2 radians , greater than half of pi
static constexpr float MIRROR_POS_Y_AXIS_BOUND =  -PI/2.f; // -pi/2 pi radians , less than -half of pi

MuzzleFlash::MuzzleFlash(AnimData animData, sf::Vector2f muzzlePosition, sf::Vector2f weaponBaseNormalized, float scale, bool isSecond) 
: animData(animData), 
sprite(*animData.texture),
 muzzlePosition(muzzlePosition), 
 weaponBaseNormalized(weaponBaseNormalized), 
 scale(scale),
 isSecond(isSecond){

    sprite.setTextureRect(sf::IntRect(animData.textureFrame.position, animData.textureFrame.size));
    InitOrigin();
    sprite.setPosition(muzzlePosition);
    sprite.setScale({scale, scale});
    RotateMuzzleFlash();
}

void MuzzleFlash::InitOrigin() {
    sf::FloatRect bounds = sprite.getLocalBounds();
    sprite.setOrigin({bounds.size.x/4,bounds.size.y/2});
}

AnimData MuzzleFlash::GetNextFlash() {
    static const AnimData flashAnimations[5] = {
        AnimUtil::WeaponFxAnim::muzzleFlash1, 
        AnimUtil::WeaponFxAnim::muzzleFlash2, 
        AnimUtil::WeaponFxAnim::muzzleFlash3, 
        AnimUtil::WeaponFxAnim::muzzleFlash4, 
        AnimUtil::WeaponFxAnim::muzzleFlash5, 
    };
    return flashAnimations[RandomUtil::GetRandomInt(0,4)];
}

void MuzzleFlash::RotateMuzzleFlash() {
    float radians = atan2(weaponBaseNormalized.y, weaponBaseNormalized.x);
    if(radians > MIRROR_NEG_Y_AXIS_BOUND || radians < MIRROR_POS_Y_AXIS_BOUND)
        sprite.setScale({scale, -scale}); // inverse y axis
    else {
        sprite.setScale({scale, scale});
    }
    sf::Angle angle = sf::radians(radians);
    sprite.setRotation(angle);
}

bool MuzzleFlash::Update(sf::Vector2f newMuzzlePosition, sf::Vector2f newNormalized, float deltaTime) {
    muzzlePosition = newMuzzlePosition;
    weaponBaseNormalized = newNormalized;
    sprite.setPosition(muzzlePosition);
    RotateMuzzleFlash();
    return AnimUtil::UpdateSpriteAnim(sprite, animData, deltaTime);
}



void MuzzleFlash::Draw(sf::RenderWindow& window) {
    window.draw(sprite);
    // HitboxDebugger::DrawSpriteGlobalBoundsHitbox(window, sprite);
    // HitboxDebugger::DrawSpriteLocalBoundsHitbox(window, sprite);
    // HitboxDebugger::DrawSpriteOrigin(window, sprite);
}

