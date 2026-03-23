#pragma once
#include "FifamTypes.h"

namespace foom {

struct nation;
struct local_region;
struct language;

struct city {
    Int mID = 0;
    String mName;
    Array<String, 6> mTranslatedNames;
    nation *mNation = nullptr;
    Int mInhabitants = 0;
    Float mLatitude = 0;
    Float mLongitude = 0;
    Int mAltitude = 0;
    local_region *mRegion = nullptr;
    language *mLanguage = nullptr;
    Int mAttraction = 0;
    Int mWeather = 0;
};

}
