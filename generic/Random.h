#pragma once
#include <random>

class Random {
    static std::mt19937 &GetEngine();
public:
    // get random int in range [min;max]
    static int Get(int min, int max);
    static int Select(std::vector<int> const &vec);
};
