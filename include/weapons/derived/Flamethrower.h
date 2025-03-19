#pragma once
#include "weapons/Weapon.h"


class Flamethrower : public Weapon {

public:
    Flamethrower();
    void CreateProjectile(std::vector<std::unique_ptr<Projectile>>& projectiles) override;
    void UpgradeWeapon() override;
};