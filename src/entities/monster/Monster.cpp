#include <iostream>
#include "entities/monster/Monster.h"
#include "entities/Player.h"
#include "core/GameState.h"
#include "entities/effects/StatusEffect.h"
#include "entities/effects/OnFire.h"
#include "entities/effects/Knockback.h"

static int monster_count = 0;
static constexpr float DISAPPEAR_TIME = 12.f;

Monster::Monster(sf::Vector2f position, AnimData animData, int health, float movementSpeed, float scale, float xHitRatio, float yHitRatio, bool multiRowAnim) 
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

// returns true if the player should be idle
bool Monster::IsIdle(std::vector<std::unique_ptr<Player>>& players, float deltaTime) {
    // if attacks on cooldown and hitbox collides with player bounds, update to IDLE
    if((HasAttacksOnCooldown() && IsNextToPlayer(players))){
        UpdateStateAndAnimation(MonsterState::IDLE);
        return true;
    } else if(animState == MonsterState::IDLE && HasAttacksOnCooldown()) { 
        // if already IDLE from colliding while on cooldown, continue IDLE until cooldown finished
        return true;
    } else if(players[0]->IsDead()) {
        // if all players are dead, monster has no need to move, set IDLE
        UpdateStateAndAnimation(MonsterState::IDLE);
        return true;
    } else if(animState == MonsterState::IDLE && !HasAttacksOnCooldown()) {
        // if IDLE, and cooldowns have finished, reset state/animation to default movement
        ResetDefaultMovementAnim();
        return false;
    } else {
        return false;
    }
}

// resets a monsters state and animation to the monsters default movement
void Monster::ResetDefaultMovementAnim() {
    UpdateStateAndAnimation(MonsterState::WALK);
}

// returns true if monster is currently in attack state
bool Monster::IsAttacking() {
    return animState == MonsterState::ATTACK1 || animState == MonsterState::ATTACK2 || animState == MonsterState::ATTACK3;
}

// returns true if attacks are on cooldown
bool Monster::HasAttacksOnCooldown() {
    return attackMap[MonsterState::ATTACK1]->cooldownTimer > 0.f;
}

// returns true if monster hitbox collides with player global bounds
bool Monster::IsNextToPlayer(std::vector<std::unique_ptr<Player>>& players) {
    for(auto& player : players) {
        if(player->IsDead()) {
            continue;
        }
        if(hitbox.findIntersection(player->GetGlobalBounds())){
            return true;
        }
    }
    return false;
}

// updates damage, timers, effects from any status effects
void Monster::UpdateStatusEffects(float deltaTime, sf::RenderWindow& window) {
    if(onFire.IsActive()){
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

// Updates death state, animation, and timers
void Monster::UpdateDead(float deltaTime) {
    UpdateStateAndAnimation(MonsterState::DEATH);
    UpdateCurrentAnimation(deltaTime);
    timeSinceDeath += deltaTime;
}

// reset sequence to a new animation if it is not currently playing
void Monster::UpdateStateAndAnimation(MonsterState state) {
    if(animState != state){
        animData = animMap[state];
        animState = state;
        animData.currFrame = animData.initCurrFrame;
    }
}

// handle attack logic, attack cooldowns
void Monster::HandleAttacks(GameState& state, float deltaTime) {

    attackMap[MonsterState::ATTACK1]->UpdateBoxBounds(); // update damage box / aggro box bounds
    if(attackMap[MonsterState::ATTACK1]->IsOnCooldown()){
        // if attack is on cooldown, then update cooldown
        attackMap[MonsterState::ATTACK1]->cooldownTimer -= deltaTime;
    }
    if(animState == MonsterState::ATTACK1){
        // if currently attacking, then call attack update, 
        attackMap[MonsterState::ATTACK1]->UpdateDuringAttack(state.players, deltaTime);
    } else {
        // else, check if can attack
        attackMap[MonsterState::ATTACK1]->CheckConditionsAndAttack(state.players);
    }
    // draw aggro and damage boxes for debugging
   // attackMap[MonsterState::ATTACK1]->DrawAggroAndDamageBoxes(state.window);
}

// update status effects, hitbox, collisions, attack logic, hp bar
void Monster::ProcessUpdates(GameState& state, float deltaTime) {
    // update location of hitbox
    UpdateHitbox();
    // update damage from collisions by incoming projectiles 
    Monster::UpdateCollisions(state);
    if(HasNoDisablingStatusEffects(false)){ // if no disabling status effects
        // Handle attack virtual, more complicated monsters override
        HandleAttacks(state, deltaTime); // handle attack logic (except updating frames)
    }
    // update the location and display of the health bar, reflecting recent damage
    hud.Update(health, hitbox);
}

// Updates current animation, override in derived if sequence is split on multiple rows in texture.
// this works for one row sequences only
void Monster::UpdateCurrentAnimation(float deltaTime) {
    if(HasNoDisablingStatusEffects(false)){ // check if monster can move without applying status effects
        AnimUtil::UpdateSpriteAnim(sprite, animData, deltaTime);
    }
}

// returns true if no status effects block movement, set apply flag to apply any effects
bool Monster::HasNoDisablingStatusEffects(bool apply) {
    if(knockback.IsActive()){
        if(apply) {
            knockback.ApplyKnockback();
            HandleKnockback();
        }
        return false;
    } else if (paralyzed.IsActive()) {
        return false;
    }
    return true;
}

// returns true if monsters body is ready to be removed
bool Monster::IsReadyForDeletion() {
    return timeSinceDeath > DISAPPEAR_TIME;
}

// updates the monsters facing direction and sets xAxisIsInverted based on the next move
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

// update monsters health by calculating projectile collisions
void Monster::UpdateCollisions(GameState& state){
    for(auto it = state.projectiles.begin(); it != state.projectiles.end();){
        // if monster hitbox collides with projectile globalbounds and is not dead
        if((hitbox.findIntersection((*it)->GetGlobalBounds()) && !IsDead())){
            // if projectile hasn't went through a monster hitbox yet (for collaterals)
            if(!(*it)->HasHit(id)){
                // create bloodspray/groundblood
                if((*it)->createsBlood){
                    Blood::CreateProjectileBlood((*it)->GetPosition(), (*it)->sourcePosition, hitbox, state.bloodSpray, state.groundBlood, this);
                }
                // update damage
                TakeDamage((*it)->GetDamage());
            }
            // update status of projectile on collisions (explosive projectiles override this to create explosions)
            (*it)->UpdateProjectileStatus(*this, state, it);
        } else {
            ++it;
        }
    }
}

// when MONSTER_HITBOX = 1, draw all monster related bounds
void Monster::DebugHitbox(GameState& state) {
    HitboxDebugger::DrawSpriteGlobalBoundsHitbox(state.window, sprite, sf::Color::Red);
    HitboxDebugger::DrawSpriteOrigin(state.window, sprite, sf::Color::Green);
    HitboxDebugger::DrawGlobalRect(state.window, hitbox, sf::Color::Magenta);
    for (const auto& pair : attackMap) {
        pair.second->DrawAggroAndDamageBoxes(state.window);
    }
}

// Base implementation to update hitbox, derived monsters should
// set xHitRatio / yHitRatio to create subrect hitbox from texture bounds
// more compilicated monsters should override entirely
sf::FloatRect Monster::UpdateHitbox() {
    sf::FloatRect origBounds = sprite.getGlobalBounds();
    float posX = origBounds.position.x + origBounds.size.x*xHitRatio;
    float posY = origBounds.position.y + origBounds.size.y*yHitRatio;
    float width = origBounds.size.x - (origBounds.size.x*xHitRatio*2.f);
    float height = origBounds.size.y*(1-yHitRatio);
    hitbox = {{posX, posY}, {width, height}};
    return hitbox;
}

// MAIN move logic
void Monster::Move(std::vector<std::unique_ptr<Player>>& players) {
    // get difference vector from monster origin to player origin
    sf::Vector2f nextMove = players[0]->GetPosition() - sprite.getPosition();
    // apply any updates to base (default is nothing), eg. wolf rotates to face player
    BaseUpdates(nextMove);
    if(nextMove.length() > 1.f){ // if monster is not on top of player
        // create next move vector for the given frame (factoring in slowed movement speed)
        nextMove = nextMove.normalized()*(movementSpeed*slowed.slowFactor);
        // updates monster to face right or left
        UpdateFacingDirection(nextMove);
        // if a player is alive, disable flag for testing
        if(!disabledMovement){  
            // apply move vector
            sprite.move(nextMove);
        }
    }
}

// MAIN UPDATE CALL -> returns true if monster should be deleted
bool Monster::Update(GameState& state, float deltaTime){
    window = &state.window;
    // Update status effects, damage, timers
    UpdateStatusEffects(deltaTime, state.window);
    if(!IsDead()) { // if Monster is not dead :
        // update status effects, hitbox, collisions, attack logic, hp bar
        ProcessUpdates(state, deltaTime);
        // update current animation
        UpdateCurrentAnimation(deltaTime);
        // if monster has no disabled status effect (apply effect) and is not attacking
        if(HasNoDisablingStatusEffects(true) && !IsAttacking()){
            // if monster is NOT idle
            if(!IsIdle(state.players, deltaTime)){
                // MAIN move call
                Move(state.players);
            }
        }
        return false; // do not delete monster yet it's still alive
    } else {
        // Updates death state, animation, and timers
        UpdateDead(deltaTime);
        // returns true when dead body should disappear
        return IsReadyForDeletion();
    }
}