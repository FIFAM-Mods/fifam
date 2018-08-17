#pragma once

#include "FifamTypes.h"
#include "FifamCompID.h"

class FifamCompetition {
public:
    enum DbType {
        Round,
        Pool,
        League,
        Cup,
        Rel1,
        Rel2,
        Rel3,
        RelSwitherland,
        RelScotland1,
        RelScotland2
    } mKind = Round;

    FifamCompID mID;
    UChar mNumSubsAllowed = 0;
    UChar mLeagueLevel = 0; // 0 = highest - international competitions always have 0
    Array<FifamCompID, 32> mPredecessors;
    Array<FifamCompID, 32> mSuccessors;
};
