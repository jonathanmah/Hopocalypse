#include "Barrett50.h"

Barrett50::Barrett50() : 
Weapon(AnimUtil::WeaponAnim::barrett50Anim, 
        {
            AnimUtil::ProjectileAnim::RegularBullet::anim,
            70.f, // speed
            150.f, // damage 
            2.5f, // bullet scale
            5, // collateral count
        }, 
        {
            .95f, //scale 
            0.f, // baseOffsetX
            16.f, // baseOffsetY
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