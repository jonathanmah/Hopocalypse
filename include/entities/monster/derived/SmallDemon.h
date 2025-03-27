#pragma once
#include <SFML/Graphics.hpp>
#include "entities/monster/Monster.h"

class SmallDemon: public Monster {
public:
    SmallDemon(sf::Vector2f position);
    void InitAttackMap() override;
    void InitAnimMap() override;
    void HandleAttacks(GameState& state, float deltaTime) override;
};