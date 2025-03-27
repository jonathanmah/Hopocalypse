#pragma once
#include <SFML/Graphics.hpp>
#include "entities/monster/Monster.h"

class Zombie: public Monster {
public:
    Zombie(sf::Vector2f position);
    void InitAttackMap() override;
    void InitAnimMap() override;
    void HandleAttacks(GameState& state, float deltaTime) override;
};