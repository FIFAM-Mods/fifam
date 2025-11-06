#pragma once
#include "FifamTypes.h"

namespace foom {

template<typename T>
using db_map = Map<Int, T>;

template<typename T>
T &map_find(Map<Int, T> &map, Int key, bool *found = nullptr) {
    static T emptyOne;
    static Int lastKey = -1;
    static T *lastResult = &emptyOne;
    if (lastKey != key) {
        lastKey = key;
        auto it = map.find(key);
        if (it == map.end())
            lastResult = &emptyOne;
        else
            lastResult = (T *)(&(*it).second);
    }
    if (found)
        *found = lastResult == &emptyOne;
    return *lastResult;
}

template<typename T>
T *map_find_ptr(Map<Int, T> &map, Int key) {
    static Int lastKey = -1;
    static T *lastResult = nullptr;
    if (lastKey != key) {
        lastKey = key;
        auto it = map.find(key);
        if (it == map.end())
            lastResult = nullptr;
        else
            lastResult = &(*it).second;
    }
    return lastResult;
}

template<typename T>
class entity {
    T *ptr = nullptr;
    int id = -1;

public:
    T *operator->() {
        return ptr;
    }

    operator T *() {
        return ptr;
    }

    operator T const *() const {
        return ptr;
    }

    void resolve(Map<Int, T> const &map) {
        bool found = map_find(map, id);
    }
};

}
