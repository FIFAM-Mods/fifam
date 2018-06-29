#pragma once

#include "FifamDbEnvironment.h"

template<size_t Version, bool Original>
class FifamDbTest {
public:
    FifamDbTest() {
        FifamDatabase *db = GetEnvironment<FifamDbEnvironment<Version, Original>>().GetDatabase();
        
    }
};
