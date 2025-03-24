#include "weapons/derived/M32.h"
#include "weapons/Grenade.h"
#include <iostream>

static const ProjectileData m32GrenadeReg = {
    AnimUtil::ProjectileAnim::m32GrenadeReg,
    30.f, // speed
    0.f, // damage 
    1.3f, // bullet scale
    1000, // collateral count
};

static const ProjectileData m32GrenadeUpgrade = {
    AnimUtil::ProjectileAnim::m32GrenadeUpgrade,
    30.f, // speed
    0.f, // damage 
    1.3f, // bullet scale
    1000, // collateral count
};

static const ExplosionData m32RegExplosion = {
    100, // damage
    150.f, // radius
    .1f, // delay
    2.f // scale
};

static const ExplosionData m32UpgradedExplosion = {
    200,
    175.f,
    .1f,
    2.f,
    true
};

M32::M32() : 
Weapon(AnimUtil::WeaponAnim::m32, 
        m32GrenadeReg, 
        {
            1.f, //scale 
            0.f, // baseOffsetX
            24.f, // baseOffsetY
            0.f, // x offset from muzzle
            -2.8f, // y offset from muzzle (pre recoil translation)
            40.f, // muzzleOffsetscalar
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

void M32::AttemptShoot(Player& player, std::vector<std::unique_ptr<Projectile>>& projectiles, float deltaTime) {
    if (weaponData.timeSinceShot > weaponData.fireRate) {
        CreateProjectile(player, projectiles);
        weaponData.timeSinceShot = 0.f;
        SetPositionPostRecoil();
    }
}

void M32::CreateProjectile(Player& player, std::vector<std::unique_ptr<Projectile>>& projectiles) {
    sf::Vector2f adjustedNormal = (mousePosGlobal - GetPosition()).normalized();
    if(!isUpgraded){
        projectiles.emplace_back(std::make_unique<Grenade>(projectileData, muzzlePosition, adjustedNormal, mousePosGlobal, AnimUtil::WeaponFxAnim::explosion, m32RegExplosion));
    } else {
        projectiles.emplace_back(std::make_unique<Grenade>(projectileData, muzzlePosition, adjustedNormal, mousePosGlobal, AnimUtil::WeaponFxAnim::energyExplosion, m32UpgradedExplosion));
    }
    
}

// create a grenade projectile... 
// has a few things. 0 damage, unlimited collateral, source pos, target pos
// an effect when it lands

void M32::UpgradeWeapon() {
    sprite.setTextureRect(AnimUtil::WeaponAnim::m32Upgraded);
    projectileData = m32GrenadeUpgrade;
    isUpgraded = true;
}