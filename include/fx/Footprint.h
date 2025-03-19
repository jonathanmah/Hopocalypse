#pragma once
#include <SFML/Graphics.hpp>
#include "../fx/Blood.h"

class AnimData;

inline constexpr float FOOTPRINT_DECAY_TIME = 2.f;
inline constexpr float FOOTPRINT_DT_RATE = .1f;

class Footprint : public Blood {

private:
    sf::Vector2f position;
    bool createLeftFoot;

public:
    Footprint(AnimData animData, const sf::FloatRect& globalBounds, sf::Vector2f direction, bool isLeft, float footprintDecayTimer);
};