#pragma once
#include <SFML/Graphics.hpp>
#include "Blood.h"
#include "AnimUtil.h"

class Blood;

inline constexpr float FOOTPRINT_DECAY_TIME = 2.f;
inline constexpr float FOOTPRINT_DT_RATE = .1f;

class Footprint : public Blood {

private:
    sf::Vector2f position;
    bool createLeftFoot;

public:
    Footprint(AnimData animData, const sf::FloatRect& globalBounds, sf::Vector2f direction, bool isLeft, float footprintDecayTimer);
   // bool Update(float deltaTime); // return true if expired, add this to remove old prints 
   // if performance issues happen from too many sprites 

};