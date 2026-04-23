#pragma once
#include "FifamTypes.h"
#include "foom_translation.h"

namespace foom {

struct nation;
struct local_region;
struct language;
struct weather;

struct city {
    Int mID = 0;
    String mName;
    FoomTranslationArray mTranslatedNames;
    nation *mNation = nullptr;
    Int mInhabitants = 0;
    Float mLatitude = 0;
    Float mLongitude = 0;
    Int mAltitude = 0;
    local_region *mRegion = nullptr;
    language *mLanguage = nullptr;
    Int mAttraction = 0;
    weather *mWeather = nullptr;

    struct converter_data {
        UInt numPlayers = 0;
    } mConverterData;
};

}
