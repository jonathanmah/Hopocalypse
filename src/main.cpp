#include <iostream>
#include <SFML/Graphics.hpp>
#include "TextureUtil.h"
#include "AnimUtil.h"
#include "Player.h"
#include "Map.h"
#include "Monster.h"
#include "Projectile.h"
#include "Weapon.h"
#include "Blood.h"
#include "Footprint.h"
#include "MonsterFactory.h"
#include "RandomUtil.h"

////////// ENV SETUP IGNORE //////////////////// ENV SETUP IGNORE //////////
void SetRandomMonsterSpawn(std::vector<Monster>& monsters, int count){
    MonsterFactory factory;
    monsters = factory.GetMonsters(count);
}

void SetSingleTest(std::vector<Monster>& monsters){
    Monster bigDemon{AnimUtil::MonsterAnim::BigDemonAnim::walkAnim, {400,400}, 100, 1.f};
    bigDemon.disabledMovement = true;
    monsters.push_back(std::move(bigDemon));
}

void SetCollateralLineup(std::vector<Monster>& monsters){
    for(float i = 0; i < 5; i++) {
        Monster bigDemon{AnimUtil::MonsterAnim::BigDemonAnim::walkAnim, {150+100*i,300}, 100, 1.f};
        bigDemon.disabledMovement = true;
        monsters.push_back(std::move(bigDemon));
    }
}



////////// ENV SETUP IGNORE //////////////////// ENV SETUP IGNORE //////////



sf::RenderWindow SetupWindow() {
    // Configure window, frame rate, VSync
    sf::ContextSettings settings;
    settings.antiAliasingLevel = 8;
    sf::RenderWindow window(sf::VideoMode({1200, 720}), "CMake exe", sf::State::Windowed, settings);
    window.setFramerateLimit(60);
    return window;
}

void LoadPlayers(std::vector<Player>& players) {
    Player princess(AnimUtil::PlayerAnim::PrincessAnim::standAnim, {1200/2,700/2});
    players.push_back(std::move(princess));
}

void LoadMonsters(std::vector<Monster>& monsters) {
    //SetRandomMonsterSpawn(monsters, 100);
    SetCollateralLineup(monsters);
    // Monster bigDemon{AnimUtil::MonsterAnim::BigDemonAnim::walkAnim, {400,400}, 100, 1.f};
    // Monster bigSlug{AnimUtil::MonsterAnim::BigSlugAnim::walkAnim, {-500,500}, 100, 20.f};
    // Monster smallDemon{AnimUtil::MonsterAnim::SmallDemonAnim::walkAnim, {200,800}, 100, 3.f};
    // monsters.push_back(std::move(bigDemon));
    // monsters.push_back(std::move(bigSlug));
    // monsters.push_back(std::move(smallDemon));}
}

void UpdateSprites(std::vector<Player>& players, std::vector<Monster>& monsters, float deltaTime, sf::RenderWindow& window, std::vector<std::unique_ptr<Projectile>>& projectiles, 
    std::vector<Blood>& bloodSpray, std::vector<GroundBlood>& groundBlood, std::vector<Footprint>& footprints) {
    for (Player& player : players) {
        // update player movement/animations, projectiles shot, create footprints intersecting with ground blood
        player.Update(deltaTime, monsters, window, footprints, groundBlood, projectiles);
    }
    // #TODO this inplace deletion with iterators and funcs return a flag is common, maybe util func?
    // Update Monsters
    for(auto it = monsters.begin(); it != monsters.end();){
        if((*it).Update(deltaTime, players, projectiles, bloodSpray, groundBlood)){
            it = monsters.erase(it);
        } else {
            ++it;
        }
    }
}

void RenderSprites(std::vector<Player>& players, std::vector<Monster>& monsters, sf::RenderWindow& window) {
    std::vector<Monster> aliveMonsters;
    std::vector<Monster> deadMonsters;
    for (Monster& monster: monsters) {
        if(monster.isAlive) {
            aliveMonsters.push_back(monster);
        } else {
            deadMonsters.push_back(monster);
        }
    }
    for (Monster& monster: deadMonsters) {
        monster.Draw(window);
    }
    for (Monster& monster: aliveMonsters) {
        monster.Draw(window);
    }
    
    for (Player& player : players) {
        player.Draw(window);
    }
}


int main() {
    RandomUtil::Initialize();
    // Setup Window
    sf::RenderWindow window = SetupWindow();
    
    // Setup Map
    Map map{*TextureUtil::LoadTexture("../assets/textures/tilesheet.png")};
    map.Load();


    // Load players - just one hardcoded player for now
    std::vector<Player> players;
    LoadPlayers(players);

    // Load monsters - populate monsters with either factory or hardcoded one for testing 
    std::vector<Monster> monsters;
    LoadMonsters(monsters);

    // Declare object vectors for rendering
    std::vector<std::unique_ptr<Projectile>> projectiles;
    std::vector<std::unique_ptr<Weapon>> weapons;
    std::vector<Blood> bloodSpray;
    std::vector<GroundBlood> groundBlood;
    std::vector<Footprint> footprints;
    
    // clock for tracking elapsed time, begins with default constructor
    sf::Clock clock;

    // Enter Game loop (capped at 60fps, windows.display() holds execution)
    while (window.isOpen()) {

        // delta time represents the time elapsed since the last frame was rendered 
        sf::Time deltaTime = clock.restart();
        float dt = deltaTime.asSeconds();

        // this is for polling events like key presses and pushes onto a queue
        // it processes one event at a time and is slow for holding keys, just using for reading window close for now
        while (const std::optional event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
                window.close();
        }
        
        // update the map (passing delta time for updating any map animations)
        map.Update(dt);

        // update position of all projectiles (#TODO projectile animations)
        Projectile::UpdateProjectiles(projectiles);

        // update the blood spray and ground blood animations
        Blood::Update(bloodSpray, groundBlood, dt);

        // #TODO this function is disgusting, we should refactor to have this as a class to access them rather than 
        // passing so many repetitive parameters.
        UpdateSprites(players, monsters, dt, window, projectiles, bloodSpray, groundBlood, footprints);

        // clear previous frame before drawing new one
        window.clear(sf::Color::Black);

        // render map
        map.Draw(window);
        // render blood objects
        Blood::RenderBlood(bloodSpray, groundBlood, footprints, window);
        // render characters
        RenderSprites(players, monsters, window);
        // render projectiles
        Projectile::RenderProjectiles(projectiles, window);
        // display to screen after finished rendering
        window.display();
        
    }

    return 0;
}