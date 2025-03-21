#pragma once
#include <SFML/Graphics.hpp>
#include <unordered_set>
#include "../util/AnimUtil.h"
#include "../entities/Character.h"
#include "weapons/Weapon.h"
#include "../core/BatchRenderer.h"
#include "../util/HitboxDebugger.h"
#include "../fx/AoE.h"

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
    sf::Vector2f position;
    float damage;
    float lifetime;
    float scale;
    
    Projectile(ProjectileData projectileData, sf::Vector2f position, sf::Vector2f normalized);

    bool HasHit(int characterId);
    // this should receive delta time, every frame it runs
    virtual void UpdatePosition(float deltaTime);
    void Draw(sf::RenderWindow& window);

    // this update is for current moving
    static void UpdateProjectiles(GameState& state, float deltaTime);
    static void RenderProjectiles(GameState& state, bool drawHitbox = false);

    // THIS UPDATE BELOW IS ON COLLISION
    // creating this to make virtual function that can be overidden by rpg for explosions
    virtual void UpdateProjectileStatus(Character& character, std::vector<std::unique_ptr<Projectile>>& projectiles, 
        std::vector<std::unique_ptr<Projectile>>::iterator& it, std::vector<std::unique_ptr<AoE>>& aoe);
    void Rotate(sf::Vector2f velocity);
    void SetPosition(sf::Vector2f position);
    virtual void UpdateAnimation(float deltaTime);

    inline sf::Sprite& GetSprite() { return sprite;}
    inline sf::Vector2f GetPosition() {return sprite.getPosition();}
    inline sf::FloatRect GetGlobalBounds() {return sprite.getGlobalBounds();}
    inline void SetDamage(float newDamage) {damage = newDamage;}
    inline float GetDamage() {return damage;}
    inline void DecrementCollateralCount() {collateralCount -= 1;}
    inline int GetCollateralCount() {return collateralCount;}
};