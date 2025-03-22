#include <iostream>
#include "weapons/derived/P90.h"
#include "core/GameState.h"
#include "entities/Character.h"


static const ProjectileData p90BulletReg = {
    AnimUtil::ProjectileAnim::medBulletReg,
    20.f, // speed
    1.f, // damage 
    1.f, // bullet scale
    1, // collateral count
};

static const ProjectileData p90BulletUpgrade = {
    AnimUtil::ProjectileAnim::redLaser,
    40.f, // speed
    0.f, // damage 
    .2f, // bullet scale
    4, // collateral count
};

P90::P90() : 
Weapon(AnimUtil::WeaponAnim::p90, 
        p90BulletReg, 
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

void P90::CreateProjectile(Player& player, std::vector<std::unique_ptr<Projectile>>& projectiles) {
    sf::Vector2f adjustedNormal = (GetTargetWithSpread(mousePosGlobal) - GetPosition()).normalized();
    if(!isUpgraded){
        projectiles.emplace_back(std::make_unique<Projectile>(projectileData, muzzlePosition, adjustedNormal));
    } else {
        projectiles.emplace_back(std::make_unique<P90HealBullet>(projectileData, muzzlePosition, adjustedNormal, player));
    }
}

void P90::UpgradeWeapon() {
    sprite.setTextureRect(AnimUtil::WeaponAnim::p90Upgraded);
    projectileData = p90BulletUpgrade;
    std::cout << "UPGRADED P90" << std::endl;
    //weaponData.spreadDeviationMax = 0.f;
    isUpgraded = true;
}

P90HealBullet::P90HealBullet(ProjectileData projectileData, sf::Vector2f position, sf::Vector2f normalized, Character& source)
: Projectile(projectileData, position, normalized), source(source) {
}

void P90HealBullet::UpdateProjectileStatus(Character& character, GameState& state,
    std::vector<std::unique_ptr<Projectile>>::iterator& it) {
        std::cout << "HEAL BULLET CONTACT" << std::endl;
        source.health+=1;
        it = state.projectiles.erase(it); 
    }