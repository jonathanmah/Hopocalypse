#pragma once
#include "weapons/Weapon.h"


class M9 : public Weapon {

public:
    M9();
    void CreateProjectile(std::vector<std::unique_ptr<Projectile>>& projectiles) override;
    void UpgradeWeapon() override;
};

class M9AtomBullet : public Projectile {

    public:
        M9AtomBullet(ProjectileData projectileData, sf::Vector2f position, sf::Vector2f normalized);
        void UpdateProjectileStatus(Character& character, std::vector<std::unique_ptr<Projectile>>& projectiles, 
            std::vector<std::unique_ptr<Projectile>>::iterator& it, std::vector<std::unique_ptr<AoE>>& aoe) override;
        void UpdateAnimation(float deltaTime) override;
    };