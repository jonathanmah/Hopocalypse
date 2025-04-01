#include "entities/monster/derived/BigDemon.h"
#include "util/AnimUtil.h"
#include "entities/monster/attack/MeleeAttack.h"
#include "core/GameState.h"


BigDemon::BigDemon(sf::Vector2f position) 
: 
    Monster(position, AnimUtil::BigDemonAnim::walk, 3000, 1.2f, 1.7f, .4f, .35f)
{
    InitAnimMap();
    InitAttackMap();
    sprite.setTextureRect(sf::IntRect(animData.textureFrame.position, animData.textureFrame.size));
    sprite.setOrigin({animData.textureFrame.size.x*.5f, animData.textureFrame.size.y*.6f});
}

void BigDemon::InitAttackMap() {
    auto attackBox = UpdateHitbox(); // use same pos as hitbox
    attackBox.position.y += .4f*attackBox.size.y;
    float yoffset = attackBox.size.y*.2f;
    attackBox.size.y *= .6f;
    attackBox.size.x *= 1.4f;
    attackMap[MonsterState::ATTACK1] = 
        std::make_unique<MeleeAttack>(
            MonsterState::ATTACK1, 
            this, 
            2.f,  // cd 
            attackBox,
            attackBox,
            3,  // frame to dmg
            0,//250, // dmg
            sf::Vector2f{0, yoffset}
        );
}

void BigDemon::InitAnimMap() {
    animMap[MonsterState::WALK] = AnimUtil::BigDemonAnim::walk;
    animMap[MonsterState::IDLE] = AnimUtil::BigDemonAnim::idle;
    animMap[MonsterState::ATTACK1] = AnimUtil::BigDemonAnim::attack;
    animMap[MonsterState::DEATH] = AnimUtil::BigDemonAnim::death;
    animMap[MonsterState::DEATH].hangLastFrame = true;
}