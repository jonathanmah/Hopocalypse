#pragma once
#include "Weapon.h"

class Rpg : public Weapon {

private:
    sf::Sprite smoke;
    sf::Sprite backfire;
    SubRectData smokeData;
    SubRectData backfireData;

public:
    Rpg();
    void CreateProjectile(std::vector<std::unique_ptr<Projectile>>& projectiles) override;
    void UpdateBase(sf::Vector2f characterPosition, float deltaTime) override;
    virtual void Draw(sf::RenderWindow& window) override;
    void UpdateFireEffects(float deltaTime);
    void RotateEffect();
};

class RPGrocket : public Projectile {
public:
    RPGrocket(ProjectileData projectileData, sf::Vector2f position, sf::Vector2f normalized);
    void UpdatePosition(float deltaTime) override;
    void UpdateProjectileStatus(std::vector<std::unique_ptr<Projectile>>& projectiles, 
        std::vector<std::unique_ptr<Projectile>>::iterator& it, std::vector<std::unique_ptr<AoE>>& aoe, int characterId) override;
};