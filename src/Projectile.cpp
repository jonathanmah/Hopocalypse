#include "Projectile.h"
#include <iostream>
#include <cmath>

static constexpr float COLLATERAL_REDUCTION_FACTOR = 0.8f;
/*
    Construct a projectile
    
    AnimData animData : given the animation data (static animation right now)
    sf::Vector2f position : starting position of the projectile in pixels
    sf::Vector2f velocity : velocity of the projectile, will move at this rate every frame update
    int damage : amount of damage the projectiles does when collides
*/

Projectile::Projectile(ProjectileData projectileData, sf::Vector2f position, sf::Vector2f normalized)
: sprite(*projectileData.anim.texture), velocity(projectileData.speed*normalized), damage(projectileData.damage), 
collateralCount(projectileData.collateralCount), scale(projectileData.scale) {
    sprite.setTextureRect(sf::IntRect(projectileData.anim.textureFrame.position, projectileData.anim.textureFrame.size));
    sprite.setPosition({position.x, position.y});
    sprite.setScale({scale, scale});
    sprite.setOrigin({sprite.getLocalBounds().size.x / 2, sprite.getLocalBounds().size.y / 2});
    Rotate(velocity);
}

bool Projectile::HasHit(int characterId) {
    if (hitCharacters.find(characterId) != hitCharacters.end()) {
        return true;
    }
    return false;
}

// this function is acting as a single iterator object
// (*it).UpdateSingleProjectileStatus()
void Projectile::UpdateProjectileStatus(std::vector<std::unique_ptr<Projectile>>& projectiles, std::vector<std::unique_ptr<Projectile>>::iterator& it, int characterId) {
    //std::cout << "ENTERED BASE PROJECTILE CLASS" << std::endl;
    if(GetCollateralCount() > 1) {
        if(HasHit(characterId)){
            ++it;
            return;
        }
        DecrementCollateralCount();
        SetDamage(GetDamage()*COLLATERAL_REDUCTION_FACTOR);
        hitCharacters.insert(characterId);
        ++it;
    } else {
        //#TODO need to somehow tell rocket animation to explode when this happens here for RPG
        it = projectiles.erase(it);
    }
}


// rotate facing direction of gun muzzle
void Projectile::Rotate(sf::Vector2f velocity) {
    // get angle of velocity in radians with arc tangent
    float radians = atan2(velocity.y,velocity.x);
    sf::Angle angle = sf::radians(radians);
    sprite.setRotation(angle);
}
/*
#TODO only sets position and rotation for now.  Will need to pass deltatime to add animation to a projectile later

Update the projectile position and rotation based on its velocity

*/
void Projectile::UpdatePosition() {
    sprite.setPosition(GetPosition() + velocity);
    Rotate(velocity);
}

// Update the position of every projectile
void Projectile::UpdateProjectiles(std::vector<std::unique_ptr<Projectile>>& projectiles) {
    for(std::unique_ptr<Projectile>& projectile: projectiles) {
        projectile->UpdatePosition();
    }
}

// #DEV MODE - render the projectile hitbox
void DrawHit(sf::Sprite& sprite, sf::RenderWindow& window) {
    sf::FloatRect bounds = sprite.getGlobalBounds();
    sf::RectangleShape rect(sf::Vector2f(bounds.size));
    rect.setPosition(bounds.position);
    rect.setFillColor(sf::Color::Transparent);
    rect.setOutlineColor(sf::Color::Red);
    rect.setOutlineThickness(2.0f);
    window.draw(rect);
}

// render a projectile
void Projectile::Draw(sf::RenderWindow& window) {
    window.draw(sprite);
    //DrawHit(sprite, window);
}

// render all projectiles
void Projectile::RenderProjectiles(std::vector<std::unique_ptr<Projectile>>& projectiles, sf::RenderWindow& window) {
    for (std::unique_ptr<Projectile>& projectile : projectiles) {
        projectile->Draw(window);
    }
}

RPGrocket::RPGrocket(ProjectileData projectileData, sf::Vector2f position, sf::Vector2f normalized)
: Projectile(projectileData, position, normalized) {
}

void RPGrocket::UpdatePosition() {
    sprite.setPosition(GetPosition() + velocity);
    Rotate(velocity);
}

void RPGrocket::UpdateProjectileStatus(std::vector<std::unique_ptr<Projectile>>& projectiles, 
    std::vector<std::unique_ptr<Projectile>>::iterator& it, int characterId) {
        // set a detonate flag?? handle explosives somehow?
        //std::cout << "ENTERED DERIVED RPG CLASS" << std::endl;
        it = projectiles.erase(it);
    }