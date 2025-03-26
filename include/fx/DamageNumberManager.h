#pragma once
#include <SFML/Graphics.hpp>
#include "fx/DamageNumber.h"

enum class NumberType {
    REGULAR,
    CRITICAL,
    FIRE,
    HEAL
};

class DamageNumberManager { 

public:
    sf::RenderTexture renderTexture;
    std::vector<DamageNumber> damageNumbers;
    sf::Font regFont;
    sf::Font critFont;

    DamageNumberManager();

    void AddDamageNumber(sf::Vector2f position, int damage, NumberType type);

    void Update(float deltaTime);

    void Draw(sf::RenderWindow& window);
};