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
    float scale;
    float movementSpeed;

    Character(AnimData animData, sf::Vector2f position, int health, float movementSpeed = 5.f, float scale = 3.f, int id = -1);
    void UpdateCollisions(GameState& state);
public:
    bool isAlive;
    int health;
    int id;
    Hud hud;
    AnimData animData;

    void CheckDeath();
    void TakeDamage(int damage);
    virtual void Draw(sf::RenderWindow& window, BatchRenderer& batchRenderer);
    inline sf::Vector2f GetPosition() {return sprite.getPosition();}
    inline sf::FloatRect GetGlobalBounds() {return sprite.getGlobalBounds();}
    inline sf::Sprite& GetSprite() { return sprite;}
    inline int GetId() {return id;}
};