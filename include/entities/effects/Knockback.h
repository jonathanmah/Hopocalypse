#pragma once
#include <SFML/Graphics.hpp>

class Character;

class Knockback {

public:

    Character& character;
    float knockbackDebt;
    sf::Vector2f knockbackVector;

    Knockback(Character& character);

    inline bool IsActive() {return knockbackDebt > 0.f;}
    void SetKnockback(float newKnockbackDistance, sf::Vector2f newKnockbackVector);
    void ApplyKnockback();


};