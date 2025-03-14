#pragma once
#include <SFML/Graphics.hpp>
#include <unordered_set>
#include "AnimUtil.h"
#include "Character.h"
#include "Weapon.h"
#include "BatchRenderer.h"

class Character;
class Weapon;
struct ProjectileData;

class Projectile {

protected:
    sf::Sprite sprite;
    static sf::Texture texture;
    sf::Vector2f position;
    sf::Vector2f velocity;
    float scale;
    float damage;
    int collateralCount;
    std::unordered_set<int> hitCharacters;

    

public:
    Projectile(ProjectileData projectileData, sf::Vector2f position, sf::Vector2f normalized);

    bool HasHit(int characterId);
    // this should receive delta time, every frame it runs
    virtual void UpdatePosition();
    void Draw(sf::RenderWindow& window);

    static void UpdateProjectiles(std::vector<std::unique_ptr<Projectile>>& projectiles);
    static void RenderProjectiles(std::vector<std::unique_ptr<Projectile>>& projectiles, BatchRenderer& batchRenderer, sf::RenderWindow& window);

    // creating this to make virtual function that can be overidden by rpg for explosions
    virtual void UpdateProjectileStatus(std::vector<std::unique_ptr<Projectile>>& projectiles, 
        std::vector<std::unique_ptr<Projectile>>::iterator& it, int characterId);
    void Rotate(sf::Vector2f velocity);
    void SetPosition(sf::Vector2f position);

    inline sf::Vector2f GetPosition() {return sprite.getPosition();}
    inline sf::FloatRect GetGlobalBounds() {return sprite.getGlobalBounds();}
    inline void SetDamage(float newDamage) {damage = newDamage;}
    inline float GetDamage() {return damage;}
    inline void DecrementCollateralCount() {collateralCount -= 1;}
    inline int GetCollateralCount() {return collateralCount;}
};

class RPGrocket : public Projectile {
public:
    RPGrocket(ProjectileData projectileData, sf::Vector2f position, sf::Vector2f normalized);
    void UpdatePosition() override;
    void UpdateProjectileStatus(std::vector<std::unique_ptr<Projectile>>& projectiles, 
        std::vector<std::unique_ptr<Projectile>>::iterator& it, int characterId) override;
};