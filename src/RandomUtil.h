#pragma once
#include <random>

class RandomUtil {
private:
    static std::random_device rd;
    static std::mt19937 rng;
    static std::uniform_real_distribution<float> realDist;
    static std::uniform_int_distribution<int> intDist;
    
public:
    static void Initialize();

    static float GetRandomFloat(float min, float max);

    static int GetRandomInt(int min, int max);
};