#pragma once
#include <SFML/Graphics.hpp>

class Hud {

private:
    void InterpolateHpBar();

public:
    sf::RectangleShape hpBar;
    sf::RectangleShape hpBarBackground; 
    sf::RectangleShape damageBar;
    std::vector<sf::RectangleShape> segmentLines;
    sf::Color healthColour;
    sf::Color damageColour;
    float currentHpWidth;
    float targetHpWidth;
    float damageHpWidth;
    int maxHp;

    Hud(int maxHp, sf::Color healthColour, sf::Color damageColour);
    void UpdateSegments(int maxHealth);
    void Update(int health, sf::FloatRect globalBounds);
    void Draw(sf::RenderWindow& window);
};