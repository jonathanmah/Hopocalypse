#include "entities/monsters/Zombie.h"
#include "util/AnimUtil.h"

Zombie::Zombie(sf::Vector2f position) 
: 
    Monster(position, AnimUtil::ZombieAnim::walk, 500, 1.f, 1.7f, .45f, .49f)
{
    InitAnimMap();
    sprite.setTextureRect(sf::IntRect(animData.textureFrame.position, animData.textureFrame.size));
    sprite.setOrigin({animData.textureFrame.size.x*.5f, animData.textureFrame.size.y*.7f});
}

void Zombie::InitAnimMap() {
    animMap[MonsterState::WALK] = AnimUtil::ZombieAnim::walk;
    animMap[MonsterState::IDLE] = AnimUtil::ZombieAnim::idle;
    animMap[MonsterState::ATTACK1] = AnimUtil::ZombieAnim::attack;
    animMap[MonsterState::DEATH] = AnimUtil::ZombieAnim::death;
    animMap[MonsterState::DEATH].hangLastFrame = true;
}