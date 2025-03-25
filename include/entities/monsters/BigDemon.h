#pragma once
#include <SFML/Graphics.hpp>
#include "entities/Monster.h"

class BigDemon: public Monster {
public:
    BigDemon(sf::Vector2f position);
    void InitAnimMap() override;
};