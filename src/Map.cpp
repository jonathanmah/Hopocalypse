#include "Map.h"
#include <iostream>
#include <cstdint>


Map::Map(sf::Texture& texture) : texture(texture), tileWidth(16), tileHeight(16), scaleFactor(3.f), animSpeed(.1f), lavaFrame(0) {}

void Map::SetTextureCoordinates(int tileId, sf::Vertex* triangles) {

    static const std::unordered_map<int, sf::Vector2f> textureMapping = {
        {-1, {1, 4}}, {0, {1, 0.f}}, {1, {2, 0.f}}, {2, {3, 0.f}}, {3, {1, 1}}, 
        {7, {3.67, 8.5}}, {8, {1.32, 7.5}}, {9, {3.67, 7.5}}, {10, {0, 9.5}}, {11, {1.32, 8.5}}, 
        {12, {5, 0}}, {16, {0.97, 10.25}}, {17, {6.84, 16.75}}, {18, {0, 10.25}}, {19, {0, 11.25}}, 
        {20, {1, 0.75}}, {21, {2, 0.75}}, {22, {3, 0.75}}, {23, {2.5, 9.25}}, 
        {101, {5, 1}}, {102, {4, 1}}, {103, {6, 1}}, {104, {5, 2}}, {105, {4, 2}}, {106, {6, 2}}
    };
    std::unordered_map<int, sf::Vector2f>::const_iterator it = textureMapping.find(tileId); 
    float x, y;
    if (it != textureMapping.end()) {
        x = it->second.x;
        y = it->second.y;
    } else {
        std::string errMsg = "No texture mapping for tile id " + std::to_string(tileId);
        throw std::runtime_error(errMsg);
    }
    // map top left triangle
    triangles[0].texCoords = sf::Vector2f(x * tileWidth, y * tileHeight);  // Top-left unique
    triangles[1].texCoords = sf::Vector2f((x+1) * tileWidth, y * tileHeight);  // Top-right
    triangles[2].texCoords = sf::Vector2f(x * tileWidth, (y+1) * tileHeight);  // Bottom-left

    // map bottom right triangle
    triangles[3].texCoords = sf::Vector2f(x * tileWidth, (y+1) * tileHeight);  // Bottom-left
    triangles[4].texCoords = sf::Vector2f((x+1) * tileWidth, (y+1) * tileHeight); // Bottom-right unique
    triangles[5].texCoords = sf::Vector2f((x+1) * tileWidth, y * tileHeight); // Top-right
}

void Map::Load() {

    constexpr std::array level = {
        8, 0, 1, 2, 0, 1, 2, 0, 1, 2, 0, 1, 12, 0, 1, 2, 0, 1, 2, 0, 1, 2, 0, 1, 9,
        11, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 101, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 7,
        10, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 104, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 7,
        10, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 7,
        10, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 7,
        10, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 7,
        10, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 7,
        10, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 7,
        10, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 7,
        10, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 7,
        10, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 7,
        10, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 7,
        10, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 7,
        18, 20, 21, 22, 20, 21, 22, 20, 21, 22, 20, 21, 22, 20, 21, 22, 20, 21, 22, 20, 21, 22, 20, 21, 16, 
        19, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 17,
    };

    vertexArr.setPrimitiveType(sf::PrimitiveType::Triangles);
    vertexArr.resize(26*15*6);

    int tileWidthScaled = tileWidth*scaleFactor;
    int tileHeightScaled = tileHeight*scaleFactor;

    for (uint32_t i = 0; i < 25; ++i) { // 16 wide
        for (uint32_t j = 0; j < 15; ++j) { // 9 high
            sf::Vertex* triangles = &vertexArr[(i + j * 25) * 6];

            triangles[0].position = sf::Vector2f(i*tileWidthScaled, j*tileHeightScaled);
            triangles[1].position = sf::Vector2f((i+1)*tileWidthScaled, j*tileHeightScaled);
            triangles[2].position = sf::Vector2f(i*tileWidthScaled, (j+1)*tileHeightScaled);
            triangles[3].position = sf::Vector2f(i*tileWidthScaled, (j+1)*tileHeightScaled);
            triangles[4].position = sf::Vector2f((i+1)*tileWidthScaled, (j+1)*tileHeightScaled);
            triangles[5].position = sf::Vector2f((i+1)*tileWidthScaled, j*tileHeightScaled);

            SetTextureCoordinates(level[i + j*25], triangles);
        }
    }
    Map::deltaTimeSum = 0.f;
}

void Map::Update(float deltaTime) {
    Map::deltaTimeSum += deltaTime;

    if(Map::deltaTimeSum >= animSpeed) {
        sf::Vertex* mid_lava_triangles = &vertexArr[(12 + (1 * 25)) * 6];
        sf::Vertex* bot_lava_triangles = &vertexArr[(12 + (2 * 25)) * 6];

        switch(lavaFrame) {
            case 0: // set lava 1 mid and bot
                SetTextureCoordinates(101, mid_lava_triangles);
                SetTextureCoordinates(104, bot_lava_triangles);
                lavaFrame = 1;
                break;
            case 1: // set lava 2 mid and bot
                SetTextureCoordinates(102, mid_lava_triangles);
                SetTextureCoordinates(105, bot_lava_triangles);
                lavaFrame = 2;
                break;
            case 2: // set lava 3 mid and bot
                SetTextureCoordinates(103, mid_lava_triangles);
                SetTextureCoordinates(106, bot_lava_triangles);
                lavaFrame = 0;
                break;   
        }
        Map::deltaTimeSum = 0.f;
    }
}

void Map::Draw(sf::RenderWindow& window) {
    window.draw(vertexArr, &texture);
}