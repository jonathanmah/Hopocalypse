#include "Character.h"
#include <iostream>
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
    //DrawHitbox(window);
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
void Character::UpdateCollisions(std::vector<std::unique_ptr<Projectile>>& projectiles, std::vector<Blood>& bloodSpray, std::vector<GroundBlood>& groundBlood) {
    for(auto it = projectiles.begin(); it != projectiles.end();){
        if(this->GetGlobalBounds().findIntersection((*it)->GetGlobalBounds()) && isAlive){
            if(!(*it)->HasHit(id)){
                Blood::CreateProjectileBlood((*it)->GetPosition(), GetGlobalBounds(), bloodSpray, groundBlood);
                UpdateHealth((*it)->GetDamage());
            }
            (*it)->UpdateProjectileStatus(projectiles, it, id);
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
void Character::UpdateFootprints(sf::Vector2f nextMoveNormalized, std::vector<Footprint>& footprints, std::vector<GroundBlood>& groundBlood, float deltaTime) {
    bool groundBloodCollision = GroundBlood::HasGroundBloodCollision(GetFootCollider(),groundBlood);
    if((groundBloodCollision || footprintDecayTimer > 0.01f) && footprintDtSumFrame >= FOOTPRINT_DT_RATE){
        AnimData footprintData;
        if(createLeftFootNext){
            footprintData = AnimUtil::BloodAnim::FootprintAnim::playerLeft;
            createLeftFootNext = false;
        } else {
            footprintData = AnimUtil::BloodAnim::FootprintAnim::playerRight;
            createLeftFootNext = true;
        }
        if (groundBloodCollision){
            footprintDecayTimer =  FOOTPRINT_DECAY_TIME; // refresh footprint timer after stepping in ground blood
        }
        std::cout << "Push back footprint" << std::endl;
        footprints.push_back(Footprint{footprintData, GetGlobalBounds(), nextMoveNormalized, 
            !createLeftFootNext, footprintDecayTimer});
        
        footprintDtSumFrame = 0.f; // reset time for next frame
    }
    footprintDtSumFrame += deltaTime;
    if(footprintDecayTimer > 0){
        footprintDecayTimer -= deltaTime;
    }
}
        