#pragma once
#include <iostream>
#include <SFML/Graphics.hpp>
#include "fx/Blood.h"
#include "fx/Footprint.h"
#include "entities/Player.h"
#include "entities/monster/Monster.h"
#include "environment/Map.h"
#include "core/BatchRenderer.h"
#include "weapons/Projectile.h"
#include "fx/DamageNumberManager.h"


class GameState {

public:
    int MONSTER_HITBOX = 0;
    int PLAYER_HITBOX = 0;
    int WEAPON_HITBOX = 0;
    int PROJECTILE_HITBOX = 0;
    int AOE_HITBOX = 0;

    sf::RenderWindow window;
    std::unique_ptr<BatchRenderer> batchRenderer;
    std::unique_ptr<DamageNumberManager> damageNumberManager;
    std::unique_ptr<Map> map;
    std::vector<std::unique_ptr<Player>> players;
    std::vector<std::unique_ptr<Monster>> monsters;
    std::vector<std::unique_ptr<Projectile>> projectiles;
    std::vector<std::unique_ptr<Weapon>> weapons;
    std::vector<Blood> bloodSpray;
    std::vector<GroundBlood> groundBlood;
    std::vector<Footprint> footprints;
    std::vector<std::unique_ptr<AoE>> aoe;

    // write a shader to blend in status effects with monster textures...


    GameState();

    void SetRandomMonsterSpawn(int count);
    void SetSingleTest();
    void SetCollateralLineup();
    void RenderCharacters();

    // initialize window settings, map, other settings

    void InitPlayers();
    void InitMonsters();

    void Update(float deltaTime);

    void Render();
};