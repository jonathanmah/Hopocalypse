#pragma once
#include <SFML/Graphics.hpp>
#include "AnimUtil.h"
#include "HitboxDebugger.h"

class MuzzleFlash{

private:
    sf::Sprite sprite;
    AnimData animData;
    sf::Vector2f muzzlePosition;
    sf::Vector2f weaponBaseNormalized;
    float scale;

    void RotateMuzzleFlash();
    virtual void InitOrigin();

public:
    bool isActive;

    MuzzleFlash(AnimData animData, sf::Vector2f muzzlePosition, sf::Vector2f weaponBaseNormalized, float scale);
    bool Update(sf::Vector2f muzzlePosition, sf::Vector2f newNormalized, float deltaTime);
    void Draw(sf::RenderWindow& window);
    static AnimData GetNextFlash();
};