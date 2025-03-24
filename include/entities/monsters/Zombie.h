#pragma once
#include <SFML/Graphics.hpp>
#include "entities/Monster.h"

class Zombie: public Monster {
public:
    Zombie(sf::Vector2f position);
    void InitAnimMap() override;
};