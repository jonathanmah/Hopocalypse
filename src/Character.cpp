#include <iostream>
#include "Character.h"
#include "GameState.h"

static constexpr float FOOT_COLLIDER_X_OFFSET = 12.f;
static int ID_COUNTER = 0;

/*
#TODO make health a default parameter of =100 in header but overwrite it to spawn tanks

Construct a Character
    
    AnimData animData : given the animation data
    sf::Vector2f position : starting position of the character
    int health : starting HP of the character
    float scale : scale transformation on sprite
    
*/
Character::Character(AnimData animData, sf::Vector2f position, int health, float movementSpeed, float scale, int id)
  : animData(animData), 
    sprite(*animData.texture),
    health(100), 
    movementSpeed(movementSpeed), 
    scale(scale), 
    isAlive(true), 
    createLeftFootNext(true),
    footprintDecayTimer(0.f),
    footprintDtSumFrame(0.f),
    id(ID_COUNTER++) {
  
    sprite.setTextureRect(sf::IntRect(animData.textureFrame.position, animData.textureFrame.size));
    sprite.setPosition(position);
    sprite.setScale({scale, scale});
    sprite.setOrigin({sprite.getLocalBounds().size.x / 2, sprite.getLocalBounds().size.y / 2});
}

// Render hitbox of a character
// #TODO this is reptitive, should have a hitbox utility function for all things that wanna load a sprite
void Character::DrawHitbox(sf::RenderWindow& window) {
    sf::FloatRect bounds = sprite.getGlobalBounds();
    sf::RectangleShape rect(sf::Vector2f(bounds.size));
    rect.setPosition(bounds.position);
    rect.setFillColor(sf::Color::Transparent);
    rect.setOutlineColor(sf::Color::Red);
    rect.setOutlineThickness(2.0f);
    window.draw(rect);
}

// Render the character and HP bar
void Character::Draw(sf::RenderWindow& window) {
    window.draw(sprite);
    if(health > 0){
        hud.Draw(window);
    }
        
}

void Character::UpdateHealth(int damage) {
    health -= damage;
    if(health <= 0) {
        isAlive = false;
        health = 0;
    }
}
/*
For all projectiles in the game:
    if a projectile collides with this current character:
        create a bloodSpray and ground blood
        #TODO keep this blood creation / blood rotation logic out of this class and keep in blood class
        // just do something simple like a push_back(Blood::func()) that returns a new blood object)
        remove projectile from array
*/ 
// logic for ALL characters
void Character::UpdateCollisions(GameState& state){
    for(auto it = state.projectiles.begin(); it != state.projectiles.end();){
        
        // DETECTS IF HIT HERE GLOBAL BOUNDS OF MONSTER VS GLOBAL BOUNDS OF PROJECTILE, NEED TO UPDATE.
        // maybe run another loop through AOE, and determine what to do to intersections with it
        // elsewhere, just use this as an entry for creating the AOE
        if(this->GetGlobalBounds().findIntersection((*it)->GetGlobalBounds()) && isAlive){
            if(!(*it)->HasHit(id)){
                Blood::CreateProjectileBlood((*it)->GetPosition(), GetGlobalBounds(), state.bloodSpray, state.groundBlood);
                
                // UPDATES DAMAGE ELSWHERE. AFTER EXPLOSION LOOPS. this just for 
                // projectile making contact.x
                UpdateHealth((*it)->GetDamage());
            }
            // ADD NEW AOE HERE
            (*it)->UpdateProjectileStatus(state.projectiles, it, state.aoe, id);
        } else {
            ++it;
        }
    }
}

// get the current hitbox for the characters foot
sf::FloatRect Character::GetFootCollider() {
    sf::FloatRect origBounds = GetGlobalBounds();
    float posX = origBounds.position.x + FOOT_COLLIDER_X_OFFSET;
    float posY = origBounds.position.y + origBounds.size.y*0.8f;
    float width = origBounds.size.x - (FOOT_COLLIDER_X_OFFSET*2);
    float height = origBounds.size.y*0.2f;
    return sf::FloatRect{{posX, posY}, {width, height}};
}

// create a new footprint from a character
void Character::UpdateFootprints(sf::Vector2f nextMoveNormalized, GameState& state, float deltaTime) {
    bool groundBloodCollision = GroundBlood::HasGroundBloodCollision(GetFootCollider(),state.groundBlood);
    if((groundBloodCollision || footprintDecayTimer > 0.01f) && footprintDtSumFrame >= FOOTPRINT_DT_RATE){
        AnimData footprintData;
        if(createLeftFootNext){
            footprintData = AnimUtil::BloodAnim::leftFootprint;
            createLeftFootNext = false;
        } else {
            footprintData = AnimUtil::BloodAnim::rightFootprint;
            createLeftFootNext = true;
        }
        if (groundBloodCollision){
            footprintDecayTimer =  FOOTPRINT_DECAY_TIME; // refresh footprint timer after stepping in ground blood
        }
        state.footprints.push_back(Footprint{footprintData, GetGlobalBounds(), nextMoveNormalized, 
            !createLeftFootNext, footprintDecayTimer});
        
        footprintDtSumFrame = 0.f; // reset time for next frame
    }
    footprintDtSumFrame += deltaTime;
    if(footprintDecayTimer > 0){
        footprintDecayTimer -= deltaTime;
    }
}
        