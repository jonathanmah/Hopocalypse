#include <iostream>
#include "entities/Player.h"
#include "entities/Monster.h"
#include "util/HitboxDebugger.h"
#include "core/GameState.h"
#include "weapons/Projectile.h"
#include "weapons/Weapon.h"
#include "weapons/derived/Ak47.h"
#include "weapons/derived/Famas.h"
#include "weapons/derived/Barrett50.h"
#include "weapons/derived/Rpg.h"
#include "weapons/derived/Uzi.h"
#include "weapons/derived/Flamethrower.h"
#include "weapons/derived/M1014.h"
#include "weapons/derived/M240.h"
#include "weapons/derived/M9.h"
#include "weapons/derived/Magnum.h"
#include "weapons/derived/ScarH.h"
#include "weapons/derived/M4a1.h"
#include "weapons/derived/Ump45.h"
#include "weapons/derived/P90.h"
#include "weapons/derived/M32.h"

/*
    Construct a player
    
    AnimData animData : the animation data
    sf::Vector2f position : starting position of the player
    float deathTimer : when a player dies, this timer starts and ends when the last death frame has been played
    bool isFiring : to set that the player is firing. Used outside the class for adding new projectiles to list
    float timeSinceLastShot : time since last projectile fired, used with a weapons cooldown 
*/
// default curr weapon to pistol later
Player::Player(AnimData animData, sf::Vector2f position) : 
    Character(animData,position,100,5.f,1.f), 
    deathTimer(0.f), 
    currWeapon(std::make_unique<Ak47>()) 
    {}

void Player::HandleDeath(float deltaTime) {
    if(Player::currState != PlayerState::DEATH){
        animData = AnimUtil::PlayerAnim::stand;
        Player::currState = PlayerState::DEATH;
    }
    if(Player::deathTimer < animData.animSpeed*animData.totalFrames) {
        AnimUtil::UpdateSpriteAnim(sprite, animData, deltaTime);
    }
    Player::deathTimer += deltaTime;
}

void Player::SetFacingDirection() {
    if(mousePosRelative.x < 0){
        sprite.setScale({scale, scale});
    }
    else {
        sprite.setScale({-scale, scale});
    }
}

void Player::Move(PlayerState& playerState, GameState& state, float deltaTime) { 
    playerState = PlayerState::STAND;
    sf::Vector2f nextMove = {0,0};
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A)) {
        playerState = PlayerState::WALK;
        if (sprite.getPosition().x - movementSpeed >= MapBounds::LEFT){
            nextMove.x += -1;
        }
        //sprite.setScale({-scale, scale}); 
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D)) {
        playerState = PlayerState::WALK;
        if (sprite.getPosition().x + movementSpeed <= MapBounds::RIGHT){
            nextMove.x += 1;
        }
        //sprite.setScale({scale, scale}); 
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W)) {
        playerState = PlayerState::WALK;
        if (sprite.getPosition().y - movementSpeed >= MapBounds::TOP){
            nextMove.y += -1;   
        }
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S)) {
        playerState = PlayerState::WALK;
        if (sprite.getPosition().y + movementSpeed <= MapBounds::BOTTOM) {
            nextMove.y += 1;
        }
    }
    if(triggerHappy > 2.5f && playerState == PlayerState::WALK){
        playerState = PlayerState::SHOOTING_WALK;
    } else if (triggerHappy > 2.5) {
        playerState = PlayerState::SHOOTING_STAND;
    }

    if(sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)) {
        triggerHappy += deltaTime;
    } else {
        triggerHappy = 0;
    }
    if(nextMove.length() > 0){
        sf::Vector2f nextMoveNormalized = nextMove.normalized();
        footprintManager.UpdateFootprints(this, nextMoveNormalized, state, deltaTime);
        sprite.move(nextMoveNormalized * Player::movementSpeed);
    }
    CycleWeapons();
    SetFacingDirection();
}

void Player::CycleWeapons() {
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::K)) {
        currWeapon = std::make_unique<Ak47>();
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::F)) {
        currWeapon = std::make_unique<Famas>();
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::B)) {
        currWeapon = std::make_unique<Barrett50>();
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::R)) {
        currWeapon = std::make_unique<Rpg>();
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::U)) {
        currWeapon = std::make_unique<Uzi>();
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::E)) {
        currWeapon = std::make_unique<Flamethrower>();
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Space)) {
        currWeapon->UpgradeWeapon();
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Num1)) {
        currWeapon = std::make_unique<M1014>();
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Num2)) {
        currWeapon = std::make_unique<M240>(*this);
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Num9)) {
        currWeapon = std::make_unique<M9>();
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::G)) {
        currWeapon = std::make_unique<Magnum>();
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::X)) {
        currWeapon = std::make_unique<ScarH>();
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::M)) {
        currWeapon = std::make_unique<M4a1>();
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Num4)) {
        currWeapon = std::make_unique<Ump45>();
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::P)) {
        currWeapon = std::make_unique<P90>();
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Num3)) {
        currWeapon = std::make_unique<M32>();
    }
}

void Player::SetMousePositions(sf::RenderWindow& window) {
    sf::Vector2i mousePos = sf::Mouse::getPosition(window);
    mousePosGlobal = sf::Vector2f{static_cast<float>(mousePos.x), static_cast<float>(mousePos.y)};
    mousePosRelative = mousePosGlobal - GetPosition();
}

// change anim data depending on player state
void Player::SetAnimDataByState(PlayerState newState) {
    switch(newState) {
        case PlayerState::STAND:
            animData = AnimUtil::PlayerAnim::stand;
            break;
        case PlayerState::WALK:
            animData = AnimUtil::PlayerAnim::walk;
            break;
        case PlayerState::HIT:
            animData = AnimUtil::PlayerAnim::walk;
            break;
        case PlayerState::SHOOTING_WALK:
            animData = AnimUtil::PlayerAnim::shootWalk;
            break;
        case PlayerState::SHOOTING_STAND:
            animData = AnimUtil::PlayerAnim::shootStand;
            break;
        case PlayerState::DEATH:
            break;
    }
    
    currState = newState;
}


void Player::Update(GameState& state, float deltaTime){
    // set relative mouse position first
    Player::SetMousePositions(state.window);

    // If Player is marked as dead, update state and death anim
    if (!Player::isAlive) {
        Player::HandleDeath(deltaTime);
        return;
    }
    // #TODO need to figure out how to deal with states better maybe...later problem
    PlayerState playerState = PlayerState::STAND;
    
    // check for actions that may change state, if all good then can move?
    // Handle key presses for movement, update footprints
    Player::Move(playerState, state, deltaTime);

    currWeapon->Update(state, *this, mousePosGlobal, deltaTime);
   
    if (Player::currState != playerState) {
        Player::SetAnimDataByState(playerState);
    }
    // update player animation
    AnimUtil::UpdateSpriteAnim(sprite, animData, deltaTime);

    // update health bar to show current HP
    hud.Update(health, Player::GetGlobalBounds());

    // set isAlive flag if a monster has intersected
    Player::CheckDeath(state.monsters);
}

// marks a player as Dead
void Player::CheckDeath(std::vector<std::unique_ptr<Monster>>& monsters) {

    if(!Player::isAlive){
        return;
    }
    for(auto& monster : monsters){
        if(monster->GetGlobalBounds().findIntersection(Player::GetGlobalBounds()) && monster->isAlive){
            Player::isAlive = true;
        }
    }
}
// draw hit box of a players global bounds and feet.  should be in character class for when monsters get footprints
void Player::DrawHitbox(sf::RenderWindow& window) {
    HitboxDebugger::DrawSpriteGlobalBoundsHitbox(window, sprite, sf::Color::Red);
    HitboxDebugger::DrawGlobalRect(window, footprintManager.GetFootCollider(this), sf::Color::Magenta, 3.0f);
}

// Render a player, hitbox, and weapon
void Player::Draw(sf::RenderWindow& window, BatchRenderer& batchRenderer) {
    window.draw(sprite);
    //DrawHitbox(window);
    if(Player::isAlive){
        currWeapon->Draw(window, batchRenderer);
    }
    hud.Draw(window);
}