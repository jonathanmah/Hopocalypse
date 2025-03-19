#pragma once
#include "weapons/Weapon.h"


class M240 : public Weapon {

public:
    M240();
    void CreateProjectile(std::vector<std::unique_ptr<Projectile>>& projectiles) override;
    void UpgradeWeapon() override;
};