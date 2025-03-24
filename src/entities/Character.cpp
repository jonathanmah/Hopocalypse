#include <iostream>
#include "entities/Character.h"
#include "core/GameState.h"

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
    id(ID_COUNTER++)
{
  
    sprite.setTextureRect(sf::IntRect(animData.textureFrame.position, animData.textureFrame.size));
    sprite.setPosition(position);
    sprite.setScale({scale, scale});
    sprite.setOrigin({sprite.getLocalBounds().size.x / 2, sprite.getLocalBounds().size.y / 2});
}

// Render the character and HP bar
void Character::Draw(sf::RenderWindow& window, BatchRenderer& batchRenderer) {
    window.draw(sprite);
    if(health > 0){
        hud.Draw(window);
    }
        
}

void Character::CheckDeath() {
    if(health <= 0) {
        isAlive = false;
        health = 0;
    }
}

void Character::TakeDamage(int damage) {
    health = std::max(0,health-damage);
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
        
        // if((*it)->ReachedDest()){
        //     (*it)->UpdateProjectileStatus(*this, state, it);
        // }
        // DETECTS IF HIT HERE GLOBAL BOUNDS OF MONSTER VS GLOBAL BOUNDS OF PROJECTILE, NEED TO UPDATE.
        // maybe run another loop through AOE, and determine what to do to intersections with it
        // elsewhere, just use this as an entry for creating the AOE
        if((this->GetGlobalBounds().findIntersection((*it)->GetGlobalBounds()) && isAlive)){
            
            // if projectile hasn't went through a monster hitbox yet
            if(!(*it)->HasHit(id)){
                if((*it)->createsBlood){
                    Blood::CreateProjectileBlood((*it)->GetPosition(), GetGlobalBounds(), state.bloodSpray, state.groundBlood);
                }
                // UPDATES DAMAGE ELSWHERE. AFTER EXPLOSION LOOPS. this just for 
                // projectile making contact.x
                TakeDamage((*it)->GetDamage());
            }
            // ADD NEW AOE HERE
            (*it)->UpdateProjectileStatus(*this, state, it);
        } else {
            ++it;
        }
    }
}