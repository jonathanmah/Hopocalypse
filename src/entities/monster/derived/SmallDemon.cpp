#include "entities/monster/derived/SmallDemon.h"
#include "util/AnimUtil.h"
#include "entities/monster/attack/MeleeAttack.h"
#include "core/GameState.h"

SmallDemon::SmallDemon(sf::Vector2f position) 
: 
    Monster(position, AnimUtil::SmallDemonAnim::walk, 700, 1.5f, 1.7f, .45f, .55f)
{
    InitAnimMap();
    InitAttackMap();
    sprite.setTextureRect(sf::IntRect(animData.textureFrame.position, animData.textureFrame.size));
    sprite.setOrigin({animData.textureFrame.size.x*.5f, animData.textureFrame.size.y*.8f});
}

void SmallDemon::InitAttackMap() {
    auto attackBox = UpdateHitbox(); // use same pos as hitbox
    attackBox.size.y *= .8f;
    attackMap[MonsterState::ATTACK1] = 
        std::make_unique<MeleeAttack>(
            MonsterState::ATTACK1, 
            this, 
            1.f,  // cd 
            attackBox,
            attackBox,
            3,  // frame to dmg
            120 // dmg
        );
}

void SmallDemon::InitAnimMap() {
    animMap[MonsterState::WALK] = AnimUtil::SmallDemonAnim::walk;
    animMap[MonsterState::IDLE] = AnimUtil::SmallDemonAnim::idle;
    animMap[MonsterState::ATTACK1] = AnimUtil::SmallDemonAnim::attack;
    animMap[MonsterState::DEATH] = AnimUtil::SmallDemonAnim::death;
    animMap[MonsterState::DEATH].hangLastFrame = true;
}