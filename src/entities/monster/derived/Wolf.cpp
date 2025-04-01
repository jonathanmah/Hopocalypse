#include "entities/monster/derived/Wolf.h"
#include "util/AnimUtil.h"
#include "entities/monster/attack/MeleeAttack.h"
#include "core/GameState.h"
#include "util/HitboxDebugger.h"
#include <cmath>
static constexpr float DEATH_DT_SUM_PER_FRAME = 0.10f; // time in between frames to rotate post death
static constexpr int DEATH_ROTATE_PER_FRAME_DEG = 5; // degrees to rotate per frame post death
static constexpr float MOVEMENTSPEED = 7.f; // movement speed constant

Wolf::Wolf(sf::Vector2f position) 
: 
    Monster(
        position, // Starting position
        AnimUtil::WolfAnim::run, // Starting animation
        1000, // health
        MOVEMENTSPEED, // movement speed
        1.f, // scale
        0.2f, // xHitRatio (used for calculating hitbox)
        0.35f// yHitRatio (used for calculating hitbox)
    ), 
    freezeJaw(false), // freezeJaw flag, used to freeze animation mid jump with wolf jaw open
    jumpCd(8.f), // cooldown time for wolf jump attack for resetting
    jumpCdTimer(0.f) // current cooldown for wolf jump attack
{
    InitAnimMap(); // initialize State:Animation map
    InitAttackMap(); // initialize State:Attack map (create attack with params)
    sprite.setTextureRect(sf::IntRect(animData.textureFrame.position, animData.textureFrame.size));
    sprite.setOrigin({animData.textureFrame.size.x*.45f, animData.textureFrame.size.y*.5f});
}

// handle attack logic, attack cooldowns
void Wolf::HandleAttacks(GameState& state, float deltaTime) {

    attackMap[MonsterState::ATTACK1]->UpdateBoxBounds();
    if(attackMap[MonsterState::ATTACK1]->IsOnCooldown()){
        // if attack is on cooldown, then update cooldown
        attackMap[MonsterState::ATTACK1]->cooldownTimer -= deltaTime;
    }
    // update jump attack cooldown timer
    if(jumpCdTimer > 0.f) {
        jumpCdTimer = std::max(0.f, jumpCdTimer-deltaTime);
    }
    // if currently attacking, then call attack update, 
    if(animState == MonsterState::ATTACK1){
        attackMap[MonsterState::ATTACK1]->UpdateDuringAttack(state.players, deltaTime);
    } else {
        // check if can start attack
        attackMap[MonsterState::ATTACK1]->CheckConditionsAndAttack(state.players);
    }
}

void Wolf::InitAttackMap() {
    auto damageBox = UpdateHitbox(); 
    auto aggrobox = GetGlobalBounds();
    damageBox.size.x *= .85f;
    aggrobox.size *= 1.2f;
    attackMap[MonsterState::ATTACK1] = 
        std::make_unique<WolfAttack>(
            MonsterState::ATTACK1, // attack state
            this,  // monster pointer
            .9f,  // cooldown  
            aggrobox, // bounds to trigger an attack
            damageBox, // bounds to apply damage
            23,  // frame of animation to damage
            100 // damage
        );
}

void Wolf::InitAnimMap() {
    animMap[MonsterState::WALK] = AnimUtil::WolfAnim::walk;
    animMap[MonsterState::IDLE] = AnimUtil::WolfAnim::idle;
    animMap[MonsterState::RUN] = AnimUtil::WolfAnim::run;
    animMap[MonsterState::ATTACK1] = AnimUtil::WolfAnim::attack;
    animMap[MonsterState::DEATH] = AnimUtil::WolfAnim::death;
    animMap[MonsterState::DEATH].hangLastFrame = true;
}

// unique death handling, rotates wolf as it dies
void Wolf::UpdateDead(float deltaTime) {
    UpdateStateAndAnimation(MonsterState::DEATH);
    AnimUtil::UpdateSpriteAnim(sprite, animData, deltaTime);
    timeSinceDeath += deltaTime;
    int targetDeg = xAxisInverted ? 330 : 30;
    
    if (deathDtSum < DEATH_DT_SUM_PER_FRAME ){
        float currRotation = sprite.getRotation().asDegrees();
        if (xAxisInverted &&  (currRotation > 330 || std::abs(currRotation) < 0.1f)) { // bug with how sfml handles degrees and it's over rotating. idk. just use 89
            if(currRotation <= 0.f || currRotation >= targetDeg){
                sprite.rotate(sf::degrees(-DEATH_ROTATE_PER_FRAME_DEG));
                sprite.move({0,3});
            }

        } else {
            if (!xAxisInverted && (std::abs(currRotation) < 0.1f || currRotation < 30)) {
                if(currRotation <= targetDeg){
                    sprite.rotate(sf::degrees(DEATH_ROTATE_PER_FRAME_DEG));
                    sprite.move({0,1});
                }
                
            }
        }
        deathDtSum = 0.f;
    }
    deathDtSum += deltaTime;
}

// update hitbox bounds
sf::FloatRect Wolf::UpdateHitbox() {
    sf::FloatRect origBounds = sprite.getGlobalBounds();
    float posX;
    if(!xAxisInverted) {
        posX = origBounds.position.x + origBounds.size.x*xHitRatio;    
    } else {
        posX = origBounds.position.x + origBounds.size.x*0.35f;    
    }
    
    float posY = origBounds.position.y + origBounds.size.y*yHitRatio;
    
    float width = origBounds.size.x * .45f;
    float height = origBounds.size.y*(.25f);
    hitbox = {{posX, posY}, {width, height}};
    return hitbox;
}

// update current animation for two dimensional sequence
void Wolf::UpdateCurrentAnimation(float deltaTime) {
    if(HasNoDisablingStatusEffects(false) && !freezeJaw){ 
        AnimUtil::UpdateSpriteXYOffsetAnim(sprite, animData, deltaTime);
    }
}

// unfreeze jaw if wolf is mid jump during knockback attack
void Wolf::HandleKnockback() {
    if(freezeJaw){
        freezeJaw = false;
        jumpCdTimer = jumpCd;
    }
}


WolfAttack::WolfAttack(
        MonsterState attackState,
        Monster* monster, 
        float cooldown, 
        sf::FloatRect aggroBox,
        sf::FloatRect damageBox, 
        int damageFrame,
        int damage
    )   :
        MeleeAttack(
            attackState, 
            monster, 
            cooldown, 
            aggroBox, 
            damageBox, 
            damageFrame,
             damage
        ), 
        jumpMovement(15.f), // init jump movement
        normalizedDir{0,0}
{}

// update damage/aggro box bounds
void WolfAttack::UpdateBoxBounds() {
    auto hitbox = monster->hitbox;
    
    if(!monster->xAxisInverted) {
        aggroBox.setOrigin({0, aggroBox.getLocalBounds().size.y/2});
        damageBox.setOrigin({0, damageBox.getLocalBounds().size.y/2});
        aggroBox.setPosition({hitbox.position.x+hitbox.size.x*0.9f,hitbox.position.y+hitbox.size.y/2});
        damageBox.setPosition({hitbox.position.x+hitbox.size.x*0.9f,hitbox.position.y+hitbox.size.y/2});
    } else {
        aggroBox.setOrigin({aggroBox.getLocalBounds().size.x, aggroBox.getLocalBounds().size.y/2});
        damageBox.setOrigin({damageBox.getLocalBounds().size.x, damageBox.getLocalBounds().size.y/2});
        aggroBox.setPosition({hitbox.position.x+hitbox.size.x*0.1f,hitbox.position.y + hitbox.size.y/2});
        damageBox.setPosition({hitbox.position.x+hitbox.size.x*0.1f,hitbox.position.y + hitbox.size.y/2});
    }

    aggroBox.setRotation(monster->sprite.getRotation());
    damageBox.setRotation(monster->sprite.getRotation());
}

// apply damage once damage frame reached, reset state to walk if attack animation finished
void WolfAttack::UpdateDuringAttack(std::vector<std::unique_ptr<Player>>& players, float deltaTime) {
    //#NOTE use this to draw target positions for future monsters
    //HitboxDebugger::DrawCircle(*monster->window, targetPosition, 20.f, sf::Color::Cyan);

    if(auto wolf = dynamic_cast<Wolf*>(monster)) { // dynamic cast monster ptr as wolf
        if(playerToJump != nullptr) { // if player to jump has been set

            if((playerToJump->GetPosition() - wolf->hitbox.getCenter()).length() < 170){ // #TODO HARDCODED PIXELS UPDATE LATER
                // if player to jump in radius of wolf, then update target position to player position
                // added this so can't tease wolf stepping back and forth xD
                targetPosition.x = playerToJump->GetPosition().x;
                targetPosition.y = playerToJump->GetPosition().y;
                normalizedDir = (targetPosition - wolf->hitbox.getCenter()).normalized();
            }
        }
        // if wolf is mid jump and reached target position, close jaws and continue animation
        if((damageBox.getGlobalBounds().getCenter() - targetPosition).length() < 50) { //#TODO HARDCODED PIXELS UPDATE LATER
            if(wolf->freezeJaw){
                wolf->freezeJaw = false;
                wolf->jumpCdTimer = wolf->jumpCd;
            }
                // 22 is frame to freeze jaw, 23 is damage frame
        } else if(wolf->jumpCdTimer <= 0.f && monster->animData.currFrame == 22) { 
            // if wolf jump is available and reached freeze frame, then freeze jaw and start jump
            wolf->freezeJaw = true;
            wolf->sprite.move(normalizedDir*jumpMovement);
        }
        
        // if current frame is damage frame - 23, then 
        //check each player in range of damage box and apply damage
        // dealt flag is to prevent multiple attacks within the same frame
        if(!dealt && monster->animData.currFrame == damageFrame) {  
            for(auto& player : players) {
                if(player->IsDead()) continue;
                if(damageBox.getGlobalBounds().findIntersection(player->GetGlobalBounds())){
                    player->TakeDamage(damage);
                }
            }
            // set attack on cooldown
            SetAttackOnCooldown();
            dealt = true;
        }
        // if monster frame is 0 and attack has dealt damage (to differentiate from beginning 0 frame) 
        // then update wolf state to walk and reduce movement speed
        if(monster->animData.currFrame == 0 && dealt) {
            monster->UpdateStateAndAnimation(MonsterState::WALK);
            monster->movementSpeed = MOVEMENTSPEED/5;
        }

        // if wolf jump is on cooldown and is regular attack, rotate wolf towards player mid attack
        if(wolf->jumpCdTimer > 0.f) { // add this for edge case where players try to juke standing on top of wolf
            for(auto& player : players) { // makes wolf rotate mid attack 
                if(player->GetGlobalBounds().findIntersection(wolf->GetGlobalBounds())){//wolf->hitbox)){
                    wolf->BaseUpdates(player->GetPosition()-wolf->hitbox.getCenter());
                    break;
                }
            }
        } else { // rotate to direction of initial normalized diff
            monster->BaseUpdates(normalizedDir);
        }

        // If player is standing on top of wolf, shift wolf to make sure damage gets applied
        if(playerToJump != nullptr){
            if((playerToJump->GetPosition()-wolf->hitbox.getCenter()).length() < 10){ // #TODO HARDCODED PIXELS
                wolf->sprite.move({-10,0});
            }
        }
    }
}

// players are in range of aggro box, and attack is not on cooldown
void WolfAttack::CheckConditionsAndAttack(std::vector<std::unique_ptr<Player>>& players) {
    if(auto wolf = dynamic_cast<Wolf*>(monster)) {
        for(auto& player : players) { //#TODO sort by closest range when checking players later
            if(player->IsDead()) continue;
            // if jump attack is available, then use big aggro box to start attack
            if (wolf->jumpCdTimer <= 0.f){
                if(cooldownTimer <= 0.f && aggroBox.getGlobalBounds().findIntersection(player->GetGlobalBounds())) {
                    StartAttack(player);
                    break;
                }
            } else {
                // if jump attack on cooldown, use wolf hitbox as trigger to attack close range
                if(cooldownTimer <= 0.f && player->GetGlobalBounds().findIntersection(wolf->hitbox)){ // has to intersect with player
                    StartAttack(player);
                    break;
                }
            }
        }
        // once attack cooldown is done, reset wolf state to run
        if(cooldownTimer <= 0.f && (wolf->animState == MonsterState::WALK)) {
            wolf->UpdateStateAndAnimation(MonsterState::RUN);
            wolf->movementSpeed = MOVEMENTSPEED;
        }
    }
}

// update state and animation to attack, init target position, player to attack, direction
void WolfAttack::StartAttack(std::unique_ptr<Player>& player) {
    monster->UpdateStateAndAnimation(attackState);
    dealt = false;
    targetPosition = player->GetPosition();
    normalizedDir = (targetPosition - monster->hitbox.getCenter()).normalized();
    playerToJump = player.get();
}

void Wolf::BaseUpdates(sf::Vector2f normalized) {
    float radians = atan2(normalized.y, normalized.x);
    xAxisInverted = (normalized.x < 0);
    float originalDegrees = radians * 180.f / M_PI;
    if(originalDegrees == -90.f) {
        originalDegrees = -90.1f;
        xAxisInverted = true;
    } else if(originalDegrees == 90.f) {
        originalDegrees = 90.1f;
    }
    float degrees = originalDegrees;
    if(animState!=MonsterState::ATTACK1){
        if (originalDegrees > 50.f && originalDegrees <= 90.f) {
            degrees = 50.f;
        } else if (originalDegrees >= 90.f && originalDegrees < 130.f) {
            degrees = 130.f;
        } else if (originalDegrees > -125.f && originalDegrees <= -90.f) {
            degrees = -125.f;
        } else if (originalDegrees >= -90.f && originalDegrees < -55.f) {
            degrees = -55.f;
        }
    }
    if (xAxisInverted) {  
        sprite.setScale({-std::abs(sprite.getScale().x), sprite.getScale().y});
        sprite.setRotation(sf::degrees(degrees + 180.f));    
    } else {                    
        sprite.setScale({std::abs(sprite.getScale().x), sprite.getScale().y});
        sprite.setRotation(sf::degrees(degrees));
    }
}