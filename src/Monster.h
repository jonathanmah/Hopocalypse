#pragma once
#include "Character.h"
#include "AnimUtil.h"
#include "Projectile.h"
#include "AoE.h"
#include <SFML/Graphics.hpp>

class Player;


class Monster: public Character {
private:
    float timeSinceDeath;
    float deathDtSum;
    bool xAxisInverted;
    void HandleDeath(float deltaTime);
    
    
public:
    bool disabledMovement; // flag for dev

    Monster(AnimData animData, sf::Vector2f position, int health, float movementSpeed);
    bool Update(float deltaTime, std::vector<Player>& players, std::vector<std::unique_ptr<Projectile>>& projectiles, std::vector<Blood>& bloodSpray, std::vector<GroundBlood>& groundBlood, std::vector<std::unique_ptr<AoE>>& aoe);
    void Move(std::vector<Player>& players);
};