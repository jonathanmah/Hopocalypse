#pragma once
#include <SFML/Graphics.hpp>
#include "util/AnimUtil.h"
#include "Hud.h"

class GameState;

class Character {

protected:
    sf::Sprite sprite;
    AnimData animData;
    bool createLeftFootNext;
    float footprintDecayTimer;
    float footprintDtSumFrame;
    float scale;
    float movementSpeed;
    int health;
    int id;

    Character(AnimData animData, sf::Vector2f position, int health, float movementSpeed = 5.f, float scale = 3.f, int id = -1);
    virtual void DrawHitbox(sf::RenderWindow& window);
    void UpdateCollisions(GameState& state);
    void UpdateHealth(int damage);
public:
    bool isAlive;
    Hud hud;

    sf::FloatRect GetFootCollider();
    virtual void Draw(sf::RenderWindow& window);
    inline sf::Vector2f GetPosition() {return sprite.getPosition();}
    inline sf::FloatRect GetGlobalBounds() {return sprite.getGlobalBounds();}
    inline sf::Sprite& GetSprite() { return sprite;}
    inline int GetId() {return id;}
    void UpdateFootprints(sf::Vector2f nextMoveNormalized, GameState& state, float deltaTime);




    inline float GetFootprintDecayTimer() const {return footprintDecayTimer;}
    inline void SetFootprintDecayTimer(float updatedDecayTimer) {footprintDecayTimer = updatedDecayTimer;}
    inline float GetFootprintDtSumFrame() const {return footprintDtSumFrame;}
    inline void SetFootprintDtSumFrame(float updatedDtSumFrame) {footprintDtSumFrame = updatedDtSumFrame;}
    inline bool GetCreateLeftFootNext() const {return createLeftFootNext;}
    inline void SetCreateLeftFootNext(bool createLeftNext) {createLeftFootNext = createLeftNext;};
};