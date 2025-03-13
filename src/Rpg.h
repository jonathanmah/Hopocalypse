#pragma once
#include "Weapon.h"

class Rpg : public Weapon {



public:
    Rpg();
    void CreateProjectile(std::vector<std::unique_ptr<Projectile>>& projectiles) override;
    void UpdateBase(sf::Vector2f characterPosition, float deltaTime) override;
    
    virtual void Draw(sf::RenderWindow& window) override;
};