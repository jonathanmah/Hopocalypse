#include "weapons/derived/Ump45.h"
#include <iostream>

static const ProjectileData ump45BulletReg = {
    AnimUtil::ProjectileAnim::medBulletReg,
    20.f, // speed
    5.f, // damage 
    1.f, // bullet scale
    1, // collateral count
};

static const ProjectileData ump45BulletUpgrade = {
    AnimUtil::ProjectileAnim::redLaser,
    40.f, // speed
    250.f, // damage 
    .2f, // bullet scale
    4, // collateral count
};

Ump45::Ump45() : 
Weapon(AnimUtil::WeaponAnim::ump45, 
        ump45BulletReg, 
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

void Ump45::CreateProjectile(Player& player, std::vector<std::unique_ptr<Projectile>>& projectiles) {
    sf::Vector2f adjustedNormal = (GetTargetWithSpread(mousePosGlobal) - GetPosition()).normalized();
    projectiles.emplace_back(std::make_unique<Projectile>(projectileData, muzzlePosition, adjustedNormal));
}

void Ump45::UpgradeWeapon() {
    sprite.setTextureRect(AnimUtil::WeaponAnim::ump45Upgraded);
    projectileData = ump45BulletUpgrade;
    //weaponData.spreadDeviationMax = 0.f;
    isUpgraded = true;
}