#pragma once
#include "weapons/Weapon.h"


class P90 : public Weapon {

public:
    P90();
    void CreateProjectile(std::vector<std::unique_ptr<Projectile>>& projectiles) override;
    void UpgradeWeapon() override;
};