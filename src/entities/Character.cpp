#include <iostream>
#include "entities/Character.h"
#include "core/GameState.h"

static int ID_COUNTER = 0;



/*
#TODO make health a default parameter of =100 in header but overwrite it to spawn tanks

Construct a Character
    
    AnimData animData : given the animation data
    sf::Vector2f position : starting position of the character
    int health : starting HP of the character
    float scale : scale transformation on sprite
    
*/
Character::Character(sf::Vector2f position, AnimData animData, int health, float movementSpeed, float scale)
  : animData(animData), 
    sprite(*animData.texture),
    health(health), 
    movementSpeed(movementSpeed), 
    scale(scale), 
    isAlive(true), 
    id(ID_COUNTER++)
{
  
    sprite.setTextureRect(sf::IntRect(animData.textureFrame.position, animData.textureFrame.size));
    sprite.setPosition(position);
    sprite.setScale({scale, scale});
    sprite.setOrigin({animData.textureFrame.size.x / 2.f, animData.textureFrame.size.y / 2.f});
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


float Character::GetYOrdering() {
    auto bounds = sprite.getGlobalBounds();
    return bounds.position.y + bounds.size.y;
}
