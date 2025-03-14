#include "Blood.h"
#include <iostream>
#include <cmath>
#include "RandomUtil.h"
#include "Footprint.h"
#include "TextureUtil.h"



static int bloodcount = 0;
sf::Texture* Blood::texture = nullptr;

// 2PI = 360deg, PI = 180deg, PI/2 = 90deg, PI/4 = 45deg
static constexpr float PI = 3.141592;
static constexpr float ON_HIT_SPRAY_ROTATION_OFFSET = PI/4.f;
static constexpr float ON_HIT_SPRAY_POSITION_OFFSET = 50.f; 
static constexpr float ON_HIT_GROUND_ROTATION_OFFSET = 0.f;
static constexpr float ON_HIT_GROUND_POSITION_OFFSET = 10.f;

const AnimData sprayAnimations[6] = {
    AnimUtil::BloodAnim::splatAnim1, 
    AnimUtil::BloodAnim::splatAnim2, 
    AnimUtil::BloodAnim::splatAnim3,
    AnimUtil::BloodAnim::splatAnim4, 
    AnimUtil::BloodAnim::splatAnim5, 
    AnimUtil::BloodAnim::splatAnim6
};

/*
Construct a blood
    
AnimData animData : given the animation data
sf::Vector2f position : starting position of the projectile in pixels
     
#TODO i think the calling function figures out rotation details with projectile direction,  that logic should probably 
live here
*/

// position center of character global bounds
Blood::Blood(AnimData animData, sf::Vector2f position) : animData(animData), colour(sf::Color::White), tempTexture(TextureUtil::LoadTexture("../assets/textures/blood.png")){
    //sprite.setTextureRect(sf::IntRect(animData.textureFrame.position, animData.textureFrame.size));
    setOrigin({static_cast<float>(animData.textureFrame.size.x)/2.f, static_cast<float>(animData.textureFrame.size.y)/2.f});
    //std::cout << "Texture Frame size : " << animData.textureFrame.size.x << "," << animData.textureFrame.size.y << std::endl;
    //std::cout << "Localized Origin coordinates : " <<  animData.textureFrame.size.x/2.f << "," << animData.textureFrame.size.y/2.f << std::endl;
    setPosition({position.x, position.y});
    setScale({1.5f, 1.5f});
    bloodcount++;
    //std::cout << "blood count : " << bloodcount << std::endl;
}

// For each bloodspray, update frame or delete it if it's finished the sequence
// For each groundblood, update the ground blood anim 
void Blood::Update(std::vector<Blood>& bloodSpray, std::vector<GroundBlood>& groundBlood, float deltaTime) {
    std::vector<Blood>::iterator it;
    for(auto it = bloodSpray.begin(); it != bloodSpray.end();){
        if(AnimUtil::UpdateFrameAnim((*it).GetAnimData(), deltaTime)){
            it = bloodSpray.erase(it);
        } else {
            ++it;
        }
    }
    for(GroundBlood& blood : groundBlood) {
       blood.UpdateGroundBloodAnim(deltaTime);
    }
    // #TODO SAVE THIS FOR LATER, MAY NEED TO DELETE FOOTSTEPS LATER FOR PERFORMANCE
    // for(auto it = footprints.begin(); it != footprints.end();){ 
    //     if((*it).Update(deltaTime)){ // would change Update to return true when it's expired
    //         it = footprints.erase(it);
    //     } else {
    //         ++it;
    //     }
    // }
}

// Rotate the blood spray object based on the direction of the incoming projectile, so it looks like it sprays through the character
void Blood::SetRotation(sf::Vector2f incomingProjectilePos) {
    sf::Vector2f difference = GetPosition() - incomingProjectilePos;
    float radians = atan2(difference.y, difference.x) + ON_HIT_SPRAY_ROTATION_OFFSET;
    sf::Angle angle = sf::radians(radians);
    setRotation(angle);
    move(difference.normalized()*ON_HIT_SPRAY_POSITION_OFFSET);
}

// #DEV MODE  render the hitbox of a blood object // this could be a common utility function outside of blood maybe...
// void DrawHitboxt(sf::Sprite& sprite, sf::RenderWindow& window) {
//     sf::FloatRect bounds = sprite.getGlobalBounds();
//     sf::RectangleShape rect(sf::Vector2f(bounds.size));
//     rect.setPosition(bounds.position);
//     rect.setFillColor(sf::Color::Transparent);
//     rect.setOutlineColor(sf::Color::Red);
//     rect.setOutlineThickness(2.0f);
//     window.draw(rect);
// }

// render a blood object
// void Blood::Draw(sf::RenderWindow& window) {
//     window.draw(sprite);
//     //DrawHitboxt(sprite, window);
// }


// use different blood spray animation each time a projectile hits
AnimData Blood::GetNextSprayAnim() {
    AnimData animData = sprayAnimations[RandomUtil::GetRandomInt(0,5)];
    return animData;
}

// update blood spray and ground blood after projectile collision
void Blood::UpdateProjectileBlood(sf::Vector2f incomingProjectilePos, sf::FloatRect characterGlobalBounds, 
    std::vector<Blood>& bloodSpray, std::vector<GroundBlood>& groundBlood){

    Blood newBloodSpray(Blood::GetNextSprayAnim(), characterGlobalBounds.getCenter());
    newBloodSpray.SetRotation(incomingProjectilePos);
    bloodSpray.push_back(newBloodSpray);
    GroundBlood newGroundBlood(AnimUtil::BloodAnim::trailAnim, characterGlobalBounds.getCenter());
    newGroundBlood.SetRotation(incomingProjectilePos);
    groundBlood.push_back(newGroundBlood);
}


// render all bloodspray, groundblood, and footprints
void Blood::RenderBlood(std::vector<Blood>& bloodSpray, std::vector<GroundBlood>& groundBlood, std::vector<Footprint>& footprints, BatchRenderer& batchRenderer, sf::RenderWindow& window) {
    batchRenderer.BatchRenderFrames(groundBlood);
    batchRenderer.BatchRenderFrames(footprints);
    batchRenderer.BatchRenderFrames(bloodSpray);
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
    collider.setRadius(21.f);
    collider.setOrigin(collider.getGeometricCenter());
    collider.setFillColor(sf::Color::Blue);
    collider.setPosition(GetPosition());
    collider.setScale({2.f,1.f});
    // overwrites completely 
    setScale({.5f,.5f});
    tempTexture = TextureUtil::LoadTexture("../assets/textures/ground_blood.png");
}

// use a unique hardcoded function to update frames because the ground blood frames are not stored linearly
void GroundBlood::UpdateGroundBloodAnim(float deltaTime) {
    if(animData.currFrame == -1)
        return;
    animData.deltaTimeSum += deltaTime;
    if(animData.deltaTimeSum >= animData.animSpeed) {
        int posX = (animData.currFrame%3)*500;
        int posY = (animData.currFrame/3)*500;
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

