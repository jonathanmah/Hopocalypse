#pragma once
#include <SFML/Graphics.hpp>
#include "entities/monster/Monster.h"
#include "entities/monster/attack/MeleeAttack.h"

class Wolf: public Monster {
public:
    bool freezeJaw; // freezeJaw flag, used to freeze animation mid jump with wolf jaw open
    float jumpCd; // cooldown time for wolf jump attack for resetting
    float jumpCdTimer; // current cooldown for wolf jump attack

    Wolf(sf::Vector2f position);
    void InitAttackMap() override; // set state to attack map, create attack with params
    void InitAnimMap() override; // set state to wolf anim map
    void HandleAttacks(GameState& state, float deltaTime) override; // handle attack logic, to start, during, attack cooldowns
    sf::FloatRect UpdateHitbox() override; // update wolf hitbox (override b/c different shape and not symmetric in frame)
    void UpdateDead(float deltaTime) override; // update death handling (override b/c had to create own death animation with manual rotation over time)
    void UpdateCurrentAnimation(float deltaTime) override; // update current animation (override b/c animation sequence on more than one row)
    void BaseUpdates(sf::Vector2f normalized) override; // Update transformations to base (override b/c wolf rotates towards players)
    void HandleKnockback() override; // extra handler when knocked back (override b/c of bug when knock back during jump attack caused infinite jump)
};

class WolfAttack : public MeleeAttack {
    sf::Vector2f normalizedDir; // direction for jump attack
    float jumpMovement; // movement speed of jump attack
    Player* playerToJump = nullptr; // player pointer to jump
public:
    WolfAttack(MonsterState attackState, Monster* monster, float cooldown, sf::FloatRect aggroBox,
        sf::FloatRect damageBox, int damageFrame, int damage
    );
    void UpdateBoxBounds() override; // update aggro/damage box (override b/c different bounds and rotates)
    void UpdateDuringAttack(std::vector<std::unique_ptr<Player>>& players, float deltaTime) override; // apply damage once damage frame reached, reset state to walk if attack animation finished
    void StartAttack(std::unique_ptr<Player>& player) override;
    void CheckConditionsAndAttack(std::vector<std::unique_ptr<Player>>& players) override;
    
};