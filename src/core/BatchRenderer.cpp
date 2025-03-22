#include "core/BatchRenderer.h"
#include <iostream>
#include "weapons/Weapon.h"

BatchRenderer::BatchRenderer(sf::RenderWindow& window) : window(window), flameTriangles(sf::PrimitiveType::Triangles){
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


constexpr float CIRCLE_SEGMENTS = 50.f; // 50 orig

void BatchRenderer::SetFlameTriangles(std::vector<Flame>& flames, float initalRadius) {
    flameTriangles.clear();
    std::vector<sf::Vector2f> unitCircle;
    for (int i = 0; i <= CIRCLE_SEGMENTS; ++i) {
        // get radians of circle split evenly
        float angleInRadians = (i / CIRCLE_SEGMENTS) * 2 * M_PI; //2PI represent whole circle
        // unit circle so radius(hypotenuse) is 1, cos angle is x coord, sin is y coord
        unitCircle.push_back({std::cos(angleInRadians), std::sin(angleInRadians)});
    }
    for (const Flame& flame : flames) {
        sf::Vector2f center = flame.position;
        sf::Color colour = flame.colour;
        float radius = 10.f * flame.size;
        // create a circle made up of triangles, all have same center vertex, with the outer verts
        // made by scaling the unit circle with the given radius
        for (int i = 0; i < CIRCLE_SEGMENTS; ++i) {
            sf::Vector2f p1 = center + unitCircle[i] * radius;
            sf::Vector2f p2 = center + unitCircle[i+1] * radius;
            
            flameTriangles.append(sf::Vertex({center, colour}));    
            flameTriangles.append(sf::Vertex({p1, colour}));
            flameTriangles.append(sf::Vertex({p2, colour}));
        }
    }
    //window.draw(flameTriangles, sf::RenderStates(sf::BlendAdd));
}

void BatchRenderer::RenderFlameTriangles(){
    if(flameTriangles.getVertexCount() > 0){
        window.draw(flameTriangles, sf::RenderStates(sf::BlendAdd));
        flameTriangles.clear();
    }
}