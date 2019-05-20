#pragma once
#include <random>

class Random {
public:
    static std::mt19937 &GetEngine();
    // get random int in range [min;max]
    static int Get(int min, int max);
    static int Select(std::vector<int> const &vec);
    static unsigned int GetIndex(std::vector<double> const &dist);
    static void Shuffle(std::vector<int> &vec);
};
