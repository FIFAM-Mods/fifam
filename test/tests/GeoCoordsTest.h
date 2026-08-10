#pragma once
#include "TestRegistry.h"
#include "FifamDbEnvironment.h"
#include "FifamGeoCoords.h"

class GeoCoordsTest {
public:
    GeoCoordsTest() {
        FifamGeoCoords coords(1048621, -6094855);
        std::wcout << coords.ToStr() << L" (" << coords.mLatitude.ToInt() << L"," << coords.mLongitude.ToInt() << L")";
    }
};

REGISTER_TEST(GeoCoordsTest)
