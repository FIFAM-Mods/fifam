#pragma once
#include "FifamCompetition.h"

// @since FM07
class FifamCompPool : public FifamCompetition {
public:
    // @since FM07
    // max teams in pool
    UInt mNumTeams = 0;
    // @since FM07
    // sorting type
    UInt mSorting = 0;
    // @since FM07
    // can contain reserve teams
    Bool mReserveTeamsAllowed = false;
    // @since FM07
    // teams from these competitions can't be added
    Vector<FifamCompID> mCompConstraints;

    void Read(FifamReader &reader, FifamDatabase *database) {

    }

    void Write(FifamWriter &writer, FifamDatabase *database) {

    }
};
