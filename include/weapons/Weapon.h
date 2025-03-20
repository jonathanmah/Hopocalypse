#pragma once
#include <SFML/Graphics.hpp>
#include <random>
#include "../util/AnimUtil.h"
#include "../weapons/Projectile.h"
#include "../util/RandomUtil.h"
#include "../fx/MuzzleFlash.h"
#include "../fx/Shell.h"


struct Flame {
    sf::Vector2f position;
    sf::Vector2f velocity;
    float lifetime;
    float size;
    sf::Color colour;
};



class BatchRenderer;
class Projectile;
class GameState;

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
    virtual void RotateBaseToMouseGlobal();
    void IncreaseSpread();
    void DecreaseSpread();
    sf::Vector2f GetTargetWithSpread(sf::Vector2f mousePosGlobal);
    void SetPositionPostRecoil();
    void UpdateRecoilReturn(float deltaTime);
    virtual void SetMuzzlePosition();
    inline void SetMousePosGlobal(sf::Vector2f mousePos){mousePosGlobal = mousePos;}
    virtual void UpdateMuzzleFlashes(float deltaTime);
    void DrawMuzzleFlashes(sf::RenderWindow& window);
    virtual void CreateShell();
    void UpdateShells(float deltaTime);
    void DrawShells(sf::RenderWindow& window);

public:
    bool isUpgraded;

    Weapon(AnimData animData, ProjectileData projectileData, WeaponData weaponData);
    virtual ~Weapon() = default;
    // need to make this virtual for akimbo uzi
    virtual void Update(GameState& state, sf::Vector2f characterPosition, sf::Vector2f mousePosGlobal, float deltaTime);
    // override to create a different projectile
    virtual void CreateProjectile(std::vector<std::unique_ptr<Projectile>>& projectiles) = 0;
    // override to handle upgrading
    virtual void UpgradeWeapon() = 0;
    // override if changing muzzle flash, recoil, spread, firerate
    virtual void AttemptShoot(std::vector<std::unique_ptr<Projectile>>& projectiles, float deltaTime);
    // override to modify any transformations done to base of gun,  OR texture updates
    virtual void UpdateBase(sf::Vector2f characterPosition, float deltaTime);
    // override to add a different type of muzzle effect
    virtual void AddMuzzleFlashEffect();
    // override to draw special effects
    virtual void Draw(sf::RenderWindow& window, BatchRenderer& batchRenderer);

    virtual void UpdateBaseTransformations(sf::Vector2f characterPosition, float deltaTime);
    
    
    inline void SetPosition(sf::Vector2f position) {sprite.setPosition(position);}
    inline sf::Vector2f GetMuzzlePosition() {return muzzlePosition;};

    inline ProjectileData GetProjectileData() {return projectileData;};
    inline sf::Vector2f GetPosition() {return sprite.getPosition();}
    inline float GetFireRate() {return weaponData.fireRate;}
    inline sf::FloatRect GetGlobalBounds() {return sprite.getGlobalBounds();}
};