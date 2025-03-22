#include <iostream>
#include "weapons/derived/M1014.h"
#include "core/GameState.h"


static constexpr float PELLET_SPREAD_SCALAR = .015f;
static constexpr float UPGRADE_SPREAD_SCALAR = .02f;

static const ProjectileData m1014BulletReg = {
    AnimUtil::ProjectileAnim::shotgunPelletReg,
    20.f, // speed
    20.f, // damage 
    1.f, // bullet scale
    1, // collateral count
    1.f, // acceleration
    0.60f, // lifetime
};

static const ProjectileData m1014BulletUpgrade = {
    AnimUtil::ProjectileAnim::lightningBullet,
    20.f, // speed
    40.f, // damage 
    .6f, // bullet scale
    4, // collateral count
    1.f, // acceleration
    .60f // lifetime
};

M1014::M1014() : 
Weapon(AnimUtil::WeaponAnim::m1014, 
        m1014BulletReg, 
        {
            1.f, //scale 
            0.f, // baseOffsetX
            24.f, // baseOffsetY
            0.f, // x offset from muzzle
            -2.8f, // y offset from muzzle (pre recoil translation)
            48.f, // muzzleOffsetscalar
            0.6f, // fireRate
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

void M1014::CreateProjectile(std::vector<std::unique_ptr<Projectile>>& projectiles) {
    // just create a spread of normals and pushback multiple shells? easy enough
    
    // create a steady offset perpendicular up and down from muzzle
    //projectiles.emplace_back(std::make_unique<Projectile>(projectileData, muzzlePosition, relative.normalized()));
    sf::Vector2f perpendicularUp = (relative.x >= 0) ? sf::Vector2f(relative.y, -relative.x) : sf::Vector2f(-relative.y, relative.x);
    sf::Vector2f spreadOffset = !isUpgraded ? perpendicularUp*PELLET_SPREAD_SCALAR : perpendicularUp*UPGRADE_SPREAD_SCALAR;
    if(!isUpgraded){
        for(int i = -4; i < 5; i++) {
            projectiles.emplace_back(std::make_unique<Projectile>(projectileData, muzzlePosition, (relative+(spreadOffset*static_cast<float>(i))).normalized()));
        }
    } else {
        for(int i = -6; i < 7; i++) {
            projectiles.emplace_back(std::make_unique<M1014ShellUpgraded>(projectileData, muzzlePosition, (relative+(spreadOffset*static_cast<float>(i))).normalized()));
        }
    }
}

void M1014::UpgradeWeapon() {
    sprite.setTextureRect(AnimUtil::WeaponAnim::m1014Upgraded);
    projectileData = m1014BulletUpgrade;
    weaponData.fireRate = 0.45f;
    weaponData.spreadDeviationMax = 0.f;
    isUpgraded = true;
}



M1014ShellUpgraded::M1014ShellUpgraded(ProjectileData projectileData, sf::Vector2f position, sf::Vector2f normalized)
: Projectile(projectileData, position, normalized) {
}

void M1014ShellUpgraded::UpdateProjectileStatus(Character& character, GameState& state, 
    std::vector<std::unique_ptr<Projectile>>::iterator& it) {
    
    it = state.projectiles.erase(it); 
}
