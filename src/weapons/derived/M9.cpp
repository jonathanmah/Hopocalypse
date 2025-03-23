#include <iostream>
#include "weapons/derived/M9.h"
#include "core/GameState.h"


static const ProjectileData m9BulletReg = {
    AnimUtil::ProjectileAnim::smallBulletReg,
    20.f, // speed
    1.f, // damage 
    1.f, // bullet scale
    1, // collateral count
};

static const ProjectileData m9BulletUpgrade = {
    AnimUtil::ProjectileAnim::atomBullet,
    20.f, // speed
    20.f, // damage 
    .6f, // bullet scale
    4, // collateral count
};

M9::M9() : 
Weapon(AnimUtil::WeaponAnim::m9, 
        m9BulletReg, 
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

void M9::CreateProjectile(Player& player, std::vector<std::unique_ptr<Projectile>>& projectiles) {
    sf::Vector2f adjustedNormal = (GetTargetWithSpread(mousePosGlobal) - GetPosition()).normalized();
    if(!isUpgraded){
        projectiles.emplace_back(std::make_unique<Projectile>(projectileData, muzzlePosition, adjustedNormal));
    } else {
        projectiles.emplace_back(std::make_unique<M9AtomBullet>(projectileData, muzzlePosition, adjustedNormal));
    }
    
}

void M9::UpgradeWeapon() {
    sprite.setTextureRect(AnimUtil::WeaponAnim::m9Upgraded);
    projectileData = m9BulletUpgrade;
    isUpgraded = true;
}

//307x410 288,783 // total 102x each 102.5y
M9AtomBullet::M9AtomBullet(ProjectileData projectileData, sf::Vector2f position, sf::Vector2f normalized)
: Projectile(projectileData, position, normalized) {
}

void M9AtomBullet::UpdateAnimation(float deltaTime) {
    AnimUtil::UpdateSpriteXYAnim(sprite, animData, deltaTime);
}

void M9AtomBullet::UpdateProjectileStatus(Character& character, GameState& state, 
    std::vector<std::unique_ptr<Projectile>>::iterator& it) {
    
    Monster* monster = dynamic_cast<Monster*>(&character);
    monster->shrink.ApplyShrink(5.f, 0.5f);
        // #TODO ALSO REDUCE DAMAGE OF ENEMIES
    it = state.projectiles.erase(it); 

}