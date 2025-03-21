#pragma once
#include "weapons/Weapon.h"

class Rpg : public Weapon {

private:
    sf::Sprite smoke;
    sf::Sprite backfire;
    SubRectData smokeData;
    SubRectData backfireData;
    sf::IntRect loadedRect;
    sf::IntRect reloadRect;
    AnimData explosion;

public:
    Rpg();
    void CreateProjectile(std::vector<std::unique_ptr<Projectile>>& projectiles) override;
    void UpdateBase(sf::Vector2f characterPosition, float deltaTime) override;
    virtual void Draw(sf::RenderWindow& window, BatchRenderer& batchRenderer) override;
    void UpdateFireEffects(float deltaTime);
    void RotateEffect();
    void UpgradeWeapon() override;
};

class RPGrocket : public Projectile {
public:
    AnimData explosion;

    RPGrocket(ProjectileData projectileData, sf::Vector2f position, sf::Vector2f normalized, AnimData explosion);
    void UpdatePosition(float deltaTime) override;
    void UpdateProjectileStatus(Character& character, std::vector<std::unique_ptr<Projectile>>& projectiles, 
        std::vector<std::unique_ptr<Projectile>>::iterator& it, std::vector<std::unique_ptr<AoE>>& aoe) override;
};