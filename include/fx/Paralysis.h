#pragma once
#include <SFML/Graphics.hpp>
#include "StatusEffect.h"

class Character;

class Paralysis : public StatusEffect {

public:
    Paralysis(AnimData animData, sf::Vector2f position, Character& character);
    bool Update(float deltaTime) override;
};