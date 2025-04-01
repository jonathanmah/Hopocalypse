#include <iostream>
#include "util/HitboxDebugger.h"


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
    shape.setOrigin(shape.getGeometricCenter());
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
    sf::CircleShape shape(2.f);
    shape.setFillColor(colour);
    shape.setPosition(rectShape.getPosition());
    //window.draw(shape);
}

void HitboxDebugger::DrawCircle(sf::RenderWindow& window, sf::Vector2f position, const float radius, sf::Color colour, float thickness){
    sf::CircleShape circle(radius);
    circle.setFillColor(sf::Color::Transparent);
    //circle.setFillColor(colour);
    circle.setOutlineColor(colour);
    circle.setOutlineThickness(thickness);
    circle.setOrigin({radius, radius});
    circle.setPosition(position);
    window.draw(circle);
}


