#pragma once
#include "FifamCompetition.h"
#include "FifamRoundID.h"
#include "FifamBeg.h"
#include "FifamFlags.h"

// @since FM07
class FifamCompRound : public FifamCompetition {
public:
    // @since FM07
    // num of teams in round
    UInt mNumTeams = 0;
    // @since FM07
    // round type
    FifamRoundID mRoundType;
    // @since FM07
    // 1st leg flags
    FifamFlags<FifamBeg> m1stLegFlags;
    // @since FM07
    // 2nd leg flags
    FifamFlags<FifamBeg> m2ndLegFlags;
    // @since FM07
    // first season calendar
    Array<UShort, 2> mFirstSeasonMatchdays = {};
    // @since FM07
    // second season calendar
    Array<UShort, 2> mSecondSeasonMatchdays = {};
    // @since FM07
    // bonuses
    Array<UInt, 4> mBonuses = {};

    FifamCompDbType GetDbType() override;
    void Read(FifamReader &reader) override;
    void Write(FifamWriter &writer) override;
};
