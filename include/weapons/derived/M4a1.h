#pragma once
#include "weapons/Weapon.h"


class M4a1 : public Weapon {

public:
    M4a1();
    void CreateProjectile(std::vector<std::unique_ptr<Projectile>>& projectiles) override;
    void UpgradeWeapon() override;
};