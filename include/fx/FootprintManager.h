#pragma once
#include <SFML/Graphics.hpp>

class Character;
class GameState;

class FootprintManager {

public:
    bool createLeftFootNext;
    float footprintDecayTimer;
    float footprintDtSumFrame;
    
    FootprintManager();
    sf::FloatRect GetFootCollider(Character* character);
    void UpdateFootprints(Character* character, sf::Vector2f nextMoveNormalized, GameState& state, float deltaTime);
};