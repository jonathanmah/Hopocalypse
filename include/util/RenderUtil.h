#pragma once
#include <SFML/Graphics.hpp>
#include "AnimUtil.h"

class RenderUtil {
    
public:
    static std::array<sf::Vector2f, 4> CalculatePositionVertices(const sf::IntRect& textureFrame, const sf::Transform& transform);
};