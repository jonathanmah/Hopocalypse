#pragma once
#include <SFML/Graphics.hpp>
#include "AoE.h"

class GameState;

struct ExplosionData {
    int damage;
    float radius;
    float delay;
    float scale;
    bool setParalyze = false;
    bool setFire = false;
};

class Explosion : public AoE {

public:
    ExplosionData explosionData;

    Explosion(AnimData animData, sf::Vector2f position, ExplosionData explosionData);
    bool Update(GameState& state, float deltaTime) override;
    void DamageNeighbours(GameState& state);
    void DrawHitbox(sf::RenderWindow& window) override;
};