#include "entities/monster/attack/MeleeAttack.h"
#include "entities/Player.h"
#include "entities/monster/Monster.h"
#include "util/HitboxDebugger.h"
#include <cmath>
static constexpr float MIRROR_NEG_Y_AXIS_BOUND = M_PI/2.f; // pi/2 radians , greater than half of pi
static constexpr float MIRROR_POS_Y_AXIS_BOUND =  -M_PI/2.f; // -pi/2 pi radians , less than -half of pi


MeleeAttack::MeleeAttack(
    MonsterState attackState,
    Monster* monster, 
    float cooldown, 
    sf::FloatRect aggroBox,
    sf::FloatRect damageBox, 
    int damageFrame,
    int damage
)   :
    Attack(attackState, monster, cooldown, aggroBox),
    damageBox(damageBox),
    damageFrame(damageFrame),
    damage(damage)
{}

void MeleeAttack::UpdateBoxBounds() {
    aggroBox = monster->hitbox;
    damageBox = monster->hitbox;
    if(!monster->xAxisInverted) {
        aggroBox.position.x += aggroBox.size.x;    
        damageBox.position.x += damageBox.size.x;
    } else {
        aggroBox.position.x -= aggroBox.size.x; 
        damageBox.position.x -= damageBox.size.x;
    }
    
}

// for testing, increase anim speed to long duration to slowly swap between frames
// update everything except for monster animation
void MeleeAttack::UpdateDuringAttack(std::vector<std::unique_ptr<Player>>& players, float deltaTime) {

    if(cooldownTimer <= 0.f && monster->animData.currFrame == damageFrame) {
        for(auto& player : players) {
            if(player->IsDead()) continue;
            if(damageBox.findIntersection(player->GetGlobalBounds())){
                player->TakeDamage(damage);
            }
            SetAttackOnCooldown();
        }
    }
    if(monster->animData.currFrame == monster->animData.totalFrames-1) {
        monster->animData = monster->animMap[MonsterState::WALK];
        monster->animState = MonsterState::WALK;
    }
    // for wolf, move monster towards player using sfml move sprite...
}

// 1. set monster to an attacking state, skip Update Move if this is true in monster update loop
void MeleeAttack::StartAttack() {
    monster->animData = monster->animMap[attackState];
    monster->animState = attackState;
}

// players are in range of aggro box, and attack is not on cooldown
void MeleeAttack::CheckConditionsAndAttack(std::vector<std::unique_ptr<Player>>& players) {
    for(auto& player : players) {
        // if player is dead then leave it
        if(player->IsDead()) continue;
        // if attack is not on cooldown and player in range
        if(cooldownTimer <= 0.f && aggroBox.findIntersection(player->GetGlobalBounds())) {
            // attack 
            StartAttack();
            break;
        }
    }
}

void MeleeAttack::DrawAggroAndDamageBoxes(sf::RenderWindow& window) {
    HitboxDebugger::DrawGlobalRect(window, aggroBox, sf::Color::Black);
    HitboxDebugger::DrawGlobalRect(window, damageBox, sf::Color::Yellow);
}