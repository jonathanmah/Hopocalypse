#pragma once
#include "weapons/Weapon.h"


class Ump45 : public Weapon {

public:
    Ump45();
    void CreateProjectile(Player& player, std::vector<std::unique_ptr<Projectile>>& projectiles) override;
    void UpgradeWeapon() override;
};