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
    int damage,
    sf::Vector2f damageBoxOffset
)   :
    Attack(attackState, monster, cooldown, aggroBox, damageBox),
    damageFrame(damageFrame),
    damage(damage),
    damageBoxOffset(damageBoxOffset)
{}

// update aggro and damage box bounds 
void MeleeAttack::UpdateBoxBounds() {
    aggroBox.setPosition(monster->hitbox.position);
    damageBox.setPosition(monster->hitbox.position);
    if(!monster->xAxisInverted) {
        aggroBox.move({aggroBox.getSize().x + damageBoxOffset.x,damageBoxOffset.y});    
        damageBox.move({damageBox.getSize().x + damageBoxOffset.x, damageBoxOffset.y});
    } else {
        aggroBox.move({-aggroBox.getSize().x - damageBoxOffset.x, damageBoxOffset.y}); 
        damageBox.move({-damageBox.getSize().x - damageBoxOffset.x, damageBoxOffset.y});
    }
}

// #NOTE for testing, increase anim speed to long duration to slowly swap between frames
// apply damage once damage frame reached, reset state to walk if attack animation finished
void MeleeAttack::UpdateDuringAttack(std::vector<std::unique_ptr<Player>>& players, float deltaTime) {
    if(cooldownTimer <= 0.f && monster->animData.currFrame == damageFrame) {
        for(auto& player : players) {
            if(player->IsDead()) continue;
            if(damageBox.getGlobalBounds().findIntersection(player->GetGlobalBounds())){
                player->TakeDamage(damage);
            }
            SetAttackOnCooldown();
            dealt = true;
        }
    }
    if(monster->animData.currFrame == 0 && dealt) {
        monster->animData = monster->animMap[MonsterState::WALK];
        monster->animState = MonsterState::WALK;
    }
}

//set monster to an attacking state
void MeleeAttack::StartAttack(std::unique_ptr<Player>& player) {
    monster->animData = monster->animMap[attackState];
    monster->animState = attackState;
    dealt = false;
}

// players are in range of aggro box, and attack is not on cooldown
void MeleeAttack::CheckConditionsAndAttack(std::vector<std::unique_ptr<Player>>& players) {
    for(auto& player : players) {
        // if player is dead then continue
        if(player->IsDead()) continue;
        // if attack is not on cooldown and player in range of aggro box
        if(cooldownTimer <= 0.f && aggroBox.getGlobalBounds().findIntersection(player->GetGlobalBounds())) {
            // start attack
            StartAttack(player);
            break;
        }
    }
}

// draw aggro and damage bounds for debugging
void MeleeAttack::DrawAggroAndDamageBoxes(sf::RenderWindow& window) {
    HitboxDebugger::DrawGlobalRect(window, aggroBox.getGlobalBounds(), sf::Color::Green);
    HitboxDebugger::DrawGlobalRect(window, damageBox.getGlobalBounds(), sf::Color::Yellow);
    //HitboxDebugger::DrawCircle(window,monster->hitbox.getCenter(), 170, sf::Color::Black);
}