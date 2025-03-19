#pragma once
#include <SFML/Graphics.hpp>

namespace MapBounds{
    inline constexpr int LEFT = 65;
    inline constexpr int RIGHT = 1135;
    inline constexpr int TOP = 90;
    inline constexpr int BOTTOM = 600;
}

class Map {

private:
    sf::Texture& texture;

    int tileWidth;
    int tileHeight;
    int scaleFactor;
    sf::VertexArray vertexArr;
    float deltaTimeSum;
    float animSpeed;
    int lavaFrame;

    void SetTextureCoordinates(int tileId, sf::Vertex* triangles);

public:
    Map(sf::Texture& texture);
    void Load();
    void Update(float deltaTime);
    void Draw(sf::RenderWindow& window);
};