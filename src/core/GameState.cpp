#include "core/GameState.h"
#include "util/TextureUtil.h"
#include "util/AnimUtil.h"
#include "entities/Player.h"
#include "environment/Map.h"
#include "entities/monster/Monster.h"
#include "fx/AoE.h"
#include "weapons/Projectile.h"
#include "weapons/Weapon.h"
#include "entities/monster/MonsterFactory.h"
#include "util/RandomUtil.h"
#include "core/BatchRenderer.h"
#include "entities/monster/derived/Zombie.h"
#include "entities/monster/derived/BigDemon.h"
#include "entities/monster/derived/SmallDemon.h"
#include "entities/monster/derived/Wolf.h"

GameState::GameState(){
    MONSTER_HITBOX = 0;
    PLAYER_HITBOX = 0;
    PROJECTILE_HITBOX = 0;
    AOE_HITBOX = 0;

    RandomUtil::Initialize();
    TextureUtil::SetStaticMemberTextures();
    sf::ContextSettings settings;
    settings.antiAliasingLevel = 0;
    window =  sf::RenderWindow{sf::VideoMode({1200, 720}), "CMake exe", sf::State::Windowed, settings};
    window.setFramerateLimit(60);
    batchRenderer = std::make_unique<BatchRenderer>(window);
    damageNumberManager = std::make_unique<DamageNumberManager>();
    map = std::make_unique<Map>(*TextureUtil::GetTexture("../assets/textures/tilesheet.png"));
    map->Load();
    InitPlayers();
    InitMonsters();
}

void GameState::SetRandomMonsterSpawn(int count){
    MonsterFactory factory;
    monsters = factory.GetMonsters(count);
}

void GameState::SetSingleTest(){
    std::unique_ptr<Monster> zombie = std::make_unique<Zombie>(sf::Vector2f{100.f,400.f});
    zombie->disabledMovement = true;
    std::unique_ptr<Monster> smallDemon = std::make_unique<SmallDemon>(sf::Vector2f{350.f,400.f});
    smallDemon->disabledMovement = true;
    std::unique_ptr<Monster> bigDemon = std::make_unique<BigDemon>(sf::Vector2f{500.f,400.f});
    bigDemon->disabledMovement = true;
    std::unique_ptr<Monster> wolf = std::make_unique<Wolf>(sf::Vector2f{500.f,300.f});
    wolf->disabledMovement = false;

    // monsters.push_back(std::move(zombie));
    // monsters.push_back(std::move(smallDemon));
    // monsters.push_back(std::move(bigDemon));
    //monsters.push_back(std::move(wolf));
}

void GameState::SetCollateralLineup(){
    for(float i = 0; i < 5; i++) {
        std::unique_ptr<Monster> zombie = std::make_unique<Zombie>(sf::Vector2f{200.f*i,400.f});
        zombie->disabledMovement = true;
        monsters.push_back(std::move(zombie)); // check later if have to do this way
    }
}

void GameState::InitPlayers() {
    players.push_back(std::make_unique<Player>(sf::Vector2f{700,600}, AnimUtil::PlayerAnim::idle)); // why cant emplace back
}

void GameState::InitMonsters() {
    SetRandomMonsterSpawn(5);
    //SetCollateralLineup();
    //SetSingleTest();
}

void GameState::Update(float deltaTime) {
    window.clear(sf::Color::Black);
    // ADD LATER
    //damageNumberManager->Update(deltaTime);

    map->Update(deltaTime);
    Projectile::UpdateProjectiles(*this, deltaTime);

    // // update the blood spray and ground blood animations
    Blood::Update(*this, deltaTime);
    AoE::UpdateAoE(*this, deltaTime);

    for (auto& player : players) {
        // update player movement/animations, projectiles shot, create footprints intersecting with ground blood
        player->Update(*this, deltaTime);
    }
    
    // update monsters
    for(auto it = monsters.begin(); it != monsters.end();) {
        if((**it).Update(*this, deltaTime)){
            it = monsters.erase(it);
        } else {
            ++it;
        }
    }
}

void GameState::RenderCharacters() {
    std::vector<std::reference_wrapper<Character>> drawCharacters;
    for (auto& monster: monsters) {
        drawCharacters.push_back(*monster);
    }
    for (auto& player: players) {
        drawCharacters.push_back(*player);
    }
    batchRenderer->BatchRenderCharacters(drawCharacters);
}

void GameState::Render() {
    //
    map->Draw(window);
    Blood::RenderBlood(*this, window);
    RenderCharacters();
    
    Projectile::RenderProjectiles(*this, false);
    AoE::RenderAoE(*this, false); // render foreground aoe and bg separately...
    StatusEffect::RenderStatusEffects(monsters, window, *batchRenderer);
    batchRenderer->RenderFlameTriangles(); // render flamethrower particles
    
    if(MONSTER_HITBOX){
        for(auto& monster : monsters){
            monster->DebugHitbox(*this);
        }
    }
    if(PLAYER_HITBOX){
        for(auto& player: players){
            player->DrawHitbox(window);
        }
    }
    // ADD LATER
    //damageNumberManager->Draw(window);

    window.display();
}