#pragma once
#include <SFML/Graphics.hpp>

class Hud {

private:
    void InterpolateHpBar();

public:
    sf::RectangleShape hpBar;
    float currentHpWidth;
    float targetHpWidth;
    Hud();
    void Update(int health, sf::FloatRect globalBounds);
    void Draw(sf::RenderWindow& window);
};