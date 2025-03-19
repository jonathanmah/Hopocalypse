#include <iostream>
#include "entities/Monster.h"
#include "entities/Player.h"
#include "core/GameState.h"


static int monster_count = 0;
static constexpr float DEATH_DT_SUM_PER_FRAME = 0.2f;
static constexpr int DEATH_ROTATE_PER_FRAME_DEG = 10;
static constexpr float DISAPPEAR_TIME = 6.f;

Monster::Monster(AnimData animData, sf::Vector2f position, int health, float movementSpeed) 
: Character(animData, position, health, movementSpeed), deathDtSum(0.f), 
timeSinceDeath(0.f), xAxisInverted(false), disabledMovement(false) {
    monster_count++;
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
    Monster::UpdateCollisions(state);
    hud.Update(health, GetGlobalBounds());
    if(isAlive) {
        AnimUtil::UpdateSpriteAnim(sprite, animData, deltaTime);
        Monster::Move(state.players);
    } else {
        Monster::HandleDeath(deltaTime);
        if(timeSinceDeath > DISAPPEAR_TIME) {
            return true;
        }
    }
    return false;
}

void Monster::Move(std::vector<Player>& players) {
    sf::Vector2f nextMove = players[0].GetPosition() - sprite.getPosition();
    if(nextMove.length() > 1.f){
        nextMove = nextMove.normalized()*Monster::movementSpeed;
        if (nextMove.x < 0) {
            if(!xAxisInverted){
                sprite.setScale({-scale, scale});
                xAxisInverted = true;
            }
        }
        else if (xAxisInverted){
            sprite.setScale({scale, scale});
            xAxisInverted = false;
        }
        if(players[0].isAlive){
            if(!disabledMovement)
                sprite.move(nextMove);
        }    
    }
    
}