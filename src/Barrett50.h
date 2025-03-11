#pragma once
#include "Weapon.h"

class Barrett50 : public Weapon {

public:
    Barrett50();
    void CreateProjectile(std::vector<std::unique_ptr<Projectile>>& projectiles) override;
};
