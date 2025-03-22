#pragma once
#include "weapons/Weapon.h"


class ScarH : public Weapon {

public:
    ScarH();
    void CreateProjectile(Player& player, std::vector<std::unique_ptr<Projectile>>& projectiles) override;
    void UpgradeWeapon() override;
};

class ScarElectricBullet : public Projectile {

public:
    ScarElectricBullet(ProjectileData projectileData, sf::Vector2f position, sf::Vector2f normalized);
    void UpdateProjectileStatus(Character& character, GameState& state, 
        std::vector<std::unique_ptr<Projectile>>::iterator& it) override;
    void UpdateAnimation(float deltaTime) override;
};