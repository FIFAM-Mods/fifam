#pragma once

#include "FifamTypes.h"
#include "FifamCompID.h"

class FifamDatabase;

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
    } mDbType = Round;

    FifamCompID mID;
    UChar mNumSubsAllowed = 0;
    UChar mLeagueLevel = 0; // 0 = highest - international competitions always have 0
    Vector<FifamCompID> mPredecessors;
    Vector<FifamCompID> mSuccessors;

    virtual void Read(FifamReader &reader, FifamDatabase *database) {

    }

    virtual void Write(FifamWriter &writer, FifamDatabase *database) {

    }
};
