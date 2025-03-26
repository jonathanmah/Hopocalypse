#include "fx/Blood.h"
#include "core/GameState.h"
#include <iostream>
#include <cmath>
#include "util/RandomUtil.h"


static int bloodcount = 0;
sf::Texture* Blood::texture = nullptr;

// 2PI = 360deg, PI = 180deg, PI/2 = 90deg, PI/4 = 45deg
static constexpr float PI = 3.141592;
static constexpr float ON_HIT_SPRAY_ROTATION_OFFSET = PI/4.f;
static constexpr float ON_HIT_SPRAY_POSITION_OFFSET = 50.f; 
static constexpr float ON_HIT_GROUND_ROTATION_OFFSET = 0.f;
static constexpr float ON_HIT_GROUND_POSITION_OFFSET = 10.f;

/*
Construct a blood
    
AnimData animData : given the animation data
sf::Vector2f position : position of character
     
*/

// position center of character global bounds
Blood::Blood(AnimData animData, sf::Vector2f position) : animData(animData), colour(sf::Color::White){
    setOrigin({static_cast<float>(animData.textureFrame.size.x)/2.f, static_cast<float>(animData.textureFrame.size.y)/2.f});
    setPosition({position.x, position.y});
    setScale({.9f, .9f});
    bloodcount++;
}

// For each bloodspray, update frame or delete it if it's finished the sequence
// For each groundblood, update the ground blood anim 
void Blood::Update(GameState& state, float deltaTime) {
    std::vector<Blood>::iterator it;
    for(auto it = state.bloodSpray.begin(); it != state.bloodSpray.end();){
        if((*it).UpdateBloodSprayAnim(deltaTime)){
            it = state.bloodSpray.erase(it);
        } else {
            ++it;
        }
    }
    for(GroundBlood& blood : state.groundBlood) {
       blood.UpdateGroundBloodAnim(deltaTime);
    }
}

// Rotate the blood spray object based on the direction of the incoming projectile, so it looks like it sprays through the character
void Blood::SetRotation(sf::Vector2f incomingProjectilePos) {
    sf::Vector2f difference = GetPosition() - incomingProjectilePos;
    float radians = atan2(difference.y, difference.x) + ON_HIT_SPRAY_ROTATION_OFFSET;
    sf::Angle angle = sf::radians(radians);
    setRotation(angle);
    move(difference.normalized()*ON_HIT_SPRAY_POSITION_OFFSET);
}


// render a blood object
// void Blood::Draw(sf::RenderWindow& window) {
//     window.draw(sprite);
//     //DrawHitboxt(sprite, window);
// }


// use different blood spray animation each time a projectile hits
AnimData Blood::GetNextSprayAnim() {
    static const AnimData sprayAnimations[6] = {
        AnimUtil::BloodAnim::spray1, 
        AnimUtil::BloodAnim::spray2, 
        AnimUtil::BloodAnim::spray3,
        AnimUtil::BloodAnim::spray4, 
        AnimUtil::BloodAnim::spray5, 
        AnimUtil::BloodAnim::spray6
    };
    AnimData animData = sprayAnimations[RandomUtil::GetRandomInt(0,5)];
    return animData;
}

// update blood spray and ground blood after projectile collision from projectile source position
void Blood::CreateProjectileBlood(sf::Vector2f sourcePosition, sf::FloatRect hitbox, 
    std::vector<Blood>& bloodSpray, std::vector<GroundBlood>& groundBlood){
    Blood newBloodSpray(Blood::GetNextSprayAnim(), hitbox.getCenter());
    newBloodSpray.SetRotation(sourcePosition);
    newBloodSpray.CachePositionVertices();
    bloodSpray.push_back(newBloodSpray);    /// even lower for ground blood
    GroundBlood newGroundBlood(AnimUtil::BloodAnim::ground, {hitbox.getCenter().x, hitbox.getCenter().y + hitbox.size.y*.3f});
    newGroundBlood.SetRotation(sourcePosition);
    newGroundBlood.CachePositionVertices();
    groundBlood.push_back(newGroundBlood);
}

sf::RectangleShape createBoundingBox(const std::array<sf::Vector2f,4> vertices);
// render all bloodspray, groundblood, and footprints in a single draw call
void Blood::RenderBlood(GameState& state, sf::RenderWindow& window){
    std::vector<Blood*> combinedBlood;
    for(Footprint& blood : state.footprints) combinedBlood.push_back(&blood);
    for(GroundBlood& blood : state.groundBlood) {
        combinedBlood.push_back(&blood);
    }
    for (Blood& blood : state.bloodSpray){
        combinedBlood.push_back(&blood);
    }
    state.batchRenderer->BatchRenderStaticFrames(combinedBlood);
}


sf::RectangleShape createBoundingBox(const std::array<sf::Vector2f,4> vertices) {
    float minX = vertices[0].x;
    float minY = vertices[0].y;
    float maxX = vertices[0].x;
    float maxY = vertices[0].y;
    for (int i = 1; i < 4; ++i) {
        minX = std::min(minX, vertices[i].x);
        minY = std::min(minY, vertices[i].y);
        maxX = std::max(maxX, vertices[i].x);
        maxY = std::max(maxY, vertices[i].y);
    }
    sf::RectangleShape boundingBox;
    boundingBox.setPosition({minX, minY});
    boundingBox.setSize(sf::Vector2f(maxX - minX, maxY - minY));
    boundingBox.setOutlineColor(sf::Color::Red);
    boundingBox.setOutlineThickness(2);
    boundingBox.setFillColor(sf::Color::Transparent);
    return boundingBox;
}



// Returns true if the global bounds of an object intersects with the ground blood
bool GroundBlood::HasGroundBloodCollision(const sf::FloatRect& globalBounds, std::vector<GroundBlood>& groundBlood) {
    for (Blood& blood : groundBlood) {
        GroundBlood& groundBlood = static_cast<GroundBlood&>(blood);
        if(groundBlood.collider.getGlobalBounds().findIntersection(globalBounds)){
            return true;
        }
    }
    return false;
}
// same constructor as base class but also intializes an oval to represent a collider and scales down the texture
GroundBlood::GroundBlood(AnimData animData, sf::Vector2f position) : Blood(animData, position){
    collider.setRadius(10.f);
    collider.setOrigin(collider.getGeometricCenter());
    collider.setFillColor(sf::Color::Blue);
    collider.setPosition(GetPosition());
    collider.setScale({2.f,1.f});
    // overwrites completely 
    setScale({.65f,.65f});
}

bool Blood::UpdateBloodSprayAnim(float deltaTime) {
    if(animData.deltaTimeSum >= animData.animSpeed) {
        int textureCoordsPosX = 32 + (animData.currFrame % animData.totalFrames) * animData.frameSpacing;  // 0 mod 4 = 0, 4 mod 4 = 0
        animData.textureFrame.position.x = textureCoordsPosX;
        animData.currFrame++;
        if (animData.currFrame >= animData.totalFrames) {
            animData.currFrame = 0; 
            return true;
        }
        animData.deltaTimeSum = 0.f;
    }
    animData.deltaTimeSum += deltaTime;
    return false;
}


// use a unique hardcoded function to update frames because the ground blood frames are not stored linearly
void GroundBlood::UpdateGroundBloodAnim(float deltaTime) {
    if(animData.currFrame == -1)
        return;
    animData.deltaTimeSum += deltaTime;
    if(animData.deltaTimeSum >= animData.animSpeed) {
        int posX = (animData.currFrame%6)*210;
        int posY = 850 + (animData.currFrame/6)*250;
        animData.textureFrame.position = {posX, posY};
        animData.currFrame++;
        if (animData.currFrame >= animData.totalFrames) {
            animData.currFrame = -1;
        }
        animData.deltaTimeSum = 0.f;
    };
}


// Same logic as rotating the bloodspray with projectiles, but this also rotates the collider
void GroundBlood::SetRotation(sf::Vector2f incomingProjectilePos) {
    sf::Vector2f difference = GetPosition() - incomingProjectilePos;
    float radians = atan2(difference.y, difference.x) + RandomUtil::GetRandomFloat(0,60.f);
    sf::Angle angle = sf::radians(radians);
    setRotation(angle);
    move(difference.normalized()* ON_HIT_GROUND_POSITION_OFFSET);
    collider.setOrigin(collider.getGeometricCenter());
    collider.setRotation(angle);
    collider.move(difference.normalized()* ON_HIT_GROUND_POSITION_OFFSET);
}

// Render the ground blood collider
void GroundBlood::RenderCollider(sf::RenderWindow& window) {
    window.draw(collider);
}

