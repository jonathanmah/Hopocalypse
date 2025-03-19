#pragma once
#include "weapons/Weapon.h"


class M1014 : public Weapon {

public:
    M1014();
    void CreateProjectile(std::vector<std::unique_ptr<Projectile>>& projectiles) override;
    void UpgradeWeapon() override;
};