#pragma once
#include <iostream>
#include <SFML/Graphics.hpp>
#include "TextureUtil.h"
#include "AnimUtil.h"
#include "Player.h"
#include "Map.h"
#include "Monster.h"
#include "AoE.h"
#include "Projectile.h"
#include "Weapon.h"
#include "Blood.h"
#include "Footprint.h"
#include "MonsterFactory.h"
#include "RandomUtil.h"
#include "BatchRenderer.h"


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
    std::vector<Monster> monsters;
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