#pragma once
#include <SFML/Graphics.hpp>
#include "StatusEffect.h"

class Character;

class OnFire : public StatusEffect {

public:
    OnFire(AnimData animData, sf::Vector2f position, Character& character);
    bool Update(float deltaTime) override;
};