#pragma once
#include "FifamCompetition.h"
#include "FifamPoolSorting.h"

// @since FM07
class FifamCompPool : public FifamCompetition {
public:
    // @since FM07
    // number of pools
    UInt mNumPools = 0;
    // @since FM07
    // sorting type
    FifamPoolSorting mSorting;
    // @since FM07
    // can contain reserve teams
    Bool mReserveTeamsAllowed = false;
    // @since FM07
    // teams from these competitions can't be added
    Vector<FifamCompetition *> mCompConstraints;
    // @since FM07
    // bonuses
    Array<UInt, 4> mBonuses = {};

    FifamCompDbType GetDbType() const override;
    void Read(FifamReader &reader, FifamDatabase *database, FifamNation nationId) override;
    void Write(FifamWriter &writer, FifamDatabase *database, FifamNation nationId) override;
};
