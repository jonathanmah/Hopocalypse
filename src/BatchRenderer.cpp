#include "BatchRenderer.h"
#include <iostream>

BatchRenderer::BatchRenderer(sf::RenderWindow& window) : window(window){
}
 
void BatchRenderer::addSpriteToBatch(const sf::Sprite& sprite) {
    sf::FloatRect bounds = sprite.getLocalBounds();
    sf::Transform transform = sprite.getTransform();
    sf::IntRect textureRect{sprite.getTextureRect()};

    // get the four corners of sprite and transform them
    sf::Vector2f topLeft = transform.transformPoint({bounds.position.x, bounds.position.y});
    sf::Vector2f bottomLeft = transform.transformPoint({bounds.position.x, bounds.position.y+bounds.size.y});
    sf::Vector2f topRight = transform.transformPoint({bounds.position.x+bounds.size.x, bounds.position.y});
    sf::Vector2f bottomRight = transform.transformPoint({bounds.position.x+bounds.size.x, bounds.position.y+bounds.size.y});
    // add verts to the vertex array
    sf::Color colour = sprite.getColor();
    triangles.emplace_back(sf::Vertex{topLeft, colour, {static_cast<float>(textureRect.position.x), static_cast<float>(textureRect.position.y)}});
    triangles.emplace_back(sf::Vertex{topRight, colour, {static_cast<float>(textureRect.position.x + textureRect.size.x), static_cast<float>(textureRect.position.y)}});
    triangles.emplace_back(sf::Vertex{bottomLeft, colour, {static_cast<float>(textureRect.position.x), static_cast<float>(textureRect.position.y + textureRect.size.y)}});
    
    triangles.emplace_back(sf::Vertex{bottomLeft, colour, {static_cast<float>(textureRect.position.x), static_cast<float>(textureRect.position.y + textureRect.size.y)}});
    triangles.emplace_back(sf::Vertex{bottomRight, colour, {static_cast<float>(textureRect.position.x + textureRect.size.x), static_cast<float>(textureRect.position.y + textureRect.size.y)}});
    triangles.emplace_back(sf::Vertex{topRight, colour, {static_cast<float>(textureRect.position.x + textureRect.size.x), static_cast<float>(textureRect.position.y)}});
}