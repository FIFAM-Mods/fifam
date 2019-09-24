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
    template<typename Type, size_t Size>
    static Type SelectFromArray(std::array<Type, Size> const &ary) {
        if (ary.empty())
            return Type();
        if (ary.size() == 1)
            return ary[0];
        return ary[Get(0, ary.size() - 1)];
    }

};
