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

void Zombie::InitAttackMap() {
    auto attackBox = UpdateHitbox(); // use same pos as hitbox
    attackBox.size.y *= .5f;
    attackMap[MonsterState::ATTACK1] = 
        std::make_unique<MeleeAttack>(
            MonsterState::ATTACK1, 
            this, 
            1.f, 
            attackBox,
            attackBox,
            2, 
            100
        );
}

void Zombie::InitAnimMap() {
    animMap[MonsterState::WALK] = AnimUtil::ZombieAnim::walk;
    animMap[MonsterState::IDLE] = AnimUtil::ZombieAnim::idle;
    animMap[MonsterState::ATTACK1] = AnimUtil::ZombieAnim::attack;
    animMap[MonsterState::DEATH] = AnimUtil::ZombieAnim::death;
    animMap[MonsterState::DEATH].hangLastFrame = true;
}