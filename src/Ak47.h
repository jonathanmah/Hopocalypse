#pragma once
#include "Weapon.h"


class Ak47 : public Weapon {

public:
    Ak47();
    void CreateProjectile(std::vector<std::unique_ptr<Projectile>>& projectiles) override;
};