#pragma once
#include <SFML/Graphics.hpp>
#include "entities/Character.h"
#include "entities/effects/StatusEffect.h"
#include "entities/effects/OnFire.h"
#include "entities/effects/Paralyzed.h"
#include "entities/effects/Slowed.h"
#include "entities/effects/Knockback.h"
#include "entities/effects/Shrink.h"
#include "entities/monster/attack/Attack.h"

class Player;

enum class MonsterState {
    WALK,
    RUN,
    ATTACK1,
    ATTACK2,
    ATTACK3,
    IDLE,
    DEATH,
};

class Monster: public Character {
    
public:
    std::unordered_map<MonsterState, AnimData> animMap; // State - Animation map
    std::unordered_map<MonsterState, std::unique_ptr<Attack>> attackMap; // State - Attack map

    MonsterState animState;
    // STATUS EFFECTS
    OnFire onFire;
    Paralyzed paralyzed;
    Slowed slowed;
    Knockback knockback;
    Shrink shrink;

    bool xAxisInverted; // true if monster turns left
    bool disabledMovement; // flag for dev
    float timeSinceDeath; // 
    float deathDtSum;

    sf::FloatRect hitbox;
    float xHitRatio;
    float yHitRatio;
    
    



    Monster(sf::Vector2f position, AnimData animData, int health, float movementSpeed, float scale, float xHitRatio, float yHitratio);
    virtual ~Monster() = default;
    bool Update(GameState& state, float deltaTime);
    void Move(std::vector<std::unique_ptr<Player>>& players);

    void ProcessUpdates(GameState& state, float deltaTime);
    void DebugHitbox(GameState& state);
    void UpdateStatusEffects(float deltaTime, sf::RenderWindow& window);
    void UpdateHitbox();
    void UpdateCollisions(GameState& state) override;
    void UpdateCurrentAnimation(float deltaTime);
    bool IsFreeToMove(bool apply);
    bool IsReadyForDeletion();
    void UpdateFacingDirection(sf::Vector2f nextMove);

    virtual void HandleAttacks(GameState& state, float deltaTime) = 0;

    virtual void InitAttackMap() = 0;
    virtual void InitAnimMap() = 0; // acting as attack triggers

    void UpdateDead(float deltaTime);
    
    
    
};