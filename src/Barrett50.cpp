#include "Barrett50.h"

static const ProjectileData barrett50BulletReg = {
    AnimUtil::ProjectileAnim::barrett50BulletReg,
    65.f, // speed
    150.f, // damage 
    1.5f, // bullet scale
    5, // collateral count
};

static const ProjectileData barrett50BulletUpgrade = {
    AnimUtil::ProjectileAnim::barrett50BulletUpgrade,
    65.f, // speed
    400.f, // damage 
    .4f, // bullet scale
    100, // collateral count
};

Barrett50::Barrett50() : 
Weapon(AnimUtil::WeaponAnim::barrett50, 
    barrett50BulletUpgrade,
        {
            .95f, //scale 
            0.f, // baseOffsetX
            22.f, // baseOffsetY
            0.f, // x offset from muzzle
            -4.f, // y offset from muzzle
            67.f, // muzzleOffsetscalar
            .9f, // fireRate
            .9f, // time since last bullet fired
            0.f, // current accumulated spread offset
            .0f, // spread offset max
            .0f, //  spread offset growth
            .0f, // spread offset decay
            0.f, // time left until gun returns back to original position after recoil
            .9f, // total time gun takes to return back
            12.f, // amount of offset, vector created using this scalar opposite point direction
            
        }
    )
{
    sprite.setOrigin({sprite.getLocalBounds().size.x / 3.2f, sprite.getLocalBounds().size.y / 2});
}

void Barrett50::CreateProjectile(std::vector<std::unique_ptr<Projectile>>& projectiles) {
    sf::Vector2f adjustedNormal = (GetTargetWithSpread(mousePosGlobal) - GetPosition()).normalized();
    projectiles.emplace_back(std::make_unique<Projectile>(projectileData, muzzlePosition, adjustedNormal));
}