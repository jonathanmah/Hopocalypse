#include "entities/monster/attack/Attack.h"
#include "entities/monster/Monster.h"

Attack::Attack(MonsterState attackState, Monster* monster, float cooldown, sf::FloatRect aggroBox) 
:
    attackState(attackState),
    monster(monster),
    aggroBox(aggroBox),
    cooldown(cooldown),
    cooldownTimer(0.f)
{}


