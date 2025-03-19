#include "fx/Shell.h"
#include "util/RandomUtil.h"
#include <iostream>

static constexpr float PI = 3.141592;
static constexpr float MIRROR_NEG_Y_AXIS_BOUND = PI/2.f; // pi/2 radians , greater than half of pi
static constexpr float MIRROR_POS_Y_AXIS_BOUND =  -PI/2.f; // -pi/2 pi radians , less than -half of pi
static constexpr float FALL_DISTANCE = 35.f;
static constexpr float MAX_TIME_ALIVE = 1.4f;



Shell::Shell(sf::Vector2f position, sf::Vector2f weaponRelative) 
    :   rotationSpeed(RandomUtil::GetRandomFloat(-0.1,0.1)),
        velocity(CreateVelocity(weaponRelative)),
        fallDistSum(0.f)
    
{  // Pixels per second squared
    shape.setSize({5.f, 1.f});
    shape.setFillColor(sf::Color{207, 166, 35});
    shape.setPosition(position);
    angle = sf::radians(rotationSpeed);
}

sf::Vector2f Shell::CreateVelocity(sf::Vector2f weaponRelative){
    
    float radians = atan2(weaponRelative.y, weaponRelative.x);
    sf::Vector2f perp;
    if(radians > MIRROR_NEG_Y_AXIS_BOUND || radians < MIRROR_POS_Y_AXIS_BOUND){
        // shoot shell facing right
        // y, -x perp clockwise
        perp = {weaponRelative.y, -weaponRelative.x};
    }else {
        // shoot shell facing left
        // -y, x counter clockwise
        perp = {-weaponRelative.y, weaponRelative.x};
    }
    // random spread of shells
    float offset = RandomUtil::GetRandomFloat(-115.f,115.f);
    perp = {perp.x+offset, perp.y+offset};
    auto norm = perp.normalized();
    // speed of falling shells
    return perp/5.f;
}


// return true when it's done
bool Shell::Update(float deltaTime) {
    fallDistSum += (velocity*deltaTime).length();
    if(fallDistSum < FALL_DISTANCE){
        shape.move(velocity * deltaTime);   // Update position
    }
    if(deltaTimeSum > MAX_TIME_ALIVE) {
        return true;
    }
    shape.rotate(angle);
    deltaTimeSum += deltaTime;
    return false;
}


void Shell::Draw(sf::RenderWindow& window) {
    window.draw(shape);
}

