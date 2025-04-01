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

    MonsterState attackState; // attack state representing an attack
    Monster* monster; // monster owner of attack
    AnimData animData; // animation of a monster while attacking, if currframe = a frame of the attack, then create the damage box for melee.
    sf::RectangleShape aggroBox; // represents range required for a monster to start attack
    sf::RectangleShape damageBox; // represents range required for a monster to start attack
    float cooldown; //cooldown after an attack
    float cooldownTimer; // current cooldown timer
    sf::Vector2f targetPosition; // can set a target position if necessary in StartAttack()
    sf::Vector2f normalizedDir; // can set a normalized direction for a projectile or jump attack in StartAttack()
    
    Attack(MonsterState attackState, Monster* monster, float cooldown, sf::FloatRect aggroBox, sf::FloatRect damageBox);
    virtual ~Attack() = default;

    // set monster to an attacking state, and set any other variables related to initial target position
    virtual void StartAttack(std::unique_ptr<Player>& player) = 0;

    // update the aggro box and damage box bounds 
    virtual void UpdateBoxBounds() = 0;

    // apply damage once damage frame reached, reset state to walk if attack animation finished
    virtual void UpdateDuringAttack(std::vector<std::unique_ptr<Player>>& players, float deltaTime) = 0;
    
    // players are in range of aggro box, and attack is not on cooldown
    virtual void CheckConditionsAndAttack(std::vector<std::unique_ptr<Player>>& players) = 0;

    // draw aggro and damage bounds for debugging
    virtual void DrawAggroAndDamageBoxes(sf::RenderWindow& window) = 0;

    // set attack on cooldown
    void SetAttackOnCooldown() {cooldownTimer = cooldown;}

    // returns true if attack on cooldown
    bool IsOnCooldown() {return cooldownTimer  > 0.f;}

};