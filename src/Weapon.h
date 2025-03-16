#pragma once
#include <SFML/Graphics.hpp>
#include <random>
#include "AnimUtil.h"
#include "Projectile.h"
#include "RandomUtil.h"
#include "MuzzleFlash.h"
#include "Shell.h"

class AK47;
class Projectile;

struct ProjectileData {
    AnimData anim;
    float speed;
    float damage;
    float scale;
    int collateralCount;
    float acceleration = 1.f;
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

    float shellOffsetX;
    float shellOffsetY;
};

class Weapon {

protected:
    sf::Sprite sprite;
    WeaponData weaponData;
    ProjectileData projectileData;
    std::vector<std::unique_ptr<MuzzleFlash>> muzzleFlash;
    std::vector<std::unique_ptr<Shell>> shells;
    sf::Vector2f relative; // relative vector from weapon origin to global mouse pos
    sf::Vector2f muzzlePosition; // Position of the weapon muzzle
    sf::Vector2f mousePosGlobal;

    sf::Vector2f GetRotatedOffset(float offsetX, float offsetY);
    void RotateBaseToMouseGlobal();
    void IncreaseSpread();
    void DecreaseSpread();
    sf::Vector2f GetTargetWithSpread(sf::Vector2f mousePosGlobal);
    void SetPositionPostRecoil();
    void UpdateRecoilReturn(float deltaTime);
    void SetMuzzlePosition();
    inline void SetMousePosGlobal(sf::Vector2f mousePos){mousePosGlobal = mousePos;}
    void UpdateMuzzleFlashes(float deltaTime);
    void DrawMuzzleFlashes(sf::RenderWindow& window);
    void CreateShell();
    void UpdateShells(float deltaTime);
    void DrawShells(sf::RenderWindow& window);

public:
    Weapon(AnimData animData, ProjectileData projectileData, WeaponData weaponData);
    virtual ~Weapon() = default; //DONT REALLY GET WHY NEED THIS IDK YET NEED2LEARN
    void Update(sf::Vector2f characterPosition, sf::Vector2f mousePosGlobal, std::vector<std::unique_ptr<Projectile>>& projectiles, float deltaTime);
    // override to create a different projectile
    virtual void CreateProjectile(std::vector<std::unique_ptr<Projectile>>& projectiles) = 0;
    // override if changing muzzle flash, recoil, spread, firerate
    virtual void AttemptShoot(std::vector<std::unique_ptr<Projectile>>& projectiles, float deltaTime);
    // override to modify any transformations done to base of gun,  OR texture updates
    virtual void UpdateBase(sf::Vector2f characterPosition, float deltaTime);
    // override to add a different type of muzzle effect
    virtual void AddMuzzleFlashEffect();
    // override to draw special effects
    virtual void Draw(sf::RenderWindow& window);

    void UpdateBaseTransformations(sf::Vector2f characterPosition, float deltaTime);
    
    
    inline void SetPosition(sf::Vector2f position) {sprite.setPosition(position);}
    inline sf::Vector2f GetMuzzlePosition() {return muzzlePosition;};

    inline ProjectileData GetProjectileData() {return projectileData;};
    inline sf::Vector2f GetPosition() {return sprite.getPosition();}
    inline float GetFireRate() {return weaponData.fireRate;}
    inline sf::FloatRect GetGlobalBounds() {return sprite.getGlobalBounds();}
};