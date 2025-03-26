#include "entities/monsters/SmallDemon.h"
#include "util/AnimUtil.h"

SmallDemon::SmallDemon(sf::Vector2f position) 
: 
    Monster(position, AnimUtil::SmallDemonAnim::walk, 700, 1.5f, 1.7f, .45f, .55f)
{
    InitAnimMap();
    sprite.setTextureRect(sf::IntRect(animData.textureFrame.position, animData.textureFrame.size));
    sprite.setOrigin({animData.textureFrame.size.x*.5f, animData.textureFrame.size.y*.8f});
}

void SmallDemon::InitAnimMap() {
    animMap[MonsterState::WALK] = AnimUtil::SmallDemonAnim::walk;
    animMap[MonsterState::IDLE] = AnimUtil::SmallDemonAnim::idle;
    animMap[MonsterState::ATTACK1] = AnimUtil::SmallDemonAnim::attack;
    animMap[MonsterState::DEATH] = AnimUtil::SmallDemonAnim::death;
    animMap[MonsterState::DEATH].hangLastFrame = true;
}