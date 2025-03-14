#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>

class BatchRenderer {
private:
    sf::RenderWindow& window;
    std::vector<sf::Vertex> triangles;


    void addFrameToBatch(const sf::IntRect& textureFrame, const sf::Transform& transform, sf::Color colour);
    void addSpriteToBatch(const sf::Sprite& sprite);


public:
    BatchRenderer(sf::RenderWindow& window);

    template <typename T>
    void BatchRenderFrames(std::vector<T>& frameWrapper){
        triangles.clear();
        const sf::Texture* texture = nullptr;
        for (T& obj : frameWrapper) {
            if(texture == nullptr){
                texture = obj.GetTexture();
            }
            addFrameToBatch(obj.GetAnimData().textureFrame, obj.getTransform(), obj.GetColour());
        }
        // batch draw to frame
        window.draw(triangles.data(), triangles.size(), sf::PrimitiveType::Triangles, texture);
    }

    template <typename T>
    void BatchRenderSprites(std::vector<T>& spriteWrapper){
        // clear previous vertices
        triangles.clear();
        // add each sprites vertices to vertex array

        const sf::Texture* texture = nullptr;
        for (T& obj : spriteWrapper) {
            if(texture == nullptr){
                texture = &obj.GetSprite().getTexture();
            }
            addSpriteToBatch(obj.GetSprite());
        }
        // batch draw to frame
        window.draw(triangles.data(), triangles.size(), sf::PrimitiveType::Triangles, texture);
    }
};