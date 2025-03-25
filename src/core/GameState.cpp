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
#include "entities/monsters/Zombie.h"
#include "entities/monsters/BigDemon.h"
#include "entities/monsters/SmallDemon.h"

GameState::GameState(){
    MONSTER_HITBOX = 0;
    PLAYER_HITBOX = 0;
    PROJECTILE_HITBOX = 0;
    AOE_HITBOX = 0;

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
    // BigDemon bigDemon{{100.f,100.f}};
    // SmallDemon smallDemon{{200.f,600.f}};
    std::unique_ptr<Monster> zombie = std::make_unique<Zombie>(sf::Vector2f{100.f,400.f});
    zombie->disabledMovement = true;
    std::unique_ptr<Monster> smallDemon = std::make_unique<SmallDemon>(sf::Vector2f{400.f,400.f});
    smallDemon->disabledMovement = true;
    std::unique_ptr<Monster> bigDemon = std::make_unique<BigDemon>(sf::Vector2f{700.f,400.f});
    bigDemon->disabledMovement = true;
    std::unique_ptr<Monster> bigDemon2 = std::make_unique<BigDemon>(sf::Vector2f{700.f,430.f});
    bigDemon2->disabledMovement = true;
    std::unique_ptr<Monster> bigDemon3 = std::make_unique<BigDemon>(sf::Vector2f{700.f,460.f});
    bigDemon3->disabledMovement = true;
    std::unique_ptr<Monster> bigDemon4 = std::make_unique<BigDemon>(sf::Vector2f{700.f,490.f});
    bigDemon4->disabledMovement = true;
    monsters.push_back(std::move(zombie));
    monsters.push_back(std::move(smallDemon));
    monsters.push_back(std::move(bigDemon));
    monsters.push_back(std::move(bigDemon2));
    monsters.push_back(std::move(bigDemon3));
    monsters.push_back(std::move(bigDemon4));
}

void GameState::SetCollateralLineup(){
    for(float i = 0; i < 5; i++) {
        Zombie zombie{{400.f,400.f}};
        zombie.disabledMovement = true;
        monsters.push_back(std::make_unique<Zombie>(std::move(zombie))); // check later if have to do this way
    }
}

void GameState::InitPlayers() {
    Player bunny({1200/2,700/2}, AnimUtil::PlayerAnim::stand);
    players.push_back(std::move(bunny)); // why cant emplace back
}

void GameState::InitMonsters() {
    //SetRandomMonsterSpawn(300);
    //SetCollateralLineup();
    SetSingleTest();
}

void GameState::Update(float deltaTime) {
    map->Update(deltaTime);
    Projectile::UpdateProjectiles(*this, deltaTime);

    // // update the blood spray and ground blood animations
    Blood::Update(*this, deltaTime);
    AoE::UpdateAoE(*this, deltaTime);
    //StatusEffect::UpdateStatusEffect(*this,deltaTime);

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
    std::vector<std::reference_wrapper<Character>> drawCharacters;
    for (auto& monster: monsters) {
        drawCharacters.push_back(*monster);
    }
    for (auto& player: players) {
        drawCharacters.push_back(player);
    }
    batchRenderer->BatchRenderCharacters(drawCharacters);
}

void GameState::Render() {
    window.clear(sf::Color::Black);
    map->Draw(window);
    Blood::RenderBlood(*this, window);
    RenderCharacters();
    
    Projectile::RenderProjectiles(*this, false);
    AoE::RenderAoE(*this, false); // render foreground aoe and bg separately...
    StatusEffect::RenderStatusEffects(monsters, window, *batchRenderer);
    batchRenderer->RenderFlameTriangles(); // render flamethrower particles
    

    if(MONSTER_HITBOX==1){
        for(auto& monster : monsters){
            monster->DebugHitbox(*this);
        }
    }
    window.display();
}