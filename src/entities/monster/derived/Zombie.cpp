#include "entities/monster/derived/Zombie.h"
#include "util/AnimUtil.h"
#include "entities/monster/attack/MeleeAttack.h"
#include "core/GameState.h"

Zombie::Zombie(sf::Vector2f position) 
: 
    Monster(position, AnimUtil::ZombieAnim::walk, 500, 1.f, 1.7f, .45f, .49f)
{
    InitAnimMap();
    InitAttackMap();
    sprite.setTextureRect(sf::IntRect(animData.textureFrame.position, animData.textureFrame.size));
    sprite.setOrigin({animData.textureFrame.size.x*.5f, animData.textureFrame.size.y*.7f});
}

// this will always run if not dead.
void Zombie::HandleAttacks(GameState& state, float deltaTime) {
    attackMap[MonsterState::ATTACK1]->UpdateBoxBounds();
    if(attackMap[MonsterState::ATTACK1]->IsOnCooldown()){
        attackMap[MonsterState::ATTACK1]->cooldownTimer -= deltaTime;
        //std::cout << attackMap[MonsterState::ATTACK1]->cooldownTimer << std::endl;
    }
    if(animState == MonsterState::ATTACK1){
        attackMap[MonsterState::ATTACK1]->UpdateDuringAttack(state.players, deltaTime);
    } else {
        attackMap[MonsterState::ATTACK1]->CheckConditionsAndAttack(state.players);
    }
    attackMap[MonsterState::ATTACK1]->DrawAggroAndDamageBoxes(state.window);
}

void Zombie::InitAttackMap() {
    attackMap[MonsterState::ATTACK1] = std::make_unique<MeleeAttack>(MonsterState::ATTACK1, this, 1.f, hitbox, hitbox, 2, 100);
}

void Zombie::InitAnimMap() {
    animMap[MonsterState::WALK] = AnimUtil::ZombieAnim::walk;
    animMap[MonsterState::IDLE] = AnimUtil::ZombieAnim::idle;
    animMap[MonsterState::ATTACK1] = AnimUtil::ZombieAnim::attack;
    animMap[MonsterState::DEATH] = AnimUtil::ZombieAnim::death;
    animMap[MonsterState::DEATH].hangLastFrame = true;
}