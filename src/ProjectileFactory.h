#pragma once
#include <SFML/Graphics.hpp>
#include "Weapon.h"
#include "Projectile.h"

class Projectile;
class Weapon;

class ProjectileFactory {
public:
    static std::unique_ptr<Projectile> CreateProjectile(Weapon* weapon, 
        sf::Vector2f characterPosition, sf::Vector2f mousePosRelative, sf::Vector2f mousePosGlobal);
};