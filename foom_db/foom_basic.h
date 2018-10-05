#pragma once
#include "FifamTypes.h"
#include "db_map.h"

namespace foom {

template<typename T>
class entity {
    T *ptr = nullptr;
    Int id = -1;

    T *operator->() {
        return ptr;
    }

    T *operator T*() {
        return ptr;
    }

    T const *operator T const *() {
        return ptr;
    }

    void resolve(db_map const &map) {
        bool found = map_find(map, id);
    }
};

}
