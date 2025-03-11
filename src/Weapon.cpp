#include <iostream>
#include <cmath>

#include "Weapon.h"
#include "RandomUtil.h"
static constexpr float PI = 3.141592;
static constexpr float MIRROR_NEG_Y_AXIS_BOUND = PI/2.f; // pi/2 radians , greater than half of pi
static constexpr float MIRROR_POS_Y_AXIS_BOUND =  -PI/2.f; // -pi/2 pi radians , less than -half of pi
// each weapon has, may have,  hardcoded pos offset, origin, damage, bullet speed, fireRate. bullet anim.
// y axis offset for bullet leaving muzzle, AND x offset too forgot that,
// muzzle flash or rocket leaving behind

// when constructing a weapon, there should be 2 states, in hands, or floating above box
Weapon::Weapon(AnimData animData, ProjectileData projectileData, WeaponData weaponData): sprite(*animData.texture), projectileData(projectileData), weaponData(weaponData) {
    sprite.setTextureRect(sf::IntRect(animData.textureFrame.position, animData.textureFrame.size));
    sprite.setScale({weaponData.scale, weaponData.scale});
}

inline void Weapon::IncreaseSpread() {
    weaponData.spreadDeviationCurr = std::min(weaponData.spreadDeviationCurr+weaponData.spreadDeviationGrowth, weaponData.spreadDeviationMax);
}

inline void Weapon::DecreaseSpread() {
    weaponData.spreadDeviationCurr = std::max(weaponData.spreadDeviationCurr-weaponData.spreadDeviationDecay,0.f);
}

sf::Vector2f Weapon::GetTargetWithSpread(sf::Vector2f mousePosGlobal) {
    float offset = RandomUtil::GetRandomFloat(weaponData.spreadDeviationCurr, weaponData.spreadDeviationCurr);
    // facing right, perpendicular up direction for (x,y) is (y,-x), (-y,x) for facing left
    sf::Vector2f perpendicular = (relative.x >= 0) ? sf::Vector2f(relative.y, -relative.x): sf::Vector2f(-relative.y, relative.x);
    sf::Vector2f normalized = perpendicular.normalized();
    float deviationScalar = RandomUtil::GetRandomFloat(-weaponData.spreadDeviationCurr/3, weaponData.spreadDeviationCurr);
    return {mousePosGlobal.x + perpendicular.x*deviationScalar,
         mousePosGlobal.y + perpendicular.y*deviationScalar};
}

sf::Vector2f Weapon::GetMuzzlePosition(sf::Vector2f normalizedDirection) {
    // take character position
    // get normal direction
    // use scalar towardsd normal direction
    // use additional projectile offsets to position
    // return the end vector
    sf::Vector2f res = sprite.getPosition() + normalizedDirection*weaponData.muzzleOffsetScalar;
    res.x += weaponData.muzzlePosOffsetX;
    res.y += weaponData.muzzlePosOffsetY;
    return res;
}

// ENTRY POINT FROM CHARACTER CLASS
void Weapon::Update(sf::Vector2f characterPosition, sf::Vector2f mousePosGlobal, std::vector<std::unique_ptr<Projectile>>& projectiles, float deltaTime) {
    // Update transformations / any other derived overrides
    UpdateBase(characterPosition, mousePosGlobal, deltaTime);
    
    // update projectile list, 
    if(sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)) {
        // 
        AttemptShoot(projectiles, characterPosition, mousePosGlobal, deltaTime);
    } else {
        DecreaseSpread();
    }
    weaponData.timeSinceShot += deltaTime;
}

void Weapon::SetPositionPostRecoil() {
    // opposite vector of (x,y) is (-x,-y)
    if(weaponData.recoilTimeRemain > 0) return;
    
    sf::Vector2f opposite{-relative.x, -relative.y};
    auto normal = opposite.normalized();
    sprite.move(normal*weaponData.recoilOffsetScalar);
    weaponData.recoilTimeRemain = weaponData.recoilTimeTotal;
}

void Weapon::UpdateRecoilReturn(float deltaTime) {
    if (weaponData.recoilTimeRemain > 0.f){
        sf::Vector2f opposite{-relative.x, -relative.y};
        sprite.move(opposite.normalized() * (weaponData.recoilOffsetScalar*(weaponData.recoilTimeRemain/weaponData.recoilTimeTotal)));
    }
    weaponData.recoilTimeRemain = std::max(weaponData.recoilTimeRemain - deltaTime, 0.f);
    std::cout << weaponData.recoilTimeRemain << std::endl;
}

// SHOOT FUNC /  UPDATE SPREAD / RECOIL / PROJECTILE LIST 
void Weapon::AttemptShoot(std::vector<std::unique_ptr<Projectile>>& projectiles, sf::Vector2f characterPosition, sf::Vector2f mousePosGlobal, float deltaTime) {
    if (weaponData.timeSinceShot > weaponData.fireRate) {
        // adjust mousePosGlobal
        // would have to calculate the offset based off of the perp vector
        // maybe increment an offset RANGE, and calculate random offset between 
        // modify target pos by finding the normal
        //sf::Vector2f targetPos = {mousePosGlobal.x+, mousePosGlobal.y+weaponData.basePosOffsetY}
        projectiles.push_back(ProjectileFactory::CreateProjectile(this, GetPosition(), relative, GetTargetWithSpread(mousePosGlobal)));
        IncreaseSpread();
        weaponData.timeSinceShot = 0.f;
        // set back recoil
        SetPositionPostRecoil();
    }
}

void Weapon::UpdateBase(sf::Vector2f characterPosition, sf::Vector2f mousePosGlobal, float deltaTime) {
    UpdateBaseTransformations(characterPosition, mousePosGlobal, deltaTime);
}

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

// Updates the position and pointing direction
void Weapon::UpdateBaseTransformations(sf::Vector2f characterPosition, sf::Vector2f mousePosGlobal, float deltaTime) {
    sprite.setPosition({characterPosition.x+weaponData.basePosOffsetX, characterPosition.y+weaponData.basePosOffsetY});
    UpdateRecoilReturn(deltaTime);
    relative = mousePosGlobal - GetPosition();
    RotateBaseToMouseGlobal();
}

//#todo repetitive code
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
    //DrawHitbox(sprite, window);
}



AK47::AK47() : 
Weapon(AnimUtil::WeaponAnim::AK47anim, 
        {
            AnimUtil::ProjectileAnim::RegularBullet::anim,
            20.f, // speed
            50.f, // damage 
            1.f, // bullet scale
            1, // collateral count
        }, 
        {
            1.f, //scale 
            0.f, // baseOffsetX
            18.f, // baseOffsetY
            0.f, // x offset from muzzle
            -2.5f, // y offset from muzzle (pre recoil translation)
            50.f, // muzzleOffsetscalar
            0.15f, // fireRate
            .15f, // time since last bullet fired
            0.f, // current accumulated spread offset
            .12f, // spread offset max
            .01f, //  spread offset growth
            .015f, // spread offset decay
            .0f, // time left until gun returns back to original position after recoil
            .15f, // total time gun takes to return back
            5.f, // amount of offset, vector created using this scalar opposite point direction
            

        }
    )
{
    sprite.setOrigin({sprite.getLocalBounds().size.x / 3, sprite.getLocalBounds().size.y / 2});
}


FAMAS::FAMAS() : 
Weapon(AnimUtil::WeaponAnim::FAMASanim, 
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
            21.f, // baseOffsetY
            0.f, // x offset from muzzle
            -9.f, // y offset from muzzle
            50.f, // muzzleOffsetscalar
            0.06f, // fireRate
            .06f, // time since last bullet fired
            0.f, // current accumulated spread offset
            .12f, // spread offset max
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
void FAMAS::AttemptShoot(std::vector<std::unique_ptr<Projectile>>& projectiles, sf::Vector2f characterPosition, sf::Vector2f mousePosGlobal, float deltaTime) {
    if (weaponData.timeSinceShot > weaponData.fireRate) {
        weaponData.fireRate = 0.10f;
        projectiles.push_back(ProjectileFactory::CreateProjectile(this, GetPosition(), relative, GetTargetWithSpread(mousePosGlobal)));
        IncreaseSpread();
        weaponData.timeSinceShot = 0.f;
        SetPositionPostRecoil();
        if(++burstFireCounter >=3) {
            weaponData.fireRate =  0.3f;
            burstFireCounter = 0;
        }
    }
}

Barrett50::Barrett50() : 
Weapon(AnimUtil::WeaponAnim::Barrett50anim, 
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
            12.f, // baseOffsetY
            0.f, // x offset from muzzle
            -4.f, // y offset from muzzle
            80.f, // muzzleOffsetscalar
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


RPG::RPG() : 
Weapon(AnimUtil::WeaponAnim::RPGanim, 
        {
            AnimUtil::WeaponAnim::RPGrocketAnim,
            20.f, // speed
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
            .9f, // total time gun takes to return back
            15.f, // amount of offset, vector created using this scalar opposite point direction

        }
    )
{
    sprite.setOrigin({sprite.getLocalBounds().size.x / 2.4f, sprite.getLocalBounds().size.y / 2});
}

// set new sub rectangle to display texture without rocket loaded
//void UpdateBase(sf::Vector2f characterPosition, sf::Vector2f mousePosRelative, float deltaTime) override;
void RPG::UpdateBase(sf::Vector2f characterPosition, sf::Vector2f mousePosGlobal, float deltaTime) {
    UpdateBaseTransformations(characterPosition, mousePosGlobal, deltaTime);
    weaponData.timeSinceShot += deltaTime;
    if(weaponData.timeSinceShot < weaponData.fireRate) {
        sprite.setTextureRect(AnimUtil::WeaponAnim::RPGreloadRect);
    } else {
        sprite.setTextureRect(AnimUtil::WeaponAnim::RPGloadedRect);
    }
}