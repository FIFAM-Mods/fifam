#pragma once
#include "FifamTypes.h"

namespace foom {

struct nation;
struct city;
struct language;

struct local_region {
    Int mID = 0;
    String mName;
    nation *mNation = 0;
    Int mPopulation = 0;
    Int mWeather = 0;
    Vector<city *> mVecCities;

    struct local_region_language {
        language *mLanguage = 0;
        Int mPercentage = 0;
    };

    Vector<local_region_language> mVecLanguages;
};

}
