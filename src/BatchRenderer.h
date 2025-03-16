#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>

class BatchRenderer {
private:
    sf::RenderWindow& window;
    std::vector<sf::Vertex> triangles;

    void AddStaticFrameToBatch(const sf::IntRect& textureFrame, std::array<sf::Vector2f, 4> cachedPosition, sf::Color colour);
    void AddSpriteToBatch(const sf::Sprite& sprite);


public:
    BatchRenderer(sf::RenderWindow& window);

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

    // pass an object with sprite container member
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
            AddSpriteToBatch(obj->GetSprite());
        }
        // batch draw to frame
        window.draw(triangles.data(), triangles.size(), sf::PrimitiveType::Triangles, texture);
    }

};