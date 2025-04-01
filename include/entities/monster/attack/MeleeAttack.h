#pragma once 
#include <SFML/Graphics.hpp>
#include "entities/monster/attack/Attack.h"

class MeleeAttack : public Attack {

public:
    int damageFrame;
    int damage;
    sf::Vector2f damageBoxOffset;
    bool dealt;

    // keep in mind monster has the animData
    MeleeAttack(MonsterState attackState, Monster* monster, float cooldown, sf::FloatRect aggroBox,
        sf::FloatRect damageBox, int damageFrame, int damage, sf::Vector2f damageBoxOffset={0,0}
    );
    // update the aggro box and damage box bounds 
    virtual void UpdateBoxBounds() override;
    // set monster to an attacking state, and set any other variables related to initial target position
    virtual void StartAttack(std::unique_ptr<Player>& player) override;
    // apply damage once damage frame reached, reset state to walk if attack animation finished
    virtual void UpdateDuringAttack(std::vector<std::unique_ptr<Player>>& players, float deltaTime) override;
    // players are in range of aggro box, and attack is not on cooldown
    virtual void CheckConditionsAndAttack(std::vector<std::unique_ptr<Player>>& players) override;
    // draw aggro and damage bounds for debugging
    void DrawAggroAndDamageBoxes(sf::RenderWindow& window) override;
};