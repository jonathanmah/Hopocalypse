#include "HitboxDebugger.h"
#include <iostream>


void HitboxDebugger::DrawSpriteGlobalBoundsHitbox(sf::RenderWindow& window, const sf::Sprite& sprite, sf::Color colour, float thickness){
    sf::FloatRect bounds = sprite.getGlobalBounds();
    sf::RectangleShape rect(sf::Vector2f(bounds.size));
    rect.setPosition(bounds.position);
    rect.setFillColor(sf::Color::Transparent);
    rect.setOutlineColor(colour);
    rect.setOutlineThickness(thickness);
    window.draw(rect);
}
void HitboxDebugger::DrawSpriteLocalBoundsHitbox(sf::RenderWindow& window, const sf::Sprite& sprite, sf::Color colour, float thickness){
    sf::FloatRect bounds = sprite.getLocalBounds();
    sf::RectangleShape rect(sf::Vector2f(bounds.size));
    rect.setPosition(bounds.position);
    rect.setFillColor(sf::Color::Transparent);
    rect.setOutlineColor(colour);
    rect.setOutlineThickness(thickness);
    window.draw(rect);
}

void HitboxDebugger::DrawSpriteOrigin(sf::RenderWindow& window, const sf::Sprite& sprite){
    sf::CircleShape shape(5.f);
    shape.setFillColor(sf::Color(100, 250, 50));
    
    shape.setPosition(sprite.getPosition());
    window.draw(shape);
}