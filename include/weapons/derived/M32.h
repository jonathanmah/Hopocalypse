#pragma once
#include "weapons/Weapon.h"


class M32 : public Weapon {

public:
    M32();
    void CreateProjectile(std::vector<std::unique_ptr<Projectile>>& projectiles) override;
    void UpgradeWeapon() override;
};