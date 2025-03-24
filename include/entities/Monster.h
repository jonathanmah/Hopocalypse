#pragma once
#include <SFML/Graphics.hpp>
#include "Character.h"
#include "entities/effects/StatusEffect.h"
#include "entities/effects/OnFire.h"
#include "entities/effects/Paralyzed.h"
#include "entities/effects/Slowed.h"
#include "entities/effects/Knockback.h"
#include "entities/effects/Shrink.h"

class Player;

enum class MonsterState {
    WALK,
    RUN,
    ATTACK1,
    ATTACK2,
    IDLE,
    DEATH,
};

class Monster: public Character {

private:
    float timeSinceDeath;
    float deathDtSum;
    
    void HandleDeath(float deltaTime);
    
    
public:

    // STATUS EFFECTS
    OnFire onFire;
    Paralyzed paralyzed;
    Slowed slowed;
    Knockback knockback;
    Shrink shrink;

    bool xAxisInverted;
    bool disabledMovement; // flag for dev

    std::unordered_map<MonsterState, AnimData> animMap;

    Monster(AnimData animData, sf::Vector2f position, int health, float movementSpeed);
    void UpdateStatusEffects(float deltaTime, sf::RenderWindow& window);
    virtual void InitAnimMap() {return;};
    bool Update(GameState& state, float deltaTime);
    void Move(std::vector<Player>& players);
    void InitPostFinalAddress();
    
};