#include "HitboxDebugger.h"
#include <iostream>


void HitboxDebugger::DrawSpriteGlobalBoundsHitbox(sf::RenderWindow& window, const sf::Sprite& sprite, sf::Color colour, float thickness){
    sf::FloatRect bounds = sprite.getGlobalBounds();
    std::cout << "Global Pos : " << bounds.position.x << ", " << bounds.position.y << std::endl;
    std::cout << "Global Width/Height: " << bounds.size.x << ", " << bounds.size.y << std::endl;
    sf::RectangleShape rect(sf::Vector2f(bounds.size));
    rect.setPosition(bounds.position);
    rect.setFillColor(sf::Color::Transparent);
    rect.setOutlineColor(colour);
    rect.setOutlineThickness(thickness);
    window.draw(rect);
}
void HitboxDebugger::DrawSpriteLocalBoundsHitbox(sf::RenderWindow& window, const sf::Sprite& sprite, sf::Color colour, float thickness){
    sf::FloatRect bounds = sprite.getLocalBounds();
    std::cout << "Local Pos : " << bounds.position.x << ", " << bounds.position.y << std::endl;
    std::cout << "Local Width/Height: " << bounds.size.x << ", " << bounds.size.y << std::endl;
    sf::RectangleShape rect(sf::Vector2f(bounds.size));
    rect.setPosition(bounds.position);
    rect.setFillColor(sf::Color::Transparent);
    rect.setOutlineColor(colour);
    rect.setOutlineThickness(thickness);
    window.draw(rect);
}

void HitboxDebugger::DrawSpriteOrigin(sf::RenderWindow& window, const sf::Sprite& sprite){
    sf::CircleShape shape(2.f);
    shape.setFillColor(sf::Color(100, 250, 50));
    
    shape.setPosition(sprite.getPosition());
    std::cout << "Origin Position: " << shape.getPosition().x << ","  << shape.getPosition().y << std::endl;
    window.draw(shape);
}