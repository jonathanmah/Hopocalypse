#pragma once
#include <SFML/Graphics.hpp>

class Shell {

private:
    sf::Vector2f CreateVelocity(sf::Vector2f weaponRelative);

public: 

    sf::RectangleShape shape;
    sf::Vector2f velocity;
    sf::Vector2f weaponRelative;

    float rotationSpeed;
    sf::Angle angle;
    float fallDistSum;
    float deltaTimeSum;

    Shell(sf::Vector2f position,sf::Vector2f weaponRelative);
    
    bool Update(float deltaTime);

    void Draw(sf::RenderWindow& window);
};