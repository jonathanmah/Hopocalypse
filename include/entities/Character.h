#pragma once
#include <SFML/Graphics.hpp>
#include "util/AnimUtil.h"
#include "core/BatchRenderer.h"
#include "Hud.h"

class GameState;
class BatchRenderer;

class Character {

protected:
    sf::Sprite sprite;
    
    bool createLeftFootNext;
    float footprintDecayTimer;
    float footprintDtSumFrame;
    float scale;
    float movementSpeed;

    Character(AnimData animData, sf::Vector2f position, int health, float movementSpeed = 5.f, float scale = 3.f, int id = -1);
    virtual void DrawHitbox(sf::RenderWindow& window);
    void UpdateCollisions(GameState& state);
public:
    bool isAlive;
    int health;
    int id;
    Hud hud;
    AnimData animData;

    float knockbackDebt;
    sf::Vector2f knockbackVector;
    float slowFactor;
    float slowedTimer;
    float flameTimer; // for damage  
    bool isOnFire;
    bool burnt;
    

    void TakeDamage(int damage);
    sf::FloatRect GetFootCollider();
    virtual void Draw(sf::RenderWindow& window, BatchRenderer& batchRenderer);
    inline sf::Vector2f GetPosition() {return sprite.getPosition();}
    inline sf::FloatRect GetGlobalBounds() {return sprite.getGlobalBounds();}
    inline sf::Sprite& GetSprite() { return sprite;}
    inline int GetId() {return id;}
    void UpdateFootprints(sf::Vector2f nextMoveNormalized, GameState& state, float deltaTime);
    void Knockback();



    inline float GetFootprintDecayTimer() const {return footprintDecayTimer;}
    inline void SetFootprintDecayTimer(float updatedDecayTimer) {footprintDecayTimer = updatedDecayTimer;}
    inline float GetFootprintDtSumFrame() const {return footprintDtSumFrame;}
    inline void SetFootprintDtSumFrame(float updatedDtSumFrame) {footprintDtSumFrame = updatedDtSumFrame;}
    inline bool GetCreateLeftFootNext() const {return createLeftFootNext;}
    inline void SetCreateLeftFootNext(bool createLeftNext) {createLeftFootNext = createLeftNext;};
};