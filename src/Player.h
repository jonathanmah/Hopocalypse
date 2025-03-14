#pragma once
#include <SFML/Graphics.hpp>
#include "Character.h"
#include "AnimUtil.h"
#include "Monster.h"
#include "Projectile.h"
#include "Weapon.h"
#include "Footprint.h"

class Monster;

enum class PlayerState {
    WALK,
    HIT,
    DEATH,
    STAND
};

class Player: public Character{

private:
    PlayerState currState;
    std::unique_ptr<Weapon> currWeapon;
    float deathTimer;
    // relative to the character
    sf::Vector2f mousePosRelative;
    // global space
    sf::Vector2f mousePosGlobal;
    
    void HandleDeath(float deltaTime);
    void Move(PlayerState& state, std::vector<Footprint>& footprints, std::vector<GroundBlood>& groundBlood, float deltaTime);
    void CheckDeath(std::vector<Monster>& monster);
    void DrawHitbox(sf::RenderWindow& window) override;
    void SetMousePositions(sf::RenderWindow& window);
    void SetFacingDirection();
    void SetAnimDataByState(PlayerState newState);
    void CycleWeapons();
    

public:
    Player(AnimData animData, sf::Vector2f position); 
    void Update(float deltaTime, std::vector<Monster>& monsters, sf::RenderWindow& window,
         std::vector<Footprint>& footprints, std::vector<GroundBlood>& groundBlood, std::vector<std::unique_ptr<Projectile>>& projectiles);
    //void UpdateWeaponPosition(sf::RenderWindow& window, float deltaTime);
    void Draw(sf::RenderWindow& window) override;
};