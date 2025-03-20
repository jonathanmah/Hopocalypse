#pragma once
#include <SFML/Graphics.hpp>
#include "Character.h"

class Player;

class Monster: public Character {

    private:
    float timeSinceDeath;
    float deathDtSum;
    bool xAxisInverted;
    void HandleDeath(float deltaTime);
    
    
public:
    bool disabledMovement; // flag for dev
    bool flameTagged;
    float flameTimer;
    bool burnt;

    Monster(AnimData animData, sf::Vector2f position, int health, float movementSpeed);
    bool Update(GameState& state, float deltaTime);
    void Move(std::vector<Player>& players);
};