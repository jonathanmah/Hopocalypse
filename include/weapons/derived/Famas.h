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
    void CreateProjectile(Player& player, std::vector<std::unique_ptr<Projectile>>& projectiles) override;
    void AttemptShoot(Player& player, std::vector<std::unique_ptr<Projectile>>& projectiles, float deltaTime) override;
    void UpgradeWeapon() override;
};

class FamasIceBullet : public Projectile {

    public:
        FamasIceBullet(ProjectileData projectileData, sf::Vector2f position, sf::Vector2f normalized);
        void UpdateProjectileStatus(Character& character, GameState& state,
            std::vector<std::unique_ptr<Projectile>>::iterator& it) override;
    };