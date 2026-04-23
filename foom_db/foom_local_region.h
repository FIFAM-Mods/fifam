#pragma once
#include "FifamTypes.h"
#include "foom_translation.h"

namespace foom {

struct nation;
struct city;
struct language;
struct weather;

struct local_region {
    Int mID = 0;
    String mName;
    FoomTranslationArray mTranslatedNames;
    nation *mNation = 0;
    Int mPopulation = 0;
    weather *mWeather = nullptr;
    Vector<city *> mVecCities;

    struct local_region_language {
        language *mLanguage = 0;
        Int mPercentage = 0;
    };

    Vector<local_region_language> mVecLanguages;
};

}
