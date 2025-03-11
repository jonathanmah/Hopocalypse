#include "Rpg.h"

Rpg::Rpg() : 
Weapon(AnimUtil::WeaponAnim::rpgAnim, 
        {
            AnimUtil::WeaponAnim::rpgRocketAnim,
            8.f, // speed
            150.f, // damage 
            1.3f, // bullet scale
            5, // collateral count
        }, 
        {
            1.3f, //scale 
            0.f, // baseOffsetX
            3.f, // baseOffsetY
            0.f, // x offset from muzzle
            7.5f, // y offset from muzzle
            50.f, // muzzleOffsetscalar
            1.8f, // fireRate
            1.75f, // time since last bullet fired
            0.f, // current accumulated spread offset
            .0f, // spread offset max
            .0f, //  spread offset growth
            .0f, // spread offset decay
            0.f, // time left until gun returns back to original position after recoil
            .3f, // total time gun takes to return back
            15.f, // amount of offset, vector created using this scalar opposite point direction

        }
    )
{
    sprite.setOrigin({sprite.getLocalBounds().size.x / 2.4f, sprite.getLocalBounds().size.y / 2});
}

void Rpg::CreateProjectile(std::vector<std::unique_ptr<Projectile>>& projectiles) {
    sf::Vector2f baseOriginToTarget = (mousePosGlobal - GetPosition()).normalized();
    projectiles.emplace_back(std::make_unique<RPGrocket>(RPGrocket(GetProjectileData(), muzzlePosition,baseOriginToTarget)));
}

// set new sub rectangle to display texture without rocket loaded
void Rpg::UpdateBase(sf::Vector2f characterPosition, float deltaTime) {
    UpdateBaseTransformations(characterPosition, deltaTime);
    weaponData.timeSinceShot += deltaTime;
    if(weaponData.timeSinceShot < weaponData.fireRate) {
        sprite.setTextureRect(AnimUtil::WeaponAnim::rpgReloadRect);
    } else {
        sprite.setTextureRect(AnimUtil::WeaponAnim::rpgLoadedRect);
    }
}