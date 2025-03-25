#include <iostream>
#include "entities/Monster.h"
#include "entities/Player.h"
#include "core/GameState.h"
#include "entities/effects/StatusEffect.h"
#include "entities/effects/OnFire.h"
#include "entities/effects/Knockback.h"

static int monster_count = 0;
static constexpr float DEATH_DT_SUM_PER_FRAME = 0.2f;
static constexpr int DEATH_ROTATE_PER_FRAME_DEG = 10;
static constexpr float DISAPPEAR_TIME = 6.f;

Monster::Monster(sf::Vector2f position, AnimData animData, int health, float movementSpeed, float scale, float xHitRatio, float yHitRatio) 
: Character(position, animData, health, movementSpeed, scale), 
    deathDtSum(0.f), 
    timeSinceDeath(0.f),
    xAxisInverted(false),
    disabledMovement(false),
    onFire(*this), // set to original address and then init after final version copied. and used
    paralyzed(*this),
    slowed(*this),
    knockback(*this),
    shrink(*this),
    xHitRatio(xHitRatio),
    yHitRatio(yHitRatio)
{
    sprite.setTextureRect(sf::IntRect(animData.textureFrame.position, animData.textureFrame.size));
    sprite.setPosition(position);
    sprite.setScale({scale, scale});
    sprite.setOrigin({sprite.getLocalBounds().size.x / 2, sprite.getLocalBounds().size.y / 2});
    monster_count++;
}

void Monster::UpdateStatusEffects(float deltaTime, sf::RenderWindow& window) {
    if(onFire.IsActive()){ // if it still has time left
        onFire.UpdateStatusEffect(deltaTime);
        std::cout << "FROM MONSTER ADDRESS : " << this << std::endl;
    }
    if(paralyzed.IsActive()){
        paralyzed.UpdateStatusEffect(deltaTime);
    } else {
        paralyzed.UpdateDisabledCd(deltaTime);
    }
    if(slowed.IsActive()) {
        slowed.UpdateStatusEffect(deltaTime);
    }
    if(shrink.IsActive()) {
        shrink.UpdateStatusEffect(deltaTime);
    }

}


void Monster::HandleDeath(float deltaTime) {
    deathDtSum += deltaTime;
    timeSinceDeath += deltaTime;
}

bool Monster::Update(GameState& state, float deltaTime){
    UpdateStatusEffects(deltaTime, state.window);
    UpdateHitbox();
    Monster::UpdateCollisions(state);
    hud.Update(health, hitbox);
    if(isAlive) {
        if(!paralyzed.IsActive()){
            AnimUtil::UpdateSpriteAnim(sprite, animData, deltaTime);
        }

        // check if certain state before moving towards players
        if(knockback.IsActive()){
            knockback.ApplyKnockback();
        } else {
            // HANDLE MOVEMENT, FROZEN, KNOCKBACK, DISTRACTED, PARLYZED
            if(!paralyzed.IsActive()){
                Monster::Move(state.players);
            }
        }
    } else {
        Monster::HandleDeath(deltaTime);
        if(timeSinceDeath > DISAPPEAR_TIME) {
            return true;
        }
    }
    CheckDeath();
    return false;
}

void Monster::Move(std::vector<Player>& players) {
    sf::Vector2f nextMove = players[0].GetPosition() - sprite.getPosition();
    if(nextMove.length() > 1.f){
        nextMove = nextMove.normalized()*(movementSpeed*slowed.slowFactor);
        if (nextMove.x < 0) {
            if(!xAxisInverted){
                sprite.setScale({-sprite.getScale().x, sprite.getScale().y});
                xAxisInverted = true;
            }
        }
        else if (xAxisInverted){
            sprite.setScale({-sprite.getScale().x, sprite.getScale().y});
            xAxisInverted = false;
        }
        if(players[0].isAlive){
            if(!disabledMovement)
                sprite.move(nextMove);
        }    
    }
    
}

void Monster::UpdateCollisions(GameState& state){
    for(auto it = state.projectiles.begin(); it != state.projectiles.end();){
        // DETECTS IF HIT HERE GLOBAL BOUNDS OF MONSTER VS GLOBAL BOUNDS OF PROJECTILE, NEED TO UPDATE.
        if((hitbox.findIntersection((*it)->GetGlobalBounds()) && isAlive)){
            // if projectile hasn't went through a monster hitbox yet
            if(!(*it)->HasHit(id)){
                if((*it)->createsBlood){
                    Blood::CreateProjectileBlood((*it)->GetPosition(), hitbox, state.bloodSpray, state.groundBlood);
                }
                TakeDamage((*it)->GetDamage());
            }
            // ADD NEW AOE HERE
            (*it)->UpdateProjectileStatus(*this, state, it);
        } else {
            ++it;
        }
    }
}

void Monster::DebugHitbox(GameState& state) {
    HitboxDebugger::DrawSpriteGlobalBoundsHitbox(state.window, sprite, sf::Color::Red);
    HitboxDebugger::DrawSpriteOrigin(state.window, sprite, sf::Color::Green);
    HitboxDebugger::DrawGlobalRect(state.window, hitbox, sf::Color::Magenta);
}

void Monster::UpdateHitbox() {
    sf::FloatRect origBounds = sprite.getGlobalBounds();
    float posX = origBounds.position.x + origBounds.size.x*xHitRatio;
    float posY = origBounds.position.y + origBounds.size.y*yHitRatio;
    float width = origBounds.size.x - (origBounds.size.x*xHitRatio*2.f);
    float height = origBounds.size.y*(1-yHitRatio);
    hitbox = {{posX, posY}, {width, height}};
}