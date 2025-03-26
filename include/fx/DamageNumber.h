#pragma once
#include <SFML/Graphics.hpp>

class DamageNumber {

public:
    sf::Text text;
    sf::Vector2f position;
    sf::Color colour;
    int damage;
    float fadeTime;
    float speed;

    DamageNumber(sf::Vector2f position, int damage, int size, sf::Color colour, sf::Font& font);
    void DrawToRenderTexture(sf::RenderTexture& renderTexture);
    void Update(float deltaTime);
    bool IsExpired();
};