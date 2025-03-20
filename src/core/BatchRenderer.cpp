#include "core/BatchRenderer.h"
#include <iostream>
#include "weapons/Weapon.h"

BatchRenderer::BatchRenderer(sf::RenderWindow& window) : window(window){
    triangles.reserve(200); // allocate for 200 sprites 
}

// no transformations to object, but animated, so texture frame coordinates and colour change but 
// global bounds never change
void BatchRenderer::AddStaticFrameToBatch(const sf::IntRect& textureFrame, std::array<sf::Vector2f, 4> cachedPosition, sf::Color colour) {
    triangles.emplace_back(sf::Vertex{cachedPosition[0], colour, {static_cast<float>(textureFrame.position.x), static_cast<float>(textureFrame.position.y)}}); // top left 
    triangles.emplace_back(sf::Vertex{cachedPosition[1], colour, {static_cast<float>(textureFrame.position.x + textureFrame.size.x), static_cast<float>(textureFrame.position.y)}}); // top right
    triangles.emplace_back(sf::Vertex{cachedPosition[2], colour, {static_cast<float>(textureFrame.position.x), static_cast<float>(textureFrame.position.y + textureFrame.size.y)}}); // bottom left
    
    triangles.emplace_back(sf::Vertex{cachedPosition[2], colour, {static_cast<float>(textureFrame.position.x), static_cast<float>(textureFrame.position.y + textureFrame.size.y)}}); // bottom left
    triangles.emplace_back(sf::Vertex{cachedPosition[3], colour, {static_cast<float>(textureFrame.position.x + textureFrame.size.x), static_cast<float>(textureFrame.position.y + textureFrame.size.y)}}); // bottom right
    triangles.emplace_back(sf::Vertex{cachedPosition[1], colour, {static_cast<float>(textureFrame.position.x + textureFrame.size.x), static_cast<float>(textureFrame.position.y)}}); // top right
}

void BatchRenderer::AddSpriteToBatch(const sf::Sprite& sprite) {
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

void BatchRenderer::AddRectangleToBatch(const sf::RectangleShape& rectShape, std::vector<sf::Vertex>& rectTriangles) {
    sf::FloatRect bounds = rectShape.getGlobalBounds();
    sf::Vector2f topLeft{bounds.position.x,bounds.position.y};
    sf::Vector2f topRight{bounds.position.x+bounds.size.x, bounds.position.y};
    sf::Vector2f bottomLeft{bounds.position.x, bounds.position.y+bounds.size.y};
    sf::Vector2f bottomRight{bounds.position.x+bounds.size.x,bounds.position.y+bounds.size.y};
    sf::Color colour = rectShape.getFillColor();
    rectTriangles.push_back(sf::Vertex({topLeft, colour}));
    rectTriangles.push_back(sf::Vertex({topRight, colour}));
    rectTriangles.push_back(sf::Vertex({bottomLeft, colour}));

    rectTriangles.push_back(sf::Vertex({bottomLeft, colour}));
    rectTriangles.push_back(sf::Vertex({bottomRight, colour}));
    rectTriangles.push_back(sf::Vertex({topRight, colour}));
}


constexpr int CIRCLE_SEGMENTS = 100;  // More segments = smoother circles

void BatchRenderer::BatchRenderFlames(sf::RenderWindow& window, std::vector<Flame>& flames) {
    sf::VertexArray vertices(sf::PrimitiveType::Triangles);
    std::vector<sf::Vector2f> unitCircle;
    for (int i = 0; i <= CIRCLE_SEGMENTS; ++i) {
        float angle = (i / static_cast<float>(CIRCLE_SEGMENTS)) * 2 * M_PI;
        unitCircle.push_back({std::cos(angle), std::sin(angle)});
    }
    for (const Flame& flame : flames) {
        sf::Vector2f center = flame.position;
        sf::Color colour = flame.colour;
        float radius = 10.f * flame.size;
        int startIndex = vertices.getVertexCount();
        vertices.append(sf::Vertex({center, colour}));
        for (int i = 0; i <= CIRCLE_SEGMENTS; ++i) {
            sf::Vector2f outerPos = center + unitCircle[i] * radius;
            vertices.append(sf::Vertex({outerPos, colour}));
            if (i > 0) {
                vertices.append(vertices[startIndex]);
                vertices.append(vertices[startIndex + i]);
                vertices.append(vertices[startIndex + i + 1]);
            }
        }
    }
    window.draw(vertices, sf::RenderStates(sf::BlendAdd));
}