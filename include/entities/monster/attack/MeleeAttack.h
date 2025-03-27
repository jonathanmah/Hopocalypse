#pragma once 
#include <SFML/Graphics.hpp>
#include "entities/monster/attack/Attack.h"

class MeleeAttack : public Attack {

public:
    sf::FloatRect damageBox;
    int damageFrame;
    int damage;

    // keep in mind monster has the animData
    MeleeAttack(MonsterState attackState, Monster* monster, float cooldown, sf::FloatRect aggroBox,
        sf::FloatRect damageBox, int damageFrame, int damage
    );
    void UpdateBoxBounds() override;

    // 1. set monster to an attacking state, skip Update Move if this is true in monster update loop
    void StartAttack() override;
    // update everything except for monster animation
    virtual void UpdateDuringAttack(std::vector<std::unique_ptr<Player>>& players, float deltaTime) override;
    // players are in range of aggro box, and attack is not on cooldown
    void CheckConditionsAndAttack(std::vector<std::unique_ptr<Player>>& players) override;

    void DrawAggroAndDamageBoxes(sf::RenderWindow& window) override;
};