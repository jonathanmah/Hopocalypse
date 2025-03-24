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

Monster::Monster(AnimData animData, sf::Vector2f position, int health, float movementSpeed) 
: Character(animData, position, health, movementSpeed), 
    deathDtSum(0.f), 
    timeSinceDeath(0.f),
    xAxisInverted(false),
    disabledMovement(false),
    onFire(this), // set to original address and then init after final version copied. and used
    paralyzed(this),
    slowed(this),
    knockback(this),
    shrink(this)
{
    monster_count++;
}

void Monster::InitPostFinalAddress() {
    onFire = OnFire{this};
    paralyzed = Paralyzed{this};
    slowed = Slowed{this};
    knockback = Knockback{this};
    shrink = Shrink{this};
}

void Monster::UpdateStatusEffects(float deltaTime, sf::RenderWindow& window) {
    if(onFire.IsActive()){ // if it still has time left
        onFire.UpdateStatusEffect(deltaTime);
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
    
    int targetDeg = 90 ? xAxisInverted : -90;

    if (deathDtSum < DEATH_DT_SUM_PER_FRAME){
        float currRotation = sprite.getRotation().asDegrees();
        if (xAxisInverted &&  currRotation < 89) { // bug with how sfml handles degrees and it's over rotating. idk. just use 89
            sprite.rotate(sf::degrees(DEATH_ROTATE_PER_FRAME_DEG));
        } else {
            if (currRotation == 0 || currRotation > 270) {
                sprite.rotate(sf::degrees(-DEATH_ROTATE_PER_FRAME_DEG));
            }
        }
    }
    deathDtSum += deltaTime;
    timeSinceDeath += deltaTime;
}

bool Monster::Update(GameState& state, float deltaTime){
    UpdateStatusEffects(deltaTime, state.window);
    Monster::UpdateCollisions(state);
    hud.Update(health, GetGlobalBounds());
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

