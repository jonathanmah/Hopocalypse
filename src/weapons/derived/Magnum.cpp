#include <iostream>
#include "weapons/derived/Magnum.h"
#include "core/GameState.h"


static constexpr float KNOCKBACK_SCALAR = 100.f;


static const ProjectileData magnumBulletReg = {
    AnimUtil::ProjectileAnim::smallBulletReg,
    20.f, // speed
    1.f, // damage 
    1.f, // bullet scale
    2, // collateral count
    1.f, // acceleration
    3.f, // lifetime
};

static const ProjectileData magnumBulletUpgrade = {
    AnimUtil::ProjectileAnim::waveBullet,
    15.f, // speed
    20.f, // damage 
    .2f, // bullet scale
    1000, // collateral count
    1.f, // acceleration
    2.0f, // lifetime
};

Magnum::Magnum() : 
Weapon(AnimUtil::WeaponAnim::magnum, 
        magnumBulletReg, 
        {
            1.f, //scale 
            0.f, // baseOffsetX
            22.f, // baseOffsetY
            0.f, // x offset from muzzle
            -6.5f, // y offset from muzzle (pre recoil translation)
            30.f, // muzzleOffsetscalar
            0.5f, // fireRate
            .15f, // time since last bullet fired
            0.f, // current accumulated spread offset
            .0f, // spread offset max
            .01f, //  spread offset growth
            .015f, // spread offset decay
            .0f, // time left until gun returns back to original position after recoil
            .15f, // total time gun takes to return back
            .5f, // amount of offset, vector created using this scalar opposite point direction
            5.f, // shell offset x
            -5.f, // shell offset y
        }
    )
{
    sprite.setOrigin({sprite.getLocalBounds().size.x / 5, sprite.getLocalBounds().size.y*0.7f});
}

void Magnum::CreateProjectile(Player& player, std::vector<std::unique_ptr<Projectile>>& projectiles) {
    sf::Vector2f adjustedNormal = (GetTargetWithSpread(mousePosGlobal) - GetPosition()).normalized();
    if(!isUpgraded){
        projectiles.emplace_back(std::make_unique<Projectile>(projectileData, muzzlePosition, adjustedNormal));
    } else {
        projectiles.emplace_back(std::make_unique<MagnumWaveBullet>(projectileData, muzzlePosition, adjustedNormal));
    }
    
}

void Magnum::UpgradeWeapon() {
    sprite.setTextureRect(AnimUtil::WeaponAnim::magnumUpgraded);
    projectileData = magnumBulletUpgrade;
    isUpgraded = true;
}

//307x410 288,783 // total 102x each 102.5y
MagnumWaveBullet::MagnumWaveBullet(ProjectileData projectileData, sf::Vector2f position, sf::Vector2f normalized)
: Projectile(projectileData, position, normalized) {
    createsBlood = false;
}

void MagnumWaveBullet::UpdateAnimation(float deltaTime) {
    AnimUtil::UpdateSpriteXYAnim(sprite, animData, deltaTime);
    if(sprite.getScale().x < 2.5f){
        sprite.setScale(sprite.getScale()*1.1f);
    }

}

void MagnumWaveBullet::UpdateProjectileStatus(Character& character, GameState& state, 
    std::vector<std::unique_ptr<Projectile>>::iterator& it) {
    
    if(GetCollateralCount() > 1) {
        if(HasHit(character.id)){
            ++it;
            return;
        }
        hitCharacters.insert(character.id);

        //character.knockbackDebt = 400.f;
        //character.knockbackVector = (*it)->velocity;
        Monster* monster = dynamic_cast<Monster*>(&character);
        monster->knockback.SetKnockback(400.f, (*it)->velocity);
        ++it;
    } else {
        //#TODO need to somehow tell rocket animation to explode when this happens here for RPG
        it = state.projectiles.erase(it);
    }
}