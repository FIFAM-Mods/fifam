#pragma once
#include "FifamTypes.h"

namespace foom {

struct team;

struct derby {
    Int mID = 0;
    String mName;
    String mShortName;
    String mAlternativeName;
    String mAlternativeShortName;
    team *mTeam1 = nullptr;
    team *mTeam2 = nullptr;
    Int mWorldReputation = 0;
    Int mNationalReputation = 0;
};

}
