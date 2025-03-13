#pragma once
#include "Weapon.h"

class Famas : public Weapon {

private:
    int burstFireCounter;
public:
    Famas();
    void CreateProjectile(std::vector<std::unique_ptr<Projectile>>& projectiles) override;
    void AttemptShoot(std::vector<std::unique_ptr<Projectile>>& projectiles, float deltaTime) override;
};