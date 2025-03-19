#pragma once
#include "weapons/Weapon.h"


class ScarH : public Weapon {

public:
    ScarH();
    void CreateProjectile(std::vector<std::unique_ptr<Projectile>>& projectiles) override;
    void UpgradeWeapon() override;
};