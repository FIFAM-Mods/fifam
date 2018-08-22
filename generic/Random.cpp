#include "Random.h"

std::mt19937 &Random::GetEngine() {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    return gen;
}

int Random::Get(int min, int max) {
    std::uniform_int_distribution<> dis(min, max);
    return dis(GetEngine());
}
