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

int Random::Select(std::vector<int> const &vec) {
    if (vec.empty())
        return -1;
    if (vec.size() == 1)
        return vec[0];
    return vec[Get(0, vec.size() - 1)];
}
