#include "util/RandomUtil.h"

std::random_device RandomUtil::rd;
std::mt19937 RandomUtil::rng;
std::uniform_real_distribution<float> RandomUtil::realDist;
std::uniform_int_distribution<int> RandomUtil::intDist;

void RandomUtil::Initialize() {
    static bool initialized = false;
    if (!initialized) {
        rng.seed(rd());
        initialized = true;
    }
}

float RandomUtil::GetRandomFloat(float min, float max) {
    realDist.param(std::uniform_real_distribution<float>::param_type(min, max));
    return realDist(rng);
}

int RandomUtil::GetRandomInt(int min, int max) {
    intDist.param(std::uniform_int_distribution<int>::param_type(min, max));
    return intDist(rng);
}