#pragma once
#include "weapons/Weapon.h"

class GameState;

class Famas : public Weapon {

private:
    int burstFireCounter;
    float attemptShotTimer;

public:
    Famas();
    void Update(GameState& state, sf::Vector2f characterPosition, sf::Vector2f mousePosGlobal, float deltaTime) override;
    void CreateProjectile(std::vector<std::unique_ptr<Projectile>>& projectiles) override;
    void AttemptShoot(std::vector<std::unique_ptr<Projectile>>& projectiles, float deltaTime) override;
    void UpgradeWeapon() override;
};