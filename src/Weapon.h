#pragma once
#include <SFML/Graphics.hpp>
#include <random>
#include "AnimUtil.h"
#include "Projectile.h"
#include "ProjectileFactory.h"
#include "RandomUtil.h"


class Projectile;

struct ProjectileData {
    AnimData anim;
    float speed;
    float damage;
    float scale;
    int collateralCount;
};

struct WeaponData {
    float scale; // scale weapon base
    float basePosOffsetX; // offset for weapon base
    float basePosOffsetY; // offset for weapon base
    float muzzlePosOffsetX; // offset for muzzle position for bullets
    float muzzlePosOffsetY; // offset for muzzle position for bullets
    float muzzleOffsetScalar; // scalar for muzzle position
    float fireRate; // rate of fire
    float timeSinceShot; // amount of time passed since last shot

    float spreadDeviationCurr; // current accumulated deviation range for bullet spread
    float spreadDeviationMax; // maximum deviation for bullet spread
    float spreadDeviationGrowth; // amount deviation grows per bullet
    float spreadDeviationDecay; // amount deviation decreases if not shooting per frame

    float recoilTimeRemain; // time left until gun returns back to original position after recoil
    float recoilTimeTotal; // total time gun takes to return back
    float recoilOffsetScalar; // amount of offset, vector created using this scalar opposite point direction


};

class Weapon {

protected:
    sf::Sprite sprite;
    WeaponData weaponData;
    ProjectileData projectileData;
    sf::Vector2f relative; // relative vector from weapon origin to global mouse pos

    void RotateBaseToMouseGlobal();
    void IncreaseSpread();
    void DecreaseSpread();
    sf::Vector2f GetTargetWithSpread(sf::Vector2f mousePosGlobal);
    void SetPositionPostRecoil();
    void UpdateRecoilReturn(float deltaTime);

public:
    Weapon(AnimData animData, ProjectileData projectileData, WeaponData weaponData);
    void Update(sf::Vector2f characterPosition, sf::Vector2f mousePosGlobal, std::vector<std::unique_ptr<Projectile>>& projectiles, float deltaTime);

    virtual void AttemptShoot(std::vector<std::unique_ptr<Projectile>>& projectiles, sf::Vector2f characterPosition, sf::Vector2f mousePosGlobal, float deltaTime);
    virtual void UpdateBase(sf::Vector2f characterPosition, sf::Vector2f mousePosGlobal, float deltaTime);

    void UpdateBaseTransformations(sf::Vector2f characterPosition, sf::Vector2f mousePosGlobal, float deltaTime);
    void Draw(sf::RenderWindow& window);
    inline void SetPosition(sf::Vector2f position) {sprite.setPosition(position);}
    sf::Vector2f GetMuzzlePosition(sf::Vector2f normalizedDirection);

    inline ProjectileData GetProjectileData() {return projectileData;};
    inline sf::Vector2f GetPosition() {return sprite.getPosition();}
    inline float GetFireRate() {return weaponData.fireRate;}
    inline sf::FloatRect GetGlobalBounds() {return sprite.getGlobalBounds();}
};

class AK47 : public Weapon {

public:
    AK47();
};

class FAMAS : public Weapon {
private:
    int burstFireCounter;
public:
    FAMAS();
    void AttemptShoot(std::vector<std::unique_ptr<Projectile>>& projectiles, sf::Vector2f characterPosition, sf::Vector2f mousePosGlobal, float deltaTime) override;
};

class Barrett50 : public Weapon {

public:
    Barrett50();
};

class RPG : public Weapon {

public:
    RPG();
    void UpdateBase(sf::Vector2f characterPosition, sf::Vector2f mousePosGlobal, float deltaTime) override;
};
    