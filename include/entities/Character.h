#pragma once
#include <SFML/Graphics.hpp>
#include "util/AnimUtil.h"
#include "core/BatchRenderer.h"
#include "Hud.h"


class GameState;
class BatchRenderer;
class PassiveEffect;
class ActiveEffect;

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
    //float flameTimer; // for flame thrower, actually need this lol not a status

    
    // i dont like this
    float knockbackDebt;
    sf::Vector2f knockbackVector;
    float slowFactor;
    float slowedTimer;
    // also paralyzed timer
    // also shrunk timer,
    // also just put this at monsters... 

    void CheckDeath();
    void TakeDamage(int damage);
    sf::FloatRect GetFootCollider();
    virtual void Draw(sf::RenderWindow& window, BatchRenderer& batchRenderer);
    inline sf::Vector2f GetPosition() {return sprite.getPosition();}
    inline sf::FloatRect GetGlobalBounds() {return sprite.getGlobalBounds();}
    inline sf::Sprite& GetSprite() { return sprite;}
    inline int GetId() {return id;}
    void UpdateFootprints(sf::Vector2f nextMoveNormalized, GameState& state, float deltaTime);
    void Knockback();

    void ApplyPassiveEffect(PassiveEffect effect);
    void ApplyActiveEffect(ActiveEffect effect);
};