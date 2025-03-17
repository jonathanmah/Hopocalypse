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

void HitboxDebugger::DrawSpriteOrigin(sf::RenderWindow& window, const sf::Sprite& sprite, sf::Color colour){
    sf::CircleShape shape(5.f);
    shape.setFillColor(colour);
    
    shape.setPosition(sprite.getPosition());
    window.draw(shape);
}

void HitboxDebugger::DrawGlobalRect(sf::RenderWindow& window, const sf::FloatRect& rect, sf::Color colour, float thickness) {
    sf::RectangleShape rectShape{{rect.size.x,rect.size.y}};
    rectShape.setPosition({rect.position.x,rect.position.y});
    rectShape.setFillColor(sf::Color::Transparent);
    rectShape.setOutlineColor(colour);
    rectShape.setOutlineThickness(thickness);
    window.draw(rectShape);
    sf::CircleShape shape(5.f);
    shape.setFillColor(sf::Color::White);
    shape.setPosition(rectShape.getPosition());
    window.draw(shape);
    
}