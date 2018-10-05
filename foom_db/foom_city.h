#pragma once
#include "FifamTypes.h"

namespace foom {

struct nation;

struct city {
    Int mID = 0;
    String mName;
    nation *mNation = nullptr;
    Int mInhabitants = 0;
    Float mLatitude = 0;
    Float mLongitude = 0;
};

}
