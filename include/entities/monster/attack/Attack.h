// create this class to represent an attack

// has a float rect to represent a range in which the attack would be triggered

// float rect aggro box // used to initiate an attack
// float rect damage box // will inflict damage if intersected. (hide during certain frames if needed)
// only for melee attacks.  Range attacks will act same as projectiles

// cooldown

// bool ConditionsToAttackMet();
// then attack.

// BaseMovementDuringAttack move monster base if needed

#pragma once
#include <SFML/Graphics.hpp>
#include "util/AnimUtil.h"

class Monster;
class Player;
enum class MonsterState;

// this attack class will represent a monster attack.
// each monster will have a vector of attacks (Sorted by priority) and 
// if it is possible to attack, then they will.
// Start Attack will set the monster's anim data to the specific attack,
// and set State to Attack1, Attack2 etc.  
// and set attack state.
class Attack {
    
public:

    MonsterState attackState;
    Monster* monster;
    AnimData animData; // animation of a monster while attacking, if currframe = a frame of the attack, then create the damage box for melee.
    sf::FloatRect aggroBox; // represents range required for a monster to start attack
    float cooldown; //cooldown after an attack
    float cooldownTimer; // current cooldown
    
    Attack(MonsterState attackState, Monster* monster, float cooldown, sf::FloatRect aggroBox);
    virtual ~Attack() = default;
    // 1. set monster to an attacking state, skip Update Move if this is true in monster update loop
    virtual void StartAttack() = 0;

    virtual void UpdateBoxBounds() = 0;

    // update everything except for monster animation
    virtual void UpdateDuringAttack(std::vector<std::unique_ptr<Player>>& players, float deltaTime) = 0;
    
    // players are in range of aggro box, and attack is not on cooldown
    virtual void CheckConditionsAndAttack(std::vector<std::unique_ptr<Player>>& players) = 0;

    virtual void DrawAggroAndDamageBoxes(sf::RenderWindow& window) = 0;

    void SetAttackOnCooldown() {cooldownTimer = cooldown;}

    // attack is ready to be used
    bool IsOnCooldown() {return cooldownTimer  > 0.f;}

};