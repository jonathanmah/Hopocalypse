#pragma once
#include <SFML/Graphics.hpp>
#include <unordered_set>
#include "util/AnimUtil.h"
#include "entities/Character.h"
#include "weapons/Weapon.h"
#include "core/BatchRenderer.h"
#include "util/HitboxDebugger.h"
#include "fx/AoE.h"
#include "fx/Explosion.h"

class GameState;
//class Character;
class Weapon;
struct ProjectileData;

class Projectile {

protected:
    sf::Sprite sprite;
    AnimData animData;
    float acceleration;
    int collateralCount;
    std::unordered_set<int> hitCharacters;

    

public:
    sf::Vector2f velocity;
    sf::Vector2f sourcePosition;
    float damage;
    float lifetime;
    float scale;

    bool createsBlood;
    
    Projectile(ProjectileData projectileData, sf::Vector2f sourcePosition, sf::Vector2f normalized);

    bool HasHit(int characterId);
    // this should receive delta time, every frame it runs
    virtual void UpdatePosition(float deltaTime);
    void Draw(sf::RenderWindow& window);

    // this update is for current moving
    static void UpdateProjectiles(GameState& state, float deltaTime);
    static void RenderProjectiles(GameState& state, bool drawHitbox = false);

    // THIS UPDATE BELOW IS ON COLLISION
    // creating this to make virtual function that can be overidden by rpg for explosions
    virtual void UpdateProjectileStatus(Character& character, GameState& state, // passing state now for aoe, projectiles, status effects
        std::vector<std::unique_ptr<Projectile>>::iterator& it);
    void Rotate(sf::Vector2f velocity);
    void SetPosition(sf::Vector2f position);
    virtual void UpdateAnimation(float deltaTime);
    
    virtual bool ReachedDest(){return false;} // REACH TARGET DEST
    virtual void ActivatePostTargetReached(GameState& state, std::vector<std::unique_ptr<Projectile>>::iterator& it) {return;}

    inline sf::Sprite& GetSprite() { return sprite;}
    inline sf::Vector2f GetPosition() {return sprite.getPosition();}
    inline sf::FloatRect GetGlobalBounds() {return sprite.getGlobalBounds();}
    inline void SetDamage(float newDamage) {damage = newDamage;}
    inline float GetDamage() {return damage;}
    inline void DecrementCollateralCount() {collateralCount -= 1;}
    inline int GetCollateralCount() {return collateralCount;}
};