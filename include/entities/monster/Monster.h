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
    OnFire onFire; // fire effect, damage over time
    Paralyzed paralyzed; // paralyzed effect, stunned and prevent movement
    Slowed slowed; // slowed effect, slowed movement speed
    Knockback knockback; // knockback effect, pushed back opposite direction from projectile
    Shrink shrink; // shrink effect, turned small and #TODO will decrease stats


    bool xAxisInverted; // true if monster is scaled to be facing Left
    bool disabledMovement; // flag to disable monster movement for dev
    float timeSinceDeath; // amount of time since monster died (used from removing dead bodies after xtime)
    float deathDtSum; // used for monsters that update (transformations/colour) outside from animation sequences

    sf::FloatRect hitbox; // a monsters hitbox for calculating projectile collisions
    float xHitRatio; // a ratio used for calculating hitbox from texture bounds. Ratio is x offset and 1-xoffset
    float yHitRatio; // a ratio used for calculating hitbox from texture bounds. y offset from top, no trimming from bottom.

    sf::RenderWindow* window; // #TODO delete later, used for drawing random things for debugging

    // constructor 
    Monster(sf::Vector2f position, AnimData animData, int health, float movementSpeed, float scale, float xHitRatio, float yHitratio, bool multiRowAnim = false);
    // destructor
    virtual ~Monster() = default;

    // CORE FUNCTIONS
    bool Update(GameState& state, float deltaTime); // main update entry point
    void Move(std::vector<std::unique_ptr<Player>>& players); // move logic
    void UpdateStateAndAnimation(MonsterState state); // used to change state and update animation
    void ProcessUpdates(GameState& state, float deltaTime); // update status effects, hitbox, collisions, attack logic, hp bar
    void UpdateStatusEffects(float deltaTime, sf::RenderWindow& window); // Update status effects, damage, timers
    void UpdateCollisions(GameState& state) override; // update hp from projectile collisions

    // pure virtual
    virtual void InitAttackMap() = 0; // state keys, attack values
    virtual void InitAnimMap() = 0; // state keys, animation values

    // virtual
    virtual sf::FloatRect UpdateHitbox(); // update a monsters hitbox position
    virtual void UpdateCurrentAnimation(float deltaTime); // update a monsters animation
    virtual void ResetDefaultMovementAnim(); // reset state/animation to default movement
    virtual void UpdateDead(float deltaTime); // update death state, animation, and timers
    virtual void HandleAttacks(GameState& state, float deltaTime); // handle attack logic, to start, during, attack cooldowns
    virtual bool HasAttacksOnCooldown(); // returns true if attacks are on cooldown

    //virtuals where default is nothing, with certain derived implementations overriding in main update
    //probably terrible practice and should change later
    // #NOTE not creating an outer function for now because still have 4 monsters left to implement and im sure 
    // the update loop will keep changing. will refactor later.
    virtual void BaseUpdates(sf::Vector2f normalized) {return;} // called in Move()
    virtual void HandleKnockback() {return;} // called when applying knockback

    bool HasNoDisablingStatusEffects(bool apply); //returns true if no status effects block movement, set apply flag to apply any effects
    bool IsNextToPlayer(std::vector<std::unique_ptr<Player>>& players);// returns true if monster collides with player
    bool IsIdle(std::vector<std::unique_ptr<Player>>& players, float deltaTime); // returns true if should be Idle, and handles Idle logic for setting
    void UpdateFacingDirection(sf::Vector2f nextMove); // updates the monsters facing direction and sets xAxisIsInverted based on the next move
    bool IsReadyForDeletion(); // returns true if monsters body is ready to be removed
    bool IsAttacking(); // returns true if monster is currently in attack state
    void DebugHitbox(GameState& state); // when MONSTER_HITBOX = 1, draw all monster related bounds
};