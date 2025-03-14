#include "Famas.h"
#include <iostream>

Famas::Famas() : 
Weapon(AnimUtil::WeaponAnim::famasAnim, 
        {
            AnimUtil::ProjectileAnim::RegularBullet::anim,
            40.f, // speed
            100.f, // damage 
            1.f, // bullet scale
            1, // collateral count
        }, 
        {
            .95f, //scale 
            0.f, // baseOffsetX
            27.f, // baseOffsetY
            0.f, // x offset from muzzle
            -1.8f, // y offset from muzzle
            45.f, // muzzleOffsetscalar
            0.06f, // fireRate
            .06f, // time since last bullet fired
            0.f, // current accumulated spread offset
            .05f, // spread offset max
            .01f, //  spread offset growth
            .015f, // spread offset decay
            .0f, // time left until gun returns back to original position after recoil
            .10f, // total time gun takes to return back
            4.f, // amount of offset, vector created using this scalar opposite point direction
        }
    ), burstFireCounter(0)
{
    sprite.setOrigin({sprite.getLocalBounds().size.x / 3.6f, sprite.getLocalBounds().size.y / 2});
}
void Famas::CreateProjectile(std::vector<std::unique_ptr<Projectile>>& projectiles){
    sf::Vector2f adjustedNormal = (GetTargetWithSpread(mousePosGlobal) - GetPosition()).normalized();
    projectiles.emplace_back(std::make_unique<Projectile>(projectileData, muzzlePosition, adjustedNormal));
}

void Famas::AttemptShoot(std::vector<std::unique_ptr<Projectile>>& projectiles, float deltaTime) {
    if (weaponData.timeSinceShot > weaponData.fireRate) {
        weaponData.fireRate = 0.10f;
        this->CreateProjectile(projectiles);
        IncreaseSpread();
        weaponData.timeSinceShot = 0.f;
        SetPositionPostRecoil();
        AddMuzzleFlashEffect();
        if(++burstFireCounter >=3) {
            weaponData.fireRate =  0.3f;
            burstFireCounter = 0;
        }
    }
}