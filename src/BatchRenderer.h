#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>

class BatchRenderer {
private:
    sf::RenderWindow& window;
    std::vector<sf::Vertex> triangles;

    void addSpriteToBatch(const sf::Sprite& sprite);


public:
    BatchRenderer(sf::RenderWindow& window);

    template <typename T>
    void BatchRender(std::vector<T>& spriteWrapper){
        // clear previous vertices
        triangles.clear();
        // add each sprites vertices to vertex array

        const sf::Texture* texture = nullptr;
        for (T& obj : spriteWrapper) {
            if(texture == nullptr){
                texture = &obj.GetSprite().getTexture();
            }
            if (&obj.GetSprite().getTexture() != texture) {
                std::cerr << "Error All sprites in batch must use same texture" << std::endl;
                return;
            }
            addSpriteToBatch(obj.GetSprite());
        }
        // batch draw to frame
        window.draw(triangles.data(), triangles.size(), sf::PrimitiveType::Triangles, texture);
    }
};