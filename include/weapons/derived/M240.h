#pragma once
#include "weapons/Weapon.h"



class M240 : public Weapon {

public:

    sf::Sprite glowSprite;
    float firingStreak;

    M240(Player& player);
    void CreateProjectile(Player& player, std::vector<std::unique_ptr<Projectile>>& projectiles) override;
    void UpgradeWeapon() override;
    void Update(GameState& state, Player& player, sf::Vector2f mousePosGlobal, float deltaTime) override;
    void ActivateFiringEffects();
    void Draw(sf::RenderWindow& window, BatchRenderer& batchRenderer) override;
};