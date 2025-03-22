#include "fx/OnFire.h"
#include "core/GameState.h"
#include <cmath>


OnFire::OnFire(AnimData animData, sf::Vector2f position, Character& character) :
    StatusEffect(animData, position, character, 5.f) {
        sprite.setOrigin({sprite.getLocalBounds().size.x*.6f, sprite.getLocalBounds().size.y*.56f});
        sprite.setScale({2.6f,2.4f});
        sf::Angle angle = sf::radians(M_PI/2);
        sprite.setRotation(angle);
    }

bool OnFire::Update(float deltaTime) {
    sf::Angle angle = sf::radians(M_PI/2);
    sprite.setRotation(angle);
    timeActive -= deltaTime;
    if(timeActive <= 0) {
        character.isOnFire = false;
    }
    return AnimUtil::UpdateSpriteXYAnim(sprite, animData, deltaTime);
}