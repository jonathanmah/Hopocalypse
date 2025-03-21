#pragma once
#include "weapons/Weapon.h"


class M1014 : public Weapon {

public:
    M1014();
    void CreateProjectile(std::vector<std::unique_ptr<Projectile>>& projectiles) override;
    void UpgradeWeapon() override;
};

class M1014ShellUpgraded : public Projectile {

public:
    M1014ShellUpgraded(ProjectileData projectileData, sf::Vector2f position, sf::Vector2f normalized);
    void UpdateProjectileStatus(Character& character, std::vector<std::unique_ptr<Projectile>>& projectiles, 
        std::vector<std::unique_ptr<Projectile>>::iterator& it, std::vector<std::unique_ptr<AoE>>& aoe) override;


};