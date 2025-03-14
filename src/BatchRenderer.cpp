#include "BatchRenderer.h"
#include <iostream>

BatchRenderer::BatchRenderer(sf::RenderWindow& window) : window(window){
}

void BatchRenderer::addFrameToBatch(const sf::IntRect& textureFrame, const sf::Transform& transform, sf::Color colour) {
    
    sf::Vector2f topLeft = transform.transformPoint({0, 0});
    sf::Vector2f bottomLeft = transform.transformPoint({0, static_cast<float>(textureFrame.size.y)});
    sf::Vector2f topRight = transform.transformPoint({static_cast<float>(textureFrame.size.x), 0});
    sf::Vector2f bottomRight = transform.transformPoint({static_cast<float>(textureFrame.size.x), static_cast<float>(textureFrame.size.y)});

    // sf::VertexArray points(sf::PrimitiveType::Points, 4);
    // set points for corners
    // points[0] = sf::Vertex({topLeft, sf::Color::Red});     // Top left
    // points[1] = sf::Vertex({bottomLeft, sf::Color::Blue}); // Bottom left
    // points[2] = sf::Vertex({topRight, sf::Color::Green});  // Top right
    // points[3] = sf::Vertex({bottomRight, sf::Color::Yellow}); // Bottom right
    // window.draw(points);

    triangles.emplace_back(sf::Vertex{topLeft, colour, {static_cast<float>(textureFrame.position.x), static_cast<float>(textureFrame.position.y)}});
    triangles.emplace_back(sf::Vertex{topRight, colour, {static_cast<float>(textureFrame.position.x + textureFrame.size.x), static_cast<float>(textureFrame.position.y)}});
    triangles.emplace_back(sf::Vertex{bottomLeft, colour, {static_cast<float>(textureFrame.position.x), static_cast<float>(textureFrame.position.y + textureFrame.size.y)}});
    
    triangles.emplace_back(sf::Vertex{bottomLeft, colour, {static_cast<float>(textureFrame.position.x), static_cast<float>(textureFrame.position.y + textureFrame.size.y)}});
    triangles.emplace_back(sf::Vertex{bottomRight, colour, {static_cast<float>(textureFrame.position.x + textureFrame.size.x), static_cast<float>(textureFrame.position.y + textureFrame.size.y)}});
    triangles.emplace_back(sf::Vertex{topRight, colour, {static_cast<float>(textureFrame.position.x + textureFrame.size.x), static_cast<float>(textureFrame.position.y)}});
}

void BatchRenderer::addSpriteToBatch(const sf::Sprite& sprite) {
    sf::FloatRect bounds = sprite.getLocalBounds();
    sf::Transform transform = sprite.getTransform();
    sf::IntRect textureFrame{sprite.getTextureRect()};

    // get the four corners of sprite and transform them
    sf::Vector2f topLeft = transform.transformPoint({bounds.position.x, bounds.position.y});
    sf::Vector2f bottomLeft = transform.transformPoint({bounds.position.x, bounds.position.y+bounds.size.y});
    sf::Vector2f topRight = transform.transformPoint({bounds.position.x+bounds.size.x, bounds.position.y});
    sf::Vector2f bottomRight = transform.transformPoint({bounds.position.x+bounds.size.x, bounds.position.y+bounds.size.y});
    // add verts to the vertex array
    sf::Color colour = sprite.getColor();
    triangles.emplace_back(sf::Vertex{topLeft, colour, {static_cast<float>(textureFrame.position.x), static_cast<float>(textureFrame.position.y)}});
    triangles.emplace_back(sf::Vertex{topRight, colour, {static_cast<float>(textureFrame.position.x + textureFrame.size.x), static_cast<float>(textureFrame.position.y)}});
    triangles.emplace_back(sf::Vertex{bottomLeft, colour, {static_cast<float>(textureFrame.position.x), static_cast<float>(textureFrame.position.y + textureFrame.size.y)}});
    
    triangles.emplace_back(sf::Vertex{bottomLeft, colour, {static_cast<float>(textureFrame.position.x), static_cast<float>(textureFrame.position.y + textureFrame.size.y)}});
    triangles.emplace_back(sf::Vertex{bottomRight, colour, {static_cast<float>(textureFrame.position.x + textureFrame.size.x), static_cast<float>(textureFrame.position.y + textureFrame.size.y)}});
    triangles.emplace_back(sf::Vertex{topRight, colour, {static_cast<float>(textureFrame.position.x + textureFrame.size.x), static_cast<float>(textureFrame.position.y)}});
}