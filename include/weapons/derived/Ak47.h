#pragma once
#include "weapons/Weapon.h"

class Ak47 : public Weapon {

public:
    Ak47();
    void CreateProjectile(Player& player, std::vector<std::unique_ptr<Projectile>>& projectiles) override;
    void UpgradeWeapon() override;
};