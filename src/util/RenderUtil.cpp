#include "util/RenderUtil.h"

std::array<sf::Vector2f, 4> RenderUtil::CalculatePositionVertices(const sf::IntRect& textureFrame, const sf::Transform& transform) {
    return {
        transform.transformPoint({0, 0}), //top left
        transform.transformPoint({static_cast<float>(textureFrame.size.x), 0}), // top right
        transform.transformPoint({0, static_cast<float>(textureFrame.size.y)}), // bottom left
        transform.transformPoint({static_cast<float>(textureFrame.size.x), static_cast<float>(textureFrame.size.y)}) //bottom right
    };
}

