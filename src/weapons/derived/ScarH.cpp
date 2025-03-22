#include <iostream>
#include "weapons/derived/ScarH.h"
#include "fx/Paralysis.h"
#include "core/GameState.h"

static const ProjectileData scarHBulletReg = {
    AnimUtil::ProjectileAnim::medBulletReg,
    20.f, // speed
    1.f, // damage 
    1.f, // bullet scale
    1, // collateral count
};

static const ProjectileData scarHBulletUpgrade = {
    AnimUtil::ProjectileAnim::electricBullet,
    20.f, // speed
    0.f, // damage 
    .3f, // bullet scale
    4, // collateral count
};

ScarH::ScarH() : 
Weapon(AnimUtil::WeaponAnim::scarH, 
        scarHBulletReg, 
        {
            1.f, //scale 
            0.f, // baseOffsetX
            24.f, // baseOffsetY
            0.f, // x offset from muzzle
            -2.8f, // y offset from muzzle (pre recoil translation)
            48.f, // muzzleOffsetscalar
            0.15f, // fireRate
            .15f, // time since last bullet fired
            0.f, // current accumulated spread offset
            .12f, // spread offset max
            .01f, //  spread offset growth
            .015f, // spread offset decay
            .0f, // time left until gun returns back to original position after recoil
            .15f, // total time gun takes to return back
            5.f, // amount of offset, vector created using this scalar opposite point direction
            5.f, // shell offset x
            -5.f, // shell offset y
        }
    )
{
    sprite.setOrigin({sprite.getLocalBounds().size.x / 3, sprite.getLocalBounds().size.y / 2});
}

void ScarH::CreateProjectile(std::vector<std::unique_ptr<Projectile>>& projectiles) {
    sf::Vector2f adjustedNormal = (GetTargetWithSpread(mousePosGlobal) - GetPosition()).normalized();
    if(!isUpgraded){
        projectiles.emplace_back(std::make_unique<Projectile>(projectileData, muzzlePosition, adjustedNormal));
    } else {
        projectiles.emplace_back(std::make_unique<ScarElectricBullet>(projectileData, muzzlePosition, adjustedNormal));
    }
}

void ScarH::UpgradeWeapon() {
    sprite.setTextureRect(AnimUtil::WeaponAnim::scarHUpgraded);
    projectileData = scarHBulletUpgrade;
    isUpgraded = true;
}

ScarElectricBullet::ScarElectricBullet(ProjectileData projectileData, sf::Vector2f position, sf::Vector2f normalized)
: Projectile(projectileData, position, normalized) {
}

void ScarElectricBullet::UpdateAnimation(float deltaTime) {
    AnimUtil::UpdateSpriteAnim(sprite, animData, deltaTime);
}

void ScarElectricBullet::UpdateProjectileStatus(Character& character, GameState& state, 
    std::vector<std::unique_ptr<Projectile>>::iterator& it){
    
    sf::Vector2f position = character.GetPosition();
    //if(!character.frozen){
    state.statusEffects.emplace_back(std::make_unique<Paralysis>(AnimUtil::StatusFxAnim::paralyze, position, character));
      //  character.frozen = true;
    //}
    it = state.projectiles.erase(it);

}