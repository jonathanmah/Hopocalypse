#pragma once
#include <SFML/Graphics.hpp>
#include <random>

class Monster;

class MonsterFactory {

private:
    std::random_device seedGen;
    std::mt19937 gen;
    std::uniform_int_distribution<int> windowWidthDist;
    std::uniform_int_distribution<int> windowHeightDist;
    std::uniform_int_distribution<int> windowSideDist;
    std::uniform_int_distribution<int> monsterDist;
    std::uniform_int_distribution<int> positionOffsetDist;
    

    sf::Vector2f GetRandomPosition(int windowSide);

public:
    MonsterFactory();
    std::vector<Monster> GetMonsters(int spawnCount);
};