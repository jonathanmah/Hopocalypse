#pragma once
#include <SFML/Graphics.hpp>
#include "Character.h"

class Weapon;
class Monster;

enum class PlayerState {
    WALK,
    SHOOTING_WALK,
    SHOOTING_STAND,
    HIT,
    DEATH,
    STAND
};

class Player: public Character{

private:
    PlayerState currState;
    std::unique_ptr<Weapon> currWeapon;
    float deathTimer;
    float triggerHappy;
    // relative to the character
    sf::Vector2f mousePosRelative;
    // global space
    sf::Vector2f mousePosGlobal;
    
    void HandleDeath(float deltaTime);
    void Move(PlayerState& playerState, GameState& state, float deltaTime);
    void CheckDeath(std::vector<Monster>& monster);
    void DrawHitbox(sf::RenderWindow& window) override;
    void SetMousePositions(sf::RenderWindow& window);
    void SetFacingDirection();
    void SetAnimDataByState(PlayerState newState);
    void CycleWeapons();
    

public:
    Player(AnimData animData, sf::Vector2f position); 
    void Update(GameState& state, float deltaTime);
    void Draw(sf::RenderWindow& window) override;
};