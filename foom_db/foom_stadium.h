#pragma once
#include "FifamTypes.h"

namespace foom {

struct nation;
struct city;
struct team;

struct stadium {
    Int mID = 0;
    String mName;
    nation *mNation = nullptr;
    city *mCity = nullptr;
    team *mOwner = nullptr;
    Int mCapacity = 0;
    Int mSeatingCapacity = 0;
    Float mLatitude = 0;
    Float mLongitude = 0;
    Int mUsedByNationalTeam = 0;
};

}
