#include <iostream>
#include <cmath>
#include "HitboxDebugger.h"
#include "Weapon.h"
#include "RandomUtil.h"
static constexpr float PI = 3.141592;
static constexpr float MIRROR_NEG_Y_AXIS_BOUND = PI/2.f; // pi/2 radians , greater than half of pi
static constexpr float MIRROR_POS_Y_AXIS_BOUND =  -PI/2.f; // -pi/2 pi radians , less than -half of pi
// each weapon has, may have,  hardcoded pos offset, origin, damage, bullet speed, fireRate. bullet anim.
// y axis offset for bullet leaving muzzle, AND x offset too forgot that,
// muzzle flash or rocket leaving behind

// when constructing a weapon, there should be 2 states, in hands, or floating above box
Weapon::Weapon(AnimData animData, ProjectileData projectileData, WeaponData weaponData): sprite(*animData.texture), projectileData(projectileData), weaponData(weaponData), muzzlePosition({0,0}) {
    sprite.setTextureRect(sf::IntRect(animData.textureFrame.position, animData.textureFrame.size));
    sprite.setScale({weaponData.scale, weaponData.scale});
}

// --------------------- RECOIL AND SPREAD -------------------------------
// Increase the bullet spread
inline void Weapon::IncreaseSpread() {
    weaponData.spreadDeviationCurr = std::min(weaponData.spreadDeviationCurr+weaponData.spreadDeviationGrowth, weaponData.spreadDeviationMax);
}
// Decrease the bullet spread
inline void Weapon::DecreaseSpread() {
    weaponData.spreadDeviationCurr = std::max(weaponData.spreadDeviationCurr-weaponData.spreadDeviationDecay,0.f);
}

// Adjusts the target position with bullet spread factored in
sf::Vector2f Weapon::GetTargetWithSpread(sf::Vector2f mousePosGlobal) {
    float offset = RandomUtil::GetRandomFloat(weaponData.spreadDeviationCurr, weaponData.spreadDeviationCurr);
    // facing right, perpendicular up direction for (x,y) is (y,-x), (-y,x) for facing left
    sf::Vector2f perpendicular = (relative.x >= 0) ? sf::Vector2f(relative.y, -relative.x): sf::Vector2f(-relative.y, relative.x);
    sf::Vector2f normalized = perpendicular.normalized();
    float deviationScalar = RandomUtil::GetRandomFloat(-weaponData.spreadDeviationCurr/3, weaponData.spreadDeviationCurr);
    return {mousePosGlobal.x + perpendicular.x*deviationScalar,
         mousePosGlobal.y + perpendicular.y*deviationScalar};
}

// Updates the position of the recoil to come back using ratio of time remaining/total 
// time and scalar to move with opposite normalized
void Weapon::UpdateRecoilReturn(float deltaTime) {
    if (weaponData.recoilTimeRemain > 0.f){
        sf::Vector2f opposite{-relative.x, -relative.y};
        sprite.move(opposite.normalized() * (weaponData.recoilOffsetScalar*(weaponData.recoilTimeRemain/weaponData.recoilTimeTotal)));
    }
    weaponData.recoilTimeRemain = std::max(weaponData.recoilTimeRemain - deltaTime, 0.f);
}

// Sets the recoil position after shooting, resets recoil timer to return to original position
void Weapon::SetPositionPostRecoil() {
    // opposite vector of (x,y) is (-x,-y)
    if(weaponData.recoilTimeRemain > 0) return;
    
    sf::Vector2f opposite{-relative.x, -relative.y};
    auto normal = opposite.normalized();
    sprite.move(normal*weaponData.recoilOffsetScalar);
    weaponData.recoilTimeRemain = weaponData.recoilTimeTotal;
}

// --------------------- ^^^^ RECOIL AND SPREAD ^^^ -------------------------------

// --------------------- BASE TRANSFORMATIONS / UPDATES  ----------------------------

// Rotates the base of the weapon to point towards the cursor
// should probably refactor this into a util
void Weapon::RotateBaseToMouseGlobal(){
    float radians = atan2(relative.y, relative.x);
    if(radians > MIRROR_NEG_Y_AXIS_BOUND || radians < MIRROR_POS_Y_AXIS_BOUND)
        sprite.setScale({weaponData.scale, -weaponData.scale}); // inverse y axis
    else {
        sprite.setScale({weaponData.scale, weaponData.scale});
    }
    sf::Angle angle = sf::radians(radians);
    sprite.setRotation(angle);
}

// ALL TRANSLATIONS AND ROTATIONS are done here, including recoil.
// Sets the latest relative vector from origin of weapon to mouse position
void Weapon::UpdateBaseTransformations(sf::Vector2f characterPosition, float deltaTime) {
    sprite.setPosition({characterPosition.x+weaponData.basePosOffsetX, characterPosition.y+weaponData.basePosOffsetY});
    UpdateRecoilReturn(deltaTime);
    relative = mousePosGlobal - GetPosition();
    RotateBaseToMouseGlobal();
    SetMuzzlePosition();
}

// Virtual method for updates to the weapon.
// Any updates to weapon texture, position outside of shooting should be done called from here
// Derived classes that override this
// RPG : set new sub rectangle to display texture without rocket loaded
void Weapon::UpdateBase(sf::Vector2f characterPosition, float deltaTime) {
    UpdateBaseTransformations(characterPosition, deltaTime);
}
// --------------------- ^ BASE TRANSFORMATIONS ^ ----------------------------




// ------------------------- WEAPON EFFECTS ------------------------- WEAPON EFFECTS -------------------------/////


void Weapon::AddMuzzleFlashEffect(){
    muzzleFlash.emplace_back(
        std::make_unique<MuzzleFlash>(MuzzleFlash{MuzzleFlash::GetNextFlash(),muzzlePosition,relative.normalized(),.3f}));
}

void Weapon::UpdateMuzzleFlashes(float deltaTime) {
    bool isComplete = false;
    for(auto it = muzzleFlash.begin(); it != muzzleFlash.end();) {
        if((*it)->Update(muzzlePosition, relative.normalized(), deltaTime)){
            it = muzzleFlash.erase(it);
        } else {
            ++it;
        }
    }
}


// ------------------------- WEAPON EFFECTS ^ ------------------------- WEAPON EFFECTS ^ -------------------------

// ------------------------ SHOOTING CORE / PROJECTILE CREATION ---------------------------------------------------

// creates projecitle, update recoil/spread
void Weapon::AttemptShoot(std::vector<std::unique_ptr<Projectile>>& projectiles, sf::Vector2f characterPosition, float deltaTime) {
    if (weaponData.timeSinceShot > weaponData.fireRate) {

        // maybe pass effects as a reference to use the same factory as projectile.
        // call it derived ProjectileEffectsFactory
        //projectiles.push_back(ProjectileFactory::CreateProjectile(this, GetPosition(), relative, GetTargetWithSpread(mousePosGlobal)));
        CreateProjectile(projectiles);
        IncreaseSpread();
        weaponData.timeSinceShot = 0.f;
        SetPositionPostRecoil();
        AddMuzzleFlashEffect();
    }
}

// Get base weapon origin, use scalar and offset values to get muzzle position
void Weapon::SetMuzzlePosition() {
    muzzlePosition = sprite.getPosition() + relative.normalized()*weaponData.muzzleOffsetScalar;
    muzzlePosition.x += weaponData.muzzlePosOffsetX;
    muzzlePosition.y += weaponData.muzzlePosOffsetY;
}
// ------------------------ ^ SHOOTING CORE / PROJECTILE CREATION ^ ---------------------------------------------------

// ------------------------ UPDATE ENTRY POINT FROM CHARACTER ---------------------------------------------------

void Weapon::Update(sf::Vector2f characterPosition, sf::Vector2f mousePosGlobal, std::vector<std::unique_ptr<Projectile>>& projectiles, float deltaTime) {
    UpdateMuzzleFlashes(deltaTime); // update effects
    SetMousePosGlobal(mousePosGlobal);
    // Update transformations / any other derived overrides
    UpdateBase(characterPosition, deltaTime);
    if(sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)) {
        AttemptShoot(projectiles, characterPosition, deltaTime); 
    } else {
        DecreaseSpread();
    }
    weaponData.timeSinceShot += deltaTime;
}
// ------------------------ ^ UPDATE ENTRY POINT FROM CHARACTER ^ ---------------------------------------------------


// ------------------------- DRAW ------------------------- DRAW ------------------------------
void Weapon::DrawMuzzleFlashes(sf::RenderWindow& window) {
    for(auto it = muzzleFlash.begin(); it != muzzleFlash.end();++it) {
        (*it)->Draw(window);
    }
}

void DrawHitbox(sf::Sprite& sprite, sf::RenderWindow& window) {
    sf::FloatRect bounds = sprite.getGlobalBounds();
    sf::RectangleShape rect(sf::Vector2f(bounds.size));
    rect.setPosition(bounds.position);
    rect.setFillColor(sf::Color::Transparent);
    rect.setOutlineColor(sf::Color::Red);
    rect.setOutlineThickness(2.0f);
    window.draw(rect);
}

void Weapon::Draw(sf::RenderWindow& window) {
    window.draw(sprite);
    DrawMuzzleFlashes(window);
    //DrawHitbox(sprite, window);
}
// ------------------------  DRAW ^ ----------------------- DRAW ------------------------------




// ------------------------- AK47 ------------------------- AK47 ------------------------------
// AK47::AK47() : 
// Weapon(AnimUtil::WeaponAnim::AK47anim, 
//         {
//             AnimUtil::ProjectileAnim::RegularBullet::anim,
//             20.f, // speed
//             50.f, // damage 
//             1.f, // bullet scale
//             1, // collateral count
//         }, 
//         {
//             1.f, //scale 
//             0.f, // baseOffsetX
//             18.f, // baseOffsetY
//             0.f, // x offset from muzzle
//             -2.5f, // y offset from muzzle (pre recoil translation)
//             50.f, // muzzleOffsetscalar
//             0.15f, // fireRate
//             .15f, // time since last bullet fired
//             0.f, // current accumulated spread offset
//             .12f, // spread offset max
//             .01f, //  spread offset growth
//             .015f, // spread offset decay
//             .0f, // time left until gun returns back to original position after recoil
//             .15f, // total time gun takes to return back
//             5.f, // amount of offset, vector created using this scalar opposite point direction
            

//         }
//     )
// {
//     sprite.setOrigin({sprite.getLocalBounds().size.x / 3, sprite.getLocalBounds().size.y / 2});
// }
// ------------------------- AK47 ^ ------------------------- AK47 ^ ------------------------------


// // ------------------------- Famas --------------------------- Famas ----------------------------
// Famas::Famas() : 
// Weapon(AnimUtil::WeaponAnim::famasAnim, 
//         {
//             AnimUtil::ProjectileAnim::RegularBullet::anim,
//             40.f, // speed
//             100.f, // damage 
//             1.f, // bullet scale
//             1, // collateral count
//         }, 
//         {
//             .95f, //scale 
//             0.f, // baseOffsetX
//             21.f, // baseOffsetY
//             0.f, // x offset from muzzle
//             -9.f, // y offset from muzzle
//             50.f, // muzzleOffsetscalar
//             0.06f, // fireRate
//             .06f, // time since last bullet fired
//             0.f, // current accumulated spread offset
//             .12f, // spread offset max
//             .01f, //  spread offset growth
//             .015f, // spread offset decay
//             .0f, // time left until gun returns back to original position after recoil
//             .10f, // total time gun takes to return back
//             4.f, // amount of offset, vector created using this scalar opposite point direction
//         }
//     ), burstFireCounter(0)
// {
//     sprite.setOrigin({sprite.getLocalBounds().size.x / 3.6f, sprite.getLocalBounds().size.y / 2});
// }
// void Famas::AttemptShoot(std::vector<std::unique_ptr<Projectile>>& projectiles, sf::Vector2f characterPosition, float deltaTime) {
//     if (weaponData.timeSinceShot > weaponData.fireRate) {
//         weaponData.fireRate = 0.10f;
//         projectiles.push_back(ProjectileFactory::CreateProjectile(this, GetPosition(), relative, GetTargetWithSpread(mousePosGlobal)));
//         IncreaseSpread();
//         weaponData.timeSinceShot = 0.f;
//         SetPositionPostRecoil();
//         AddMuzzleFlashEffect();
//         if(++burstFireCounter >=3) {
//             weaponData.fireRate =  0.3f;
//             burstFireCounter = 0;
//         }
//     }
// }
// ------------------------- Famas ^ ---------------------------Famas ^ -------------------------


// ------------------------- BARRET50 --------------------------- BARRET50 ----------------------------
// Barrett50::Barrett50() : 
// Weapon(AnimUtil::WeaponAnim::barrett50Anim, 
//         {
//             AnimUtil::ProjectileAnim::RegularBullet::anim,
//             70.f, // speed
//             150.f, // damage 
//             2.5f, // bullet scale
//             5, // collateral count
//         }, 
//         {
//             .95f, //scale 
//             0.f, // baseOffsetX
//             16.f, // baseOffsetY
//             0.f, // x offset from muzzle
//             -4.f, // y offset from muzzle
//             80.f, // muzzleOffsetscalar
//             .9f, // fireRate
//             .9f, // time since last bullet fired
//             0.f, // current accumulated spread offset
//             .0f, // spread offset max
//             .0f, //  spread offset growth
//             .0f, // spread offset decay
//             0.f, // time left until gun returns back to original position after recoil
//             .9f, // total time gun takes to return back
//             12.f, // amount of offset, vector created using this scalar opposite point direction
            
//         }
//     )
// {
//     sprite.setOrigin({sprite.getLocalBounds().size.x / 3.2f, sprite.getLocalBounds().size.y / 2});
// }

// ------------------------- BARRET50 ^ --------------------------- BARRET50 ^ ----------------------------

// ------------------------- RPG --------------------------- RPG ----------------------------
// Rpg::Rpg() : 
// Weapon(AnimUtil::WeaponAnim::rpgAnim, 
//         {
//             AnimUtil::WeaponAnim::rpgRocketAnim,
//             8.f, // speed
//             150.f, // damage 
//             1.3f, // bullet scale
//             5, // collateral count
//         }, 
//         {
//             1.3f, //scale 
//             0.f, // baseOffsetX
//             3.f, // baseOffsetY
//             0.f, // x offset from muzzle
//             7.5f, // y offset from muzzle
//             50.f, // muzzleOffsetscalar
//             1.8f, // fireRate
//             1.75f, // time since last bullet fired
//             0.f, // current accumulated spread offset
//             .0f, // spread offset max
//             .0f, //  spread offset growth
//             .0f, // spread offset decay
//             0.f, // time left until gun returns back to original position after recoil
//             .3f, // total time gun takes to return back
//             15.f, // amount of offset, vector created using this scalar opposite point direction

//         }
//     )
// {
//     sprite.setOrigin({sprite.getLocalBounds().size.x / 2.4f, sprite.getLocalBounds().size.y / 2});
// }


// // set new sub rectangle to display texture without rocket loaded
// void Rpg::UpdateBase(sf::Vector2f characterPosition, float deltaTime) {
//     UpdateBaseTransformations(characterPosition, deltaTime);
//     weaponData.timeSinceShot += deltaTime;
//     if(weaponData.timeSinceShot < weaponData.fireRate) {
//         sprite.setTextureRect(AnimUtil::WeaponAnim::rpgReloadRect);
//     } else {
//         sprite.setTextureRect(AnimUtil::WeaponAnim::rpgLoadedRect);
//     }
// }
// ------------------------- RPG ^ --------------------------- RPG ^ ----------------------------