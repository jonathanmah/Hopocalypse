#pragma once
#include <SFML/Graphics.hpp>
#include "util/AnimUtil.h"
#include "core/BatchRenderer.h"
#include "entities/Hud.h"


class GameState;
class BatchRenderer;

class Character {
    
public:
    Hud hud;
    AnimData animData;
    sf::Sprite sprite;
    int health;
    int id = -1;
    float scale;
    float movementSpeed;


    Character(sf::Vector2f position, AnimData animData, sf::Color healthColour, sf::Color damageColour, int health, float movementSpeed, float scale);
    virtual void UpdateCollisions(GameState& state) {return;}
    bool IsDead();
    void TakeDamage(int damage);
    float GetYOrdering();
    virtual void Draw(sf::RenderWindow& window, BatchRenderer& batchRenderer);
    inline sf::Vector2f GetPosition() {return sprite.getPosition();}
    inline sf::FloatRect GetGlobalBounds() {return sprite.getGlobalBounds();}
    inline sf::Sprite& GetSprite() { return sprite;}
    inline int GetId() {return id;}
};