#pragma once
#include <SFML/Graphics.hpp>
#include "AoE.h"

class Freeze : public AoE {

public:
    Freeze(AnimData animData, sf::Vector2f position);
    bool Update(float deltaTime) override;
};