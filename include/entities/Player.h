#pragma once
#include <SFML/Graphics.hpp>
#include "Character.h"
#include "core/BatchRenderer.h"
#include "fx/FootprintManager.h"

class Weapon;
class Monster;

enum class PlayerState {
    WALK,
    SHOOT_WALK,
    SHOOT_IDLE,
    HIT,
    DEATH,
    IDLE
};

class Player: public Character{

public:
    PlayerState currState; // current State for animations
    std::unique_ptr<Weapon> currWeapon; // current weapon
    float deathTimer; // 
    float triggerHappy;
    
    sf::Vector2f mousePosRelative; // relative to the character
    sf::Vector2f mousePosGlobal;
    FootprintManager footprintManager;
    bool yOrderFirst;

    Player(sf::Vector2f position, AnimData animData); 
    
    void HandleDeath(float deltaTime);
    void Move(PlayerState& playerState, GameState& state, float deltaTime);
    void CheckDeath(std::vector<std::unique_ptr<Monster>>& monster);
    void DrawHitbox(sf::RenderWindow& window);
    void SetMousePositions(sf::RenderWindow& window);
    void SetFacingDirection();
    void SetAnimDataByState(PlayerState newState);
    void CycleWeapons();
    

    void Update(GameState& state, float deltaTime);
    void Draw(sf::RenderWindow& window, BatchRenderer& batchRenderer) override;
};