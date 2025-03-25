#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>
#include <typeindex>
#include <typeinfo>
#include <map>

enum class MonsterE{
    LOW
};

class Character;
class Monster;
class Flame;

class BatchRenderer {
public:
    sf::RenderWindow& window;
    std::vector<sf::Vertex> triangles;
    sf::VertexArray flameTriangles;
    std::vector<sf::Vertex> hpBarTriangles;
    std::vector<sf::Vertex> onFireTriangles;
    std::vector<sf::Vertex> effectTriangles;

    std::map<MonsterE, std::vector<sf::Vertex>> monsterTriangles{
        {MonsterE::LOW, std::vector<sf::Vertex>{}},
    };

    BatchRenderer(sf::RenderWindow& window);

    void AddRectangleToBatch(const sf::RectangleShape& rectShape, std::vector<sf::Vertex>& rectTriangles);
    void AddStaticFrameToBatch(const sf::IntRect& textureFrame, std::array<sf::Vector2f, 4> cachedPosition, sf::Color colour);
    void AddSpriteToBatch(const sf::Sprite& sprite, std::vector<sf::Vertex>& vertices);


    template <typename T>
    void BatchRenderStaticFrames(std::vector<T>& frameWrapper){
        static int count = 0;
        triangles.clear();
        const sf::Texture* texture = nullptr;
        for (T& obj : frameWrapper) {
            if(texture == nullptr){
                texture = obj->GetTexture();
            }
            AddStaticFrameToBatch(obj->GetAnimData().textureFrame, obj->GetCachedVertices(), obj->GetColour());
        }
        window.draw(triangles.data(), triangles.size(), sf::PrimitiveType::Triangles, texture);
        count ++;
    }

    // pass a pointer object with sprite container member
    template <typename T>
    void BatchRenderSprites(std::vector<T>& spriteWrapper){
        // clear previous vertices
        triangles.clear();
        // add each sprites vertices to vertex array

        const sf::Texture* texture = nullptr;
        for (T& obj : spriteWrapper) {
            if(texture == nullptr){
                texture = &obj->GetSprite().getTexture();
            }
            AddSpriteToBatch(obj->GetSprite(), triangles);
        }
        // batch draw to frame
        window.draw(triangles.data(), triangles.size(), sf::PrimitiveType::Triangles, texture);
    }

    void ClearMonsterTriangles();
    void BatchRenderCharacters(std::vector<std::reference_wrapper<Character>>& characters);

    
    void SetFlameTriangles(std::vector<Flame>& flames, float initialRadius);
    void RenderFlameTriangles();
    void AppendOnFireTriangles(sf::Sprite* sprite);
};

