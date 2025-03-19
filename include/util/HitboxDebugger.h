#pragma once
#include <SFML/Graphics.hpp>

class HitboxDebugger {

public:
    static void DrawSpriteGlobalBoundsHitbox(sf::RenderWindow& window, const sf::Sprite& sprite, sf::Color colour = sf::Color::Red, float thickness = 2.0f);
    static void DrawSpriteOrigin(sf::RenderWindow& window, const sf::Sprite& sprite, sf::Color colour = sf::Color::White);
    static void DrawSpriteLocalBoundsHitbox(sf::RenderWindow& window, const sf::Sprite& sprite, sf::Color colour = sf::Color::Magenta, float thickness = 5.0f);
    static void DrawGlobalRect(sf::RenderWindow& window, const sf::FloatRect& rect, sf::Color colour = sf::Color::Yellow, float thickness = 2.0f);

};