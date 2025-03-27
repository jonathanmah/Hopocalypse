#include <iostream>
#include "entities/monster/Monster.h"
#include "entities/Player.h"
#include "core/GameState.h"
#include "entities/effects/StatusEffect.h"
#include "entities/effects/OnFire.h"
#include "entities/effects/Knockback.h"

static int monster_count = 0;
static constexpr float DISAPPEAR_TIME = 12.f;

Monster::Monster(sf::Vector2f position, AnimData animData, int health, float movementSpeed, float scale, float xHitRatio, float yHitRatio) 
: Character(position, animData, sf::Color::Red, sf::Color::Yellow, health, movementSpeed, scale), 
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
    yHitRatio(yHitRatio),
    animState(MonsterState::WALK)
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


void Monster::UpdateDead(float deltaTime) {
    AnimUtil::UpdateSpriteAnim(sprite, animData, deltaTime);
    timeSinceDeath += deltaTime;
}

// update status effects, hitbox, collisions, check death, and update hp bar
void Monster::ProcessUpdates(GameState& state, float deltaTime) {
    // Update status effect effects, damage, timers
    UpdateStatusEffects(deltaTime, state.window);
    // update location of hitbox
    UpdateHitbox();
    // update damage from collisions by incoming projectiles 
    Monster::UpdateCollisions(state);
    // update character if 0 hp, set death animation on monster
    if(IsDead() && animState != MonsterState::DEATH){
        animData = animMap[MonsterState::DEATH];
        animState = MonsterState::DEATH;
    } else {
        if(IsFreeToMove(false)){ // check for disabling statuses (#TODO add more later) before attempting attack
            // handle attack logic excluding frame updates, animation updates will be done afterwards
            // what we can do here, is 
            // check if any pre-existing attacks have been started
            // if an existing attack is ongoing, then update it, else if check for all attacks
            // in priority order.
            // else means no pre-existing attacks have been started,
            // so loop through all attacks, and check if conditions have been met, player in range, cooldown etc.
            // if so, then start attack, set animState to ATTACK
            HandleAttacks(state, deltaTime);
        }
    }
    // update the location and display of the health bar, reflecting recent damage
    hud.Update(health, hitbox);
}

void Monster::UpdateCurrentAnimation(float deltaTime) {
    if(IsFreeToMove(false)){ // check can move without applying effects by passing false
        AnimUtil::UpdateSpriteAnim(sprite, animData, deltaTime);
    }
}

bool Monster::IsFreeToMove(bool apply) {
    if(knockback.IsActive()){
        if(apply) {
            knockback.ApplyKnockback();
        }
        return false;
    } else if (paralyzed.IsActive()) {
        return false;
    } 
    return true;
}

bool Monster::IsReadyForDeletion() {
    return timeSinceDeath > DISAPPEAR_TIME;
}

void Monster::UpdateFacingDirection(sf::Vector2f nextMove) {
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
}

bool Monster::Update(GameState& state, float deltaTime){
    if(!IsDead()) {
        // update status effects, hitbox, collisions, death logic, hp bar
        ProcessUpdates(state, deltaTime);
    
        // updates the anim data to be the next frame// INCLUDING FOR ATTACK
        // new animation assigned earlier in this loop, deltatimesum would be 0 so will play first frame
        UpdateCurrentAnimation(deltaTime);


        if(IsFreeToMove(true)){ // true flag will apply disabling effect eg. knockback
            //ALL ACTIONABLE LOGIC WILL GO HERE
            Move(state.players);
        }
        return false;
    } else {
        UpdateDead(deltaTime);
        return IsReadyForDeletion();
    }
}

void Monster::Move(std::vector<std::unique_ptr<Player>>& players) {
    // if any players are in range, move hitbox
    sf::Vector2f nextMove = players[0]->GetPosition() - sprite.getPosition();
    if(nextMove.length() > 1.f){
        nextMove = nextMove.normalized()*(movementSpeed*slowed.slowFactor);
        UpdateFacingDirection(nextMove);
        if(!players[0]->IsDead()){ 
            if(!disabledMovement){ //REMOVE THIS WHEN DONE TESTING
                sprite.move(nextMove);
            }
        }
    }
}

void Monster::UpdateCollisions(GameState& state){
    for(auto it = state.projectiles.begin(); it != state.projectiles.end();){
        // DETECTS IF HIT HERE GLOBAL BOUNDS OF MONSTER VS GLOBAL BOUNDS OF PROJECTILE, NEED TO UPDATE.
        if((hitbox.findIntersection((*it)->GetGlobalBounds()) && !IsDead())){
            // if projectile hasn't went through a monster hitbox yet
            if(!(*it)->HasHit(id)){
                if((*it)->createsBlood){
                    Blood::CreateProjectileBlood((*it)->GetPosition(), (*it)->sourcePosition, hitbox, state.bloodSpray, state.groundBlood);
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
    for (const auto& pair : attackMap) {
        pair.second->DrawAggroAndDamageBoxes(state.window);
    }
}

void Monster::UpdateHitbox() {
    sf::FloatRect origBounds = sprite.getGlobalBounds();
    float posX = origBounds.position.x + origBounds.size.x*xHitRatio;
    float posY = origBounds.position.y + origBounds.size.y*yHitRatio;
    float width = origBounds.size.x - (origBounds.size.x*xHitRatio*2.f);
    float height = origBounds.size.y*(1-yHitRatio);
    hitbox = {{posX, posY}, {width, height}};
}