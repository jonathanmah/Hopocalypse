#pragma once
#include "weapons/Weapon.h"


class M9 : public Weapon {

public:
    M9();
    void CreateProjectile(std::vector<std::unique_ptr<Projectile>>& projectiles) override;
    void UpgradeWeapon() override;
};