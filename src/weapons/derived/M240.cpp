#include "weapons/derived/M240.h"
#include "entities/Player.h"
#include <iostream>
#include "core/GameState.h"

static const ProjectileData m240BulletReg = {
    AnimUtil::ProjectileAnim::medBulletReg,
    20.f, // speed
    0.f, // damage 
    1.f, // bullet scale
    1, // collateral count
};

static const ProjectileData m240BulletUpgrade = {
    AnimUtil::ProjectileAnim::redLaser,
    40.f, // speed
    250.f, // damage 
    .2f, // bullet scale
    4, // collateral count
};

M240::M240(Player& player) : 
Weapon(AnimUtil::WeaponAnim::m240, 
        m240BulletReg, 
        {
            1.f, //scale 
            0.f, // baseOffsetX
            24.f, // baseOffsetY
            0.f, // x offset from muzzle
            -2.8f, // y offset from muzzle (pre recoil translation)
            48.f, // muzzleOffsetscalar
            0.1f, // fireRate
            .15f, // time since last bullet fired
            0.f, // current accumulated spread offset
            .15f, // spread offset max
            .01f, //  spread offset growth
            .015f, // spread offset decay
            .0f, // time left until gun returns back to original position after recoil
            .15f, // total time gun takes to return back
            5.f, // amount of offset, vector created using this scalar opposite point direction
            5.f, // shell offset x
            -5.f, // shell offset y
        }
    ), firingStreak(0.f), glowSprite(*AnimUtil::WeaponAnim::m240.texture)
{
    sprite.setOrigin({sprite.getLocalBounds().size.x / 3, sprite.getLocalBounds().size.y / 2});
}

void M240::CreateProjectile(std::vector<std::unique_ptr<Projectile>>& projectiles) {
    sf::Vector2f adjustedNormal = (GetTargetWithSpread(mousePosGlobal) - GetPosition()).normalized();
    projectiles.emplace_back(std::make_unique<Projectile>(projectileData, muzzlePosition, adjustedNormal));
}

void M240::UpgradeWeapon() {
    sprite.setTextureRect(AnimUtil::WeaponAnim::m240Upgraded);
    projectileData = m240BulletUpgrade;
    isUpgraded = true;
}

void M240::ActivateFiringEffects() {
    if(firingStreak < 2.f) {
        projectileData = m240BulletReg;
    } else if(firingStreak >= 2.f && firingStreak < 4.f){ // yellow 
        projectileData.anim = AnimUtil::ProjectileAnim::yellowLaser;
        projectileData.damage += 5;
        projectileData.scale = .2f;
    } else if (firingStreak >= 4.f && firingStreak < 6.f){ // orange
        projectileData.anim = AnimUtil::ProjectileAnim::orangeLaser;
        projectileData.damage += 5;
        projectileData.scale = .2f;
    } else if (firingStreak >= 6.f && firingStreak < 8.f){ // red 
        projectileData.anim = AnimUtil::ProjectileAnim::redLaser;
        projectileData.damage += 5;
        projectileData.scale = .35f;
    } else { // white
        projectileData.anim = AnimUtil::ProjectileAnim::whiteLaser;
        projectileData.damage += 5;
        projectileData.collateralCount = 10;
        projectileData.scale = .5f;
    }


}

void M240::Draw(sf::RenderWindow& window, BatchRenderer& batchRenderer){
    // if(firingStreak > 1.f){
    //     window.draw(glowSprite, sf::BlendAdd);
    // }
    window.draw(sprite);
    DrawMuzzleFlashes(window);
    DrawShells(window);
}

void M240::Update(GameState& state, Player& player, sf::Vector2f mousePosGlobal, float deltaTime) {
    UpdateMuzzleFlashes(deltaTime); // update effects
    UpdateShells(deltaTime);
    SetMousePosGlobal(mousePosGlobal);
 
     UpdateBase(player.GetPosition(), deltaTime);
     if(sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)) {
        if(isUpgraded){
            ActivateFiringEffects();
            firingStreak += deltaTime;
        }
        AttemptShoot(state.projectiles, deltaTime);
     } else {
        DecreaseSpread();
        firingStreak = std::max(0.f, firingStreak - (deltaTime*10));
     }
     weaponData.timeSinceShot += deltaTime;
 }