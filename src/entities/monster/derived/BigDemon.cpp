#include "entities/monster/derived/BigDemon.h"
#include "util/AnimUtil.h"
#include "entities/monster/attack/MeleeAttack.h"

BigDemon::BigDemon(sf::Vector2f position) 
: 
    Monster(position, AnimUtil::BigDemonAnim::walk, 3000, 1.2f, 1.7f, .4f, .35f)
{
    InitAnimMap();
    sprite.setTextureRect(sf::IntRect(animData.textureFrame.position, animData.textureFrame.size));
    sprite.setOrigin({animData.textureFrame.size.x*.5f, animData.textureFrame.size.y*.6f});
}

void BigDemon::HandleAttacks(GameState& state, float deltaTime) {
    return;
}

void BigDemon::InitAttackMap() {
    attackMap[MonsterState::ATTACK1] = std::make_unique<MeleeAttack>(MonsterState::ATTACK1, this, 5.f, hitbox, hitbox, 2, 20);
}

void BigDemon::InitAnimMap() {
    animMap[MonsterState::WALK] = AnimUtil::BigDemonAnim::walk;
    animMap[MonsterState::IDLE] = AnimUtil::BigDemonAnim::idle;
    animMap[MonsterState::ATTACK1] = AnimUtil::BigDemonAnim::attack;
    animMap[MonsterState::DEATH] = AnimUtil::BigDemonAnim::death;
    animMap[MonsterState::DEATH].hangLastFrame = true;
}
