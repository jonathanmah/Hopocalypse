#include "entities/monster/attack/Attack.h"
#include "entities/monster/Monster.h"

Attack::Attack(MonsterState attackState, Monster* monster, float cooldown, sf::FloatRect aggroBox, sf::FloatRect damageBox) 
:
    attackState(attackState),
    monster(monster),
    aggroBox(aggroBox.size),
    damageBox(damageBox.size),
    cooldown(cooldown),
    cooldownTimer(0.f)
{}