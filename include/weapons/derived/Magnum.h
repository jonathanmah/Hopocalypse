#pragma once
#include "weapons/Weapon.h"


class Magnum : public Weapon {

public:
    Magnum();
    void CreateProjectile(std::vector<std::unique_ptr<Projectile>>& projectiles) override;
    void UpgradeWeapon() override;
};