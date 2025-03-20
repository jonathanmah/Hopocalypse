#pragma once
#include "weapons/Weapon.h"

class Monster;
class BatchRenderer;

class Flamethrower : public Weapon {

private:
    void ShootFlame();
    void FlameUpdate(std::vector<std::unique_ptr<Monster>>& monsters, float deltaTime);
    std::vector<std::reference_wrapper<Monster>> GetNeighbourMonsters(std::vector<std::unique_ptr<Monster>>& monsters);

public:
    std::vector<Flame> flames;


    Flamethrower();
    void Update(GameState& state, sf::Vector2f characterPosition, sf::Vector2f mousePosGlobal, float deltaTime) override;
    void CreateProjectile(std::vector<std::unique_ptr<Projectile>>& projectiles) override;
    void UpgradeWeapon() override;
    void UpdateFireEffects(float deltaTime);
    void Draw(sf::RenderWindow& window, BatchRenderer& BatchRenderer) override;
};