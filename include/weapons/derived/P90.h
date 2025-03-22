#pragma once
#include "weapons/Weapon.h"

class Character;

class P90 : public Weapon {

public:
    P90();
    void CreateProjectile(Player& player, std::vector<std::unique_ptr<Projectile>>& projectiles) override;
    void UpgradeWeapon() override;
};

class P90HealBullet : public Projectile {

    Character& source;

    public:
        P90HealBullet(ProjectileData projectileData, sf::Vector2f position, sf::Vector2f normalized, Character& source);
        void UpdateProjectileStatus(Character& character, GameState& state,
            std::vector<std::unique_ptr<Projectile>>::iterator& it) override;
    };