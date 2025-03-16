#pragma once
#include "AnimUtil.h"
#include "Projectile.h"
#include "Blood.h"
#include "Hud.h"
#include "Footprint.h"
#include "AoE.h"
#include <SFML/Graphics.hpp>

class Projectile;

class Character {

protected:
    sf::Sprite sprite;
    AnimData animData;
    Hud hud;
    bool createLeftFootNext;
    float footprintDecayTimer;
    float footprintDtSumFrame;
    float scale;
    float movementSpeed;
    int health;
    int id;

    Character(AnimData animData, sf::Vector2f position, int health, float movementSpeed = 5.f, float scale = 3.f, int id = -1);
    virtual void DrawHitbox(sf::RenderWindow& window);
    void UpdateCollisions(std::vector<std::unique_ptr<Projectile>>& projectiles, std::vector<Blood>& bloodSpray, std::vector<GroundBlood>& groundBlood, std::vector<std::unique_ptr<AoE>>& aoe);
    void UpdateHealth(int damage);
public:
    bool isAlive;

    sf::FloatRect GetFootCollider();
    virtual void Draw(sf::RenderWindow& window);
    inline sf::Vector2f GetPosition() {return sprite.getPosition();}
    inline sf::FloatRect GetGlobalBounds() {return sprite.getGlobalBounds();}
    inline int GetId() {return id;}
    void UpdateFootprints(sf::Vector2f nextMoveNormalized, std::vector<Footprint>& footprints, std::vector<GroundBlood>& groundBlood, float deltaTime);




    inline float GetFootprintDecayTimer() const {return footprintDecayTimer;}
    inline void SetFootprintDecayTimer(float updatedDecayTimer) {footprintDecayTimer = updatedDecayTimer;}
    inline float GetFootprintDtSumFrame() const {return footprintDtSumFrame;}
    inline void SetFootprintDtSumFrame(float updatedDtSumFrame) {footprintDtSumFrame = updatedDtSumFrame;}
    inline bool GetCreateLeftFootNext() const {return createLeftFootNext;}
    inline void SetCreateLeftFootNext(bool createLeftNext) {createLeftFootNext = createLeftNext;};
};