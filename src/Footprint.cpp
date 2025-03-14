
#include "Footprint.h"
#include <iostream>
#include <cmath>
#include "TextureUtil.h"

static int footprint_count = 0;
static constexpr float FOOTPRINT_OFFSET_PIXELS = 8.f;
static constexpr float PI = 3.141592;


/*
Construct a footprint
    AnimData animData : the animation data
    sf::FloatRect& globalBounds : the global bounds of the character who is creating the footprint
    sf::Vector2f direction : the normalized vector in the direction of the characters next move
    bool isLeft : True should create a left footprint, False should create a right footprint
    float footprintDecayTimer : this is the elapsed time since the first footprint of this sequence was created. This
            will reset every time a character's foot collider intersects with the ground blood collider
*/
Footprint::Footprint(AnimData animData, const sf::FloatRect& globalBounds, sf::Vector2f direction, bool createLeftFoot, float footprintDecayTimer) 
    :   Blood(animData,globalBounds.position),
        createLeftFoot(createLeftFoot) {
    
    // Create footprint, apply transformations
    setScale({0.3f, 0.3f});
    move({globalBounds.size.x/2, globalBounds.size.y});
    setRotation(direction.angle());
    sf::Angle angle(sf::radians(PI/2));
    rotate(angle);
    tempTexture = TextureUtil::LoadTexture("../assets/textures/footprints.png");
    
    // adjust the footprints apart with a perpendicular vector for direction and offset for magnitude
    if(createLeftFoot){
        move(sf::Vector2f{direction.y, -direction.x}*FOOTPRINT_OFFSET_PIXELS);
    } else {
        move(sf::Vector2f{-direction.y, direction.x}*FOOTPRINT_OFFSET_PIXELS);
    }
    // slowly adjust the alpha channel to make footprints disappear over time until a character walks on blood again
    // sf::Color colour = getColor();
    float decayTimeRatio = (FOOTPRINT_DECAY_TIME-footprintDecayTimer) / FOOTPRINT_DECAY_TIME;
    float newAlphaChannel = (1-decayTimeRatio) * 255;
    colour.a = static_cast<u_int8_t>(newAlphaChannel); //sf::Color{colour.r, colour.g, colour.b, static_cast<u_int8_t>(newAlphaChannel)});
    footprint_count++;
    std::cout << 'footprint count : ' << footprint_count << std::endl;
}