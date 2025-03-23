#include <iostream>
#include "weapons/derived/Famas.h"
#include "core/GameState.h"
#include "entities/effects/Slowed.h"

static const ProjectileData famasBulletReg = {
    AnimUtil::ProjectileAnim::medBulletReg,
        40.f, // speed
        100.f, // damage 
        1.f, // bullet scale
        1, // collateral count
};

static const ProjectileData famasBulletUpgrade = {
    AnimUtil::ProjectileAnim::iceBullet,
    15.f, // speed
    0.f, // damage 
    1.3f, // bullet scale
    1, // collateral count
};

Famas::Famas() : 
Weapon(AnimUtil::WeaponAnim::famas, 
        famasBulletReg, 
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
    ), burstFireCounter(0), attemptShotTimer(0.f)
{
    sprite.setOrigin({sprite.getLocalBounds().size.x / 3.6f, sprite.getLocalBounds().size.y / 2});

}
void Famas::CreateProjectile(Player& player, std::vector<std::unique_ptr<Projectile>>& projectiles) {
    sf::Vector2f adjustedNormal = (GetTargetWithSpread(mousePosGlobal) - GetPosition()).normalized();
    if(!isUpgraded){
        projectiles.emplace_back(std::make_unique<Projectile>(projectileData, muzzlePosition, adjustedNormal));
    } else {
        projectiles.emplace_back(std::make_unique<FamasIceBullet>(projectileData, muzzlePosition, adjustedNormal));
    }
}


void Famas::AttemptShoot(Player& player, std::vector<std::unique_ptr<Projectile>>& projectiles, float deltaTime) {
    if (burstFireCounter==1 && sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)){
        attemptShotTimer = 3*weaponData.fireRate;
    }
    // try to add bullet if time since last shot is less than 3xfirerate?
    if (weaponData.timeSinceShot > weaponData.fireRate) { // add bullet if time between shots is good
        weaponData.fireRate = 0.10f;
        this->CreateProjectile(player, projectiles);
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

void Famas::Update(GameState& state, Player& player, sf::Vector2f mousePosGlobal, float deltaTime) {
    attemptShotTimer = std::max(0.f, attemptShotTimer-deltaTime);
    UpdateMuzzleFlashes(deltaTime);
    UpdateShells(deltaTime); 
    SetMousePosGlobal(mousePosGlobal);
     UpdateBase(player.GetPosition(), deltaTime);
     if(attemptShotTimer > 0 || sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)) {
         AttemptShoot(player, state.projectiles, deltaTime);
     } else {
         DecreaseSpread();
     }
     weaponData.timeSinceShot += deltaTime;
 }

void Famas::UpgradeWeapon() {
    sprite.setTextureRect(AnimUtil::WeaponAnim::famasUpgraded);
    projectileData = famasBulletUpgrade;
    isUpgraded = true;
}


FamasIceBullet::FamasIceBullet(ProjectileData projectileData, sf::Vector2f position, sf::Vector2f normalized)
: Projectile(projectileData, position, normalized) {
}

void FamasIceBullet::UpdateProjectileStatus(Character& character, GameState& state, 
    std::vector<std::unique_ptr<Projectile>>::iterator& it) {
    
    Monster* monster = dynamic_cast<Monster*>(&character);
    monster->slowed.ApplySlow(3.f, .3f);
    it = state.projectiles.erase(it); 

}