#include "weapons/derived/Uzi.h"
#include <iostream>
static constexpr float PI = 3.141592;
static constexpr float MIRROR_NEG_Y_AXIS_BOUND = PI/2.f; // pi/2 radians , greater than half of pi
static constexpr float MIRROR_POS_Y_AXIS_BOUND =  -PI/2.f; // -pi/2 pi radians , less than -half of pi


static const ProjectileData uziBulletReg = {
    AnimUtil::ProjectileAnim::medBulletReg,
    20.f, // speed
    15.f, // damage 
    1.f, // bullet scale
    1, // collateral count
};

static const ProjectileData uziBulletUpgrade = {
    AnimUtil::ProjectileAnim::redLaser,
    20.f, // speed
    50.f, // damage 
    .1f, // bullet scale
    1, // collateral count
};

Uzi::Uzi() : 
Weapon(AnimUtil::WeaponAnim::uzi, 
        uziBulletReg, 
        {
            1.f, //scale 
            0.f, // baseOffsetX
            22.f, // baseOffsetY
            0.f, // x offset from muzzle
            -6.5f, // y offset from muzzle (pre recoil translation)
            20.f, // muzzleOffsetscalar
            0.08f, // fireRate
            .15f, // time since last bullet fired
            0.f, // current accumulated spread offset
            .2f, // spread offset max
            .01f, //  spread offset growth
            .015f, // spread offset decay
            .0f, // time left until gun returns back to original position after recoil
            .3f, // total time gun takes to return back
            1.f, // amount of offset, vector created using this scalar opposite point direction
            5.f, // shell offset x
            -5.f, // shell offset y
        }
    ), spriteSecond(*AnimUtil::WeaponAnim::uzi.texture), 
    spriteSecondOffset({20,0}), muzzleSecondOffset({0,-5})
{
    sprite.setOrigin({sprite.getLocalBounds().size.x/2, sprite.getLocalBounds().size.y / 2});
    spriteSecond.setOrigin({sprite.getLocalBounds().size.x/2, sprite.getLocalBounds().size.y / 2});
}

void Uzi::CreateProjectile(std::vector<std::unique_ptr<Projectile>>& projectiles) {
    sf::Vector2f adjustedNormal = (GetTargetWithSpread(mousePosGlobal) - GetPosition()).normalized();
    projectiles.emplace_back(std::make_unique<Projectile>(projectileData, muzzlePosition, adjustedNormal));
    if(isUpgraded) {
        sf::Vector2f adjustedNormalSecond = (GetTargetWithSpread(mousePosGlobal) - GetPosition()).normalized();
        projectiles.emplace_back(std::make_unique<Projectile>(projectileData, muzzlePositionSecond, adjustedNormalSecond));
    }
}

void Uzi::UpgradeWeapon() {
    sprite.setTextureRect(AnimUtil::WeaponAnim::uziUpgraded);
    spriteSecond.setTextureRect(AnimUtil::WeaponAnim::uziUpgraded);
    projectileData = uziBulletUpgrade;
    isUpgraded = true;
}

void Uzi::SetMuzzlePosition() {
    muzzlePosition = sprite.getPosition() + relative.normalized()*weaponData.muzzleOffsetScalar;
    muzzlePosition.x += weaponData.muzzlePosOffsetX;
    muzzlePosition.y += weaponData.muzzlePosOffsetY;
    if(isUpgraded) {          
                                           // using original relative for now
        muzzlePositionSecond = spriteSecond.getPosition() + relative.normalized()*weaponData.muzzleOffsetScalar;
        muzzlePositionSecond.x += weaponData.muzzlePosOffsetX;
        muzzlePositionSecond.y += weaponData.muzzlePosOffsetY;
    }
}

void Uzi::UpdateMuzzleFlashes(float deltaTime) {
    bool isComplete = false;
    for(auto it = muzzleFlash.begin(); it != muzzleFlash.end();) {
        if((*it)->isSecond){
            if((*it)->Update(muzzlePositionSecond, relative.normalized(), deltaTime)){
                it = muzzleFlash.erase(it);
            } else {
                ++it;
            }
        } else {
            if((*it)->Update(muzzlePosition, relative.normalized(), deltaTime)){
                it = muzzleFlash.erase(it);
            } else {
                ++it;
            }
        }
        
    }
}

void Uzi::RotateBaseToMouseGlobal(){
    float radians = atan2(relative.y, relative.x);
    if(radians > MIRROR_NEG_Y_AXIS_BOUND || radians < MIRROR_POS_Y_AXIS_BOUND){
        sprite.setScale({weaponData.scale, -weaponData.scale}); // inverse y axis
        if(isUpgraded) {
            spriteSecond.setScale({weaponData.scale, -weaponData.scale}); // inverse y axis;
            spriteSecond.move({-spriteSecondOffset.x, spriteSecondOffset.y});
        }
    }
    else { // face forward
        sprite.setScale({weaponData.scale, weaponData.scale});
        if(isUpgraded) {
            spriteSecond.setScale({weaponData.scale, weaponData.scale});
            spriteSecond.move(spriteSecondOffset);
        }
    }
    sf::Angle angle = sf::radians(radians);
    sprite.setRotation(angle);
    if(isUpgraded) {
        spriteSecond.setRotation(angle);
    }
}
void Uzi::AddMuzzleFlashEffect(){
    muzzleFlash.emplace_back(
       std::make_unique<MuzzleFlash>(MuzzleFlash::GetNextFlash(),muzzlePosition,relative.normalized(),.2f));
    
    if(isUpgraded) {
        muzzleFlash.emplace_back(
            std::make_unique<MuzzleFlash>(MuzzleFlash::GetNextFlash(),muzzlePositionSecond,relative.normalized(),.2f, true));          
    }
}

void Uzi::CreateShell() {
    // set offset here
    sf::Vector2f ejectPosition = GetPosition();
    sf::Vector2f offset = GetRotatedOffset(weaponData.shellOffsetX, weaponData.shellOffsetY);
    ejectPosition.x += offset.x;
    ejectPosition.y += offset.y;
    shells.emplace_back(std::make_unique<Shell>(ejectPosition, relative));
    if(isUpgraded) {
        shells.emplace_back(std::make_unique<Shell>(spriteSecond.getPosition(), relative));
    }
}


void Uzi::AttemptShoot(std::vector<std::unique_ptr<Projectile>>& projectiles, float deltaTime) {
    if (weaponData.timeSinceShot > weaponData.fireRate) {

        // maybe pass effects as a reference to use the same factory as projectile.
        // call it derived ProjectileEffectsFactory
        //projectiles.push_back(ProjectileFactory::CreateProjectile(this, GetPosition(), relative, GetTargetWithSpread(mousePosGlobal)));
        CreateProjectile(projectiles);
        IncreaseSpread();
        weaponData.timeSinceShot = 0.f;
        SetPositionPostRecoil();
        AddMuzzleFlashEffect();

        CreateShell();
    }
}

void Uzi::UpdateBaseTransformations(sf::Vector2f characterPosition, float deltaTime) {
    sprite.setPosition({characterPosition.x+weaponData.basePosOffsetX, characterPosition.y+weaponData.basePosOffsetY});
    UpdateRecoilReturn(deltaTime);
    relative = mousePosGlobal - GetPosition();

    // if upgraded, get second relative (or use first tbh)
    // set second position
    if(isUpgraded) {
        spriteSecond.setPosition({characterPosition.x+weaponData.basePosOffsetX, characterPosition.y+weaponData.basePosOffsetY});
    }

    // update second recoil

    // virtual to rotate second 
    RotateBaseToMouseGlobal();

    // definitely set second muzzle position
    SetMuzzlePosition();
}

void Uzi::UpdateBase(sf::Vector2f characterPosition, float deltaTime) {
    UpdateBaseTransformations(characterPosition, deltaTime);
}

void Uzi::Update(sf::Vector2f characterPosition, sf::Vector2f mousePosGlobal, std::vector<std::unique_ptr<Projectile>>& projectiles, float deltaTime) {
    UpdateMuzzleFlashes(deltaTime); // update effects
    UpdateShells(deltaTime);
 
    SetMousePosGlobal(mousePosGlobal);
 //    Update transformations / any other derived overrides
     UpdateBase(characterPosition, deltaTime);
     if(sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)) {
         AttemptShoot(projectiles, deltaTime);
     } else {
         DecreaseSpread();
     }
     weaponData.timeSinceShot += deltaTime;
 }


void Uzi::Draw(sf::RenderWindow& window) {
    if(isUpgraded){
        //spriteSecond.setColor(sf::Color{255,100,100});
        window.draw(spriteSecond);
    }
    window.draw(sprite);
    DrawMuzzleFlashes(window);
    DrawShells(window);
    // HitboxDebugger::DrawSpriteGlobalBoundsHitbox(window, sprite);
    // HitboxDebugger::DrawSpriteOrigin(window, sprite, sf::Color::Magenta);
}