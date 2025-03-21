#pragma once
#include "weapons/Weapon.h"

class GameState;

class Famas : public Weapon {

private:
    int burstFireCounter;
    float attemptShotTimer;

public:
    Famas();
    void Update(GameState& state, Player& player, sf::Vector2f mousePosGlobal, float deltaTime) override;
    void CreateProjectile(std::vector<std::unique_ptr<Projectile>>& projectiles) override;
    void AttemptShoot(std::vector<std::unique_ptr<Projectile>>& projectiles, float deltaTime) override;
    void UpgradeWeapon() override;
};

class FamasIceBullet : public Projectile {

    public:
        FamasIceBullet(ProjectileData projectileData, sf::Vector2f position, sf::Vector2f normalized);
        void UpdateProjectileStatus(Character& character, std::vector<std::unique_ptr<Projectile>>& projectiles, 
            std::vector<std::unique_ptr<Projectile>>::iterator& it, std::vector<std::unique_ptr<AoE>>& aoe) override;
    };