#pragma once
#include <SFML/Graphics.hpp>

class HitboxDebugger {

public:
    static void DrawSpriteGlobalBoundsHitbox(sf::RenderWindow& window, const sf::Sprite& sprite, sf::Color colour = sf::Color::Red, float thickness = 2.0f);
    static void DrawSpriteOrigin(sf::RenderWindow& window, const sf::Sprite& sprite);
    static void DrawSpriteLocalBoundsHitbox(sf::RenderWindow& window, const sf::Sprite& sprite, sf::Color colour = sf::Color::Magenta, float thickness = 5.0f);


};