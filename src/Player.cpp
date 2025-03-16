#include "AnimUtil.h"
#include "Player.h"
#include "Projectile.h"
#include "Weapon.h"
#include "Map.h"
#include <iostream>
#include "Ak47.h"
#include "Famas.h"
#include "Barrett50.h"
#include "Rpg.h"

/*
    Construct a player
    
    AnimData animData : the animation data
    sf::Vector2f position : starting position of the player
    float deathTimer : when a player dies, this timer starts and ends when the last death frame has been played
    bool isFiring : to set that the player is firing. Used outside the class for adding new projectiles to list
    float timeSinceLastShot : time since last projectile fired, used with a weapons cooldown 
*/
// default curr weapon to pistol later
Player::Player(AnimData animData, sf::Vector2f position) : Character(animData,position,100,5.f,1.f), deathTimer(0.f), currWeapon(std::make_unique<Ak47>()) {

}

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

void Player::Move(PlayerState& state, std::vector<Footprint>& footprints, std::vector<GroundBlood>& groundBlood, float deltaTime) { 
    state = PlayerState::STAND;
    sf::Vector2f nextMove = {0,0};
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A)) {
        state = PlayerState::WALK;
        if (sprite.getPosition().x - movementSpeed >= MapBounds::LEFT){
            nextMove.x += -1;
        }
        //sprite.setScale({-scale, scale}); 
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D)) {
        state = PlayerState::WALK;
        if (sprite.getPosition().x + movementSpeed <= MapBounds::RIGHT){
            nextMove.x += 1;
        }
        //sprite.setScale({scale, scale}); 
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W)) {
        state = PlayerState::WALK;
        if (sprite.getPosition().y - movementSpeed >= MapBounds::TOP){
            nextMove.y += -1;   
        }
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S)) {
        state = PlayerState::WALK;
        if (sprite.getPosition().y + movementSpeed <= MapBounds::BOTTOM) {
            nextMove.y += 1;
        }
    }
    if(triggerHappy > 2.5f && state == PlayerState::WALK){
        state = PlayerState::SHOOTING_WALK;
    } else if (triggerHappy > 2.5) {
        state = PlayerState::SHOOTING_STAND;
    }

    if(sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)) {
        triggerHappy += deltaTime;
    } else {
        triggerHappy = 0;
    }
    if(nextMove.length() > 0){
        sf::Vector2f nextMoveNormalized = nextMove.normalized();
        Character::UpdateFootprints(nextMoveNormalized, footprints, groundBlood, deltaTime);
        sprite.move(nextMoveNormalized * Player::movementSpeed);
    }
    CycleWeapons();
    SetFacingDirection();
}

// 
// void Player::UpdateWeaponPosition(sf::RenderWindow& window, float deltaTime) {
//     currWeapon->Update(GetPosition(), mousePosRelative.normalized(), deltaTime);
// }

void Player::CycleWeapons() {
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::E)) {
        currWeapon = std::make_unique<Ak47>();
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::R)) {
        currWeapon = std::make_unique<Famas>();
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Q)) {
        currWeapon = std::make_unique<Barrett50>();
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::F)) {
        currWeapon = std::make_unique<Rpg>();
    }
}

void Player::SetMousePositions(sf::RenderWindow& window) {
    sf::Vector2i mousePos = sf::Mouse::getPosition(window);
    mousePosGlobal = sf::Vector2f{static_cast<float>(mousePos.x), static_cast<float>(mousePos.y)};
    mousePosRelative = mousePosGlobal - GetPosition();
}

// void Player::Shoot(std::vector<std::unique_ptr<Projectile>>& projectiles) {
//     currWeapon->Shoot(projectiles, mousePosRelative.normalized(), mousePosGlobal);
// }
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


void Player::Update(float deltaTime, std::vector<Monster>& monsters, sf::RenderWindow& window, 
    std::vector<Footprint>& footprints, std::vector<GroundBlood>& groundBlood, std::vector<std::unique_ptr<Projectile>>& projectiles) { 
    // set relative mouse position first
    Player::SetMousePositions(window);

    // If Player is marked as dead, update state and death anim
    if (!Player::isAlive) {
        Player::HandleDeath(deltaTime);
        return;
    }
    // #TODO need to figure out how to deal with states better maybe...later problem
    PlayerState state = PlayerState::STAND;
    
    // check for actions that may change state, if all good then can move?
    // Handle key presses for movement, update footprints
    Player::Move(state, footprints, groundBlood, deltaTime);

    currWeapon->Update(GetPosition(), mousePosGlobal, projectiles, deltaTime);
   
    if (Player::currState != state) {
        Player::SetAnimDataByState(state);
    }
    // update player animation
    AnimUtil::UpdateSpriteAnim(sprite, animData, deltaTime);

    // update health bar to show current HP
    hud.Update(health, Player::GetGlobalBounds());

    // set isAlive flag if a monster has intersected
    Player::CheckDeath(monsters);
}

// marks a player as Dead
void Player::CheckDeath(std::vector<Monster>& monsters) {

    if(!Player::isAlive){
        return;
    }
    for(Monster& monster : monsters){
        if(monster.GetGlobalBounds().findIntersection(Player::GetGlobalBounds()) && monster.isAlive){
            Player::isAlive = true;
        }
    }
}

//#TODO refactor this into common utility func. draw bounds of a sprite
void DrawBounds(sf::RenderWindow& window, sf::FloatRect bounds, sf::Color colour, float thickness=2.0f) {
    sf::RectangleShape rect(sf::Vector2f(bounds.size));
    rect.setPosition(bounds.position);
    rect.setFillColor(sf::Color::Transparent);
    rect.setOutlineColor(colour);
    rect.setOutlineThickness(thickness);
    window.draw(rect);
}

// draw hit box of a players global bounds and feet.  should be in character class for when monsters get footprints
void Player::DrawHitbox(sf::RenderWindow& window) {
    DrawBounds(window, GetGlobalBounds(), sf::Color::Red);
    DrawBounds(window, GetFootCollider(), sf::Color::Magenta, 3.0f);
}

// Render a player, hitbox, and weapon
void Player::Draw(sf::RenderWindow& window) {
    window.draw(sprite);
    //DrawHitbox(window);
    if(Player::isAlive){
        currWeapon->Draw(window);
    }
    hud.Draw(window);
}