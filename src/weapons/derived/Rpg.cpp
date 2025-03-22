#include "weapons/derived/Rpg.h"
#include "util/TextureUtil.h"
#include "fx/Explosion.h"
#include "core/GameState.h"

static const sf::Texture* PROJECTILE_TEXTURE = TextureUtil::GetTexture("../assets/textures/weapons/projectiles_atlas.png");
static const float SMOKE_OFFSET_SCALAR = 15.f;
static const float BACKFIRE_OFFSET_SCALAR = 70.f;

static const ProjectileData rpgRocketReg = {
    AnimUtil::ProjectileAnim::rpgRocketReg,
    5.f, // speed
    1.f, // damage 
    1.3f, // bullet scale
    1, // collateral count
    1.8f, // acceleration
};

static const ProjectileData rpgRocketUpgrade = {
    AnimUtil::ProjectileAnim::rpgRocketUpgrade,
    20.f, // speed
    1.f, // damage 
    1.3f, // bullet scale
    1, // collateral count
    1.8f, // acceleration
};


Rpg::Rpg() : 
Weapon(AnimUtil::WeaponAnim::rpg, 
        {
            // PROJECTILE DEFINED HERE
            AnimUtil::ProjectileAnim::rpgRocketReg,
            5.f, // speed
            1.f, // damage 
            1.3f, // bullet scale
            1, // collateral count
            1.8f, // acceleration
        }, 
        {
            1.3f, //scale 
            0.f, // baseOffsetX
            9.f, // baseOffsetY
            0.f, // x offset from muzzle
            0.f, // y offset from muzzle
            40.f, // muzzleOffsetscalar
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
    ), smoke(*PROJECTILE_TEXTURE),
    backfire(*PROJECTILE_TEXTURE),
    smokeData(AnimUtil::WeaponFxAnim::rpgSmoke),
    backfireData(AnimUtil::WeaponFxAnim::rpgBackfire),
    loadedRect(AnimUtil::WeaponAnim::rpgLoadedRect),
    reloadRect(AnimUtil::WeaponAnim::rpgReloadRect),
    explosion(AnimUtil::WeaponFxAnim::explosion)
{
    sprite.setOrigin({sprite.getLocalBounds().size.x / 2.4f, sprite.getLocalBounds().size.y / 2});
    smoke.setOrigin({0, static_cast<float>(smokeData.frameSequence[0].size.y / 2)}); // dont forget local bounds is for the texture thats binding. not the subrect
    backfire.setOrigin({static_cast<float>(backfireData.frameSequence[0].size.x / 2),static_cast<float>(backfireData.frameSequence[0].size.y / 2)});
    backfire.setScale({-1,1});
}

void Rpg::CreateProjectile(Player& player, std::vector<std::unique_ptr<Projectile>>& projectiles) {
    sf::Vector2f adjustedNormal = (GetTargetWithSpread(mousePosGlobal) - GetPosition()).normalized();
    projectiles.emplace_back(std::make_unique<RPGrocket>(projectileData, muzzlePosition, adjustedNormal, explosion));
    smoke.setTextureRect(smokeData.frameSequence[0]);
    smokeData.hide = false;
    backfire.setTextureRect(backfireData.frameSequence[0]);
    backfireData.hide = false;
}
// 64 , 38 , 40
// set new sub rectangle to display texture without rocket loaded
void Rpg::UpdateBase(sf::Vector2f characterPosition, float deltaTime) {
    UpdateBaseTransformations(characterPosition, deltaTime);
    weaponData.timeSinceShot += deltaTime;
        if(weaponData.timeSinceShot < weaponData.fireRate-0.05) { // weird bug shooting 5 rockets when swap
            sprite.setTextureRect(reloadRect);
        } else {
            sprite.setTextureRect(loadedRect);
        }
    UpdateFireEffects(deltaTime);
}

// rotate facing direction of gun muzzle
void Rpg::RotateEffect() {
    // get angle of velocity in radians with arc tangent
    float radians = atan2(relative.y,relative.x);
    sf::Angle angle = sf::radians(radians);
    smoke.setRotation(angle);
    backfire.setRotation(angle);
}

void Rpg::UpdateFireEffects(float deltaTime) {
    AnimUtil::UpdateSubRect(smokeData, deltaTime);
    smoke.setTextureRect(smokeData.frameSequence[smokeData.currFrame]);
    smoke.setPosition(GetMuzzlePosition());
    smoke.move(-relative.normalized()*SMOKE_OFFSET_SCALAR);
    AnimUtil::UpdateSubRect(backfireData, deltaTime);
    backfire.setTextureRect(backfireData.frameSequence[backfireData.currFrame]);
    backfire.setPosition(GetPosition());
    backfire.move(-relative.normalized()*BACKFIRE_OFFSET_SCALAR);
    RotateEffect();
}

void Rpg::Draw(sf::RenderWindow& window, BatchRenderer& batchRenderer) {
    window.draw(sprite);
    if(!smokeData.hide){
        window.draw(smoke);
    }
    if(!backfireData.hide){
        window.draw(backfire);
    }
    //HitboxDebugger::DrawSpriteGlobalBoundsHitbox(window, backfire, sf::Color::Yellow);
    //HitboxDebugger::DrawSpriteOrigin(window, backfire, sf::Color::Cyan);
}

void Rpg::UpgradeWeapon() {
    loadedRect = AnimUtil::WeaponAnim::rpgUpgradedLoadedRect;
    reloadRect = AnimUtil::WeaponAnim::rpgUpgradedReloadRect;
    projectileData = rpgRocketUpgrade;
    explosion = AnimUtil::WeaponFxAnim::explosionAlt;
    isUpgraded = true;
}


RPGrocket::RPGrocket(ProjectileData projectileData, sf::Vector2f position, sf::Vector2f normalized, AnimData explosion)
: Projectile(projectileData, position, normalized), explosion(explosion) {
}

void RPGrocket::UpdatePosition(float deltaTime) {
    sprite.setPosition(GetPosition() + velocity);
    Rotate(velocity);
    velocity =  velocity*(1 + acceleration*deltaTime);
}

void RPGrocket::UpdateProjectileStatus(Character& character, GameState& state, 
    std::vector<std::unique_ptr<Projectile>>::iterator& it) {
    // set a detonate flag?? handle explosives somehow?
    //#TODO need to somehow tell rocket animation to explode when this happens here for RPG
    sf::Vector2f position = (*it)->GetPosition();
    state.aoe.emplace_back(std::make_unique<Explosion>(explosion,position));
    it = state.projectiles.erase(it); 
}