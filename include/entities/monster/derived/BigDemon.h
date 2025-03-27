#pragma once
#include <SFML/Graphics.hpp>
#include "entities/monster/Monster.h"

class BigDemon: public Monster {
public:
    BigDemon(sf::Vector2f position);
    void InitAttackMap() override;
    void InitAnimMap() override;
    void HandleAttacks(GameState& state, float deltaTime) override;
};