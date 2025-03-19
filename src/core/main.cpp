#include <iostream>
#include <SFML/Graphics.hpp>
#include "core/GameState.h"

int main() {
    GameState game;
    // clock for tracking elapsed time, begins with default constructor
    sf::Clock clock;

    // Enter Game loop (capped at 60fps, windows.display() holds execution)
    while (game.window.isOpen()) {

        // delta time represents the time elapsed since the last frame was rendered 
        sf::Time delta = clock.restart();
        float deltaTime = delta.asSeconds();

        // this is for polling events like key presses and pushes onto a queue
        // it processes one event at a time and is slow for holding keys, just using for reading window close for now
        while (const std::optional event = game.window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
                game.window.close();
        }
        game.Update(deltaTime);
        game.Render();
    }

    return 0;
}