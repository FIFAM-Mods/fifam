#pragma once
#include "FifamTypes.h"
#include "foom_translation.h"

namespace foom {

struct team;

struct derby {
    Int mID = 0;
    String mName;
    FoomTranslationArray mTranslatedNames;
    String mShortName;
    FoomTranslationArray mTranslatedShortNames;
    String mAlternativeName;
    FoomTranslationArray mTranslatedAlternativeNames;
    String mAlternativeShortName;
    FoomTranslationArray mTranslatedAlternativeShortNames;
    team *mTeam1 = nullptr;
    team *mTeam2 = nullptr;
    Int mWorldReputation = 0;
    Int mNationalReputation = 0;
};

}
