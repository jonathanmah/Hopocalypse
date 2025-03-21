#include "core/GameState.h"
#include "util/TextureUtil.h"
#include "util/AnimUtil.h"
#include "entities/Player.h"
#include "environment/Map.h"
#include "entities/Monster.h"
#include "fx/AoE.h"
#include "weapons/Projectile.h"
#include "weapons/Weapon.h"
#include "fx/Blood.h"
#include "fx/Footprint.h"
#include "entities/MonsterFactory.h"
#include "util/RandomUtil.h"
#include "core/BatchRenderer.h"

GameState::GameState(){
    RandomUtil::Initialize();
    TextureUtil::SetStaticMemberTextures();
    sf::ContextSettings settings;
    settings.antiAliasingLevel = 8;
    window =  sf::RenderWindow{sf::VideoMode({1200, 720}), "CMake exe", sf::State::Windowed, settings};
    window.setFramerateLimit(60);
    batchRenderer = std::make_unique<BatchRenderer>(window);
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
    Monster bigDemon{AnimUtil::BigDemonAnim::walk, {400,400}, 100, 3.f};
    bigDemon.disabledMovement = true;
    monsters.push_back(std::make_unique<Monster>(std::move(bigDemon)));
}

void GameState::SetCollateralLineup(){
    for(float i = 0; i < 5; i++) {
        Monster bigDemon{AnimUtil::BigDemonAnim::walk, {150+100*i,300}, 100, 1.f};
        bigDemon.disabledMovement = true;
        monsters.push_back(std::make_unique<Monster>(std::move(bigDemon))); // check later if have to do this way
    }
}

void GameState::InitPlayers() {
    Player bunny(AnimUtil::PlayerAnim::stand, {1200/2,700/2});
    players.push_back(std::move(bunny)); // why cant emplace back
}

void GameState::InitMonsters() {
    //SetRandomMonsterSpawn(300);
    SetCollateralLineup();
    //SetSingleTest();
}

void GameState::Update(float deltaTime) {
    map->Update(deltaTime);
    Projectile::UpdateProjectiles(*this, deltaTime);

    // // update the blood spray and ground blood animations
    Blood::Update(*this, deltaTime);
    AoE::UpdateAoE(*this, deltaTime);

    for (Player& player : players) {
        // update player movement/animations, projectiles shot, create footprints intersecting with ground blood
        player.Update(*this, deltaTime);
    }
    for(auto it = monsters.begin(); it != monsters.end();){
        if((**it).Update(*this, deltaTime)){
            it = monsters.erase(it);
        } else {
            ++it;
        }
    }
}

void GameState::RenderCharacters() {
    //#TODO will have to figure out a way to keep monsters as pointers while being able to render them in order...
    std::vector<std::reference_wrapper<Monster>> monstersAlive;
    std::vector<std::reference_wrapper<Monster>> monstersDead;
    for (auto& monster: monsters) {
        if(monster->isAlive)
            monstersAlive.push_back(*monster);
    }
    for (auto& monster: monsters) {
        if(!monster->isAlive)
            monstersDead.push_back(*monster);
    }
    batchRenderer->BatchRenderCharacters(monstersDead);
    batchRenderer->BatchRenderCharacters(monstersAlive); //remove this batch renderer func later when 
    for (Player& player: players) {
        player.Draw(window, *batchRenderer);
    }
}

void GameState::Render() {
    window.clear(sf::Color::Black);
    map->Draw(window);
    Blood::RenderBlood(*this, window);
    RenderCharacters();
    Projectile::RenderProjectiles(*this, true);
    AoE::RenderAoE(*this, true); // render foreground aoe and bg separately...
    window.display();
}