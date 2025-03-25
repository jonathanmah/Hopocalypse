#pragma once
#include <SFML/Graphics.hpp>
#include "entities/Monster.h"

class SmallDemon: public Monster {
public:
    SmallDemon(sf::Vector2f position);
    void InitAnimMap() override;
};