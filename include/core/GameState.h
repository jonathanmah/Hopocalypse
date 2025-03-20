#pragma once
#include <iostream>
#include <SFML/Graphics.hpp>
#include "fx/Blood.h"
#include "fx/Footprint.h"
#include "entities/Player.h"
#include "entities/Monster.h"
#include "environment/Map.h"
#include "core/BatchRenderer.h"
#include "weapons/Projectile.h"

class GameState {
private:
    void SetRandomMonsterSpawn(int count);
    void SetSingleTest();
    void SetCollateralLineup();

    void RenderCharacters();

public:

    sf::RenderWindow window;
    std::unique_ptr<BatchRenderer> batchRenderer;
    std::unique_ptr<Map> map;
    std::vector<Player> players;
    std::vector<std::unique_ptr<Monster>> monsters;
    std::vector<std::unique_ptr<Projectile>> projectiles;
    std::vector<std::unique_ptr<Weapon>> weapons;
    std::vector<Blood> bloodSpray;
    std::vector<GroundBlood> groundBlood;
    std::vector<Footprint> footprints;
    std::vector<std::unique_ptr<AoE>> aoe;

    GameState();

    // initialize window settings, map, other settings

    void InitPlayers();
    void InitMonsters();

    void Update(float deltaTime);

    void Render();
};