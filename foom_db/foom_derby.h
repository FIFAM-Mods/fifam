#pragma once
#include "FifamTypes.h"

namespace foom {

struct team;

struct derby {
    Int mID = 0;
    String mName;
    Array<String, 6> mTranslatedNames;
    String mShortName;
    Array<String, 6> mTranslatedShortNames;
    String mAlternativeName;
    Array<String, 6> mTranslatedAlternativeNames;
    String mAlternativeShortName;
    Array<String, 6> mTranslatedAlternativeShortNames;
    team *mTeam1 = nullptr;
    team *mTeam2 = nullptr;
    Int mWorldReputation = 0;
    Int mNationalReputation = 0;
};

}
