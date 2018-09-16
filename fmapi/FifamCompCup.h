#pragma once
#include "FifamCompetition.h"
#include "FifamRoundID.h"
#include "FifamBeg.h"
#include "FifamFlags.h"
#include "FifamCupSystemType.h"

// @since FM07
class FifamCompCup : public FifamCompetition {
public:
    // @since FM07
    // cup template type
    FifamCupSystemType mCupTemplate;
    // @since FM07
    // when to draw teams
    UShort mDrawPeriodInWeeks = 0;
    // @since FM07
    // total number of teams in cup
    UInt mNumTeams = 0;
    // @since FM07
    // first season calendar
    Vector<UShort> mFirstSeasonMatchdays;
    // @since FM07
    // second season calendar
    Vector<UShort> mSecondSeasonMatchdays;
    // @since FM07
    class Round {
    public:
        // @since FM07
        // round type (final, sem-final, etc.)
        FifamRoundID mRoundID;
        // @since FM07
        // how many teams in this round
        UShort mTeamsRound = 0;
        // @since FM07
        // how many teams joined the cup since this round
        UShort mNewTeamsRound = 0;
        // @since FM07
        // index of the first beg
        UShort mStartBeg = 0;
        // @since FM07
        // index of the end beg (non-inclusive)
        UShort mEndBeg = 0;
        // @since FM07
        // round flags
        FifamFlags<FifamBeg> mFlags = FifamBeg::_1stLeg | FifamBeg::WithExtraTime | FifamBeg::WithPenalty;
        // @since FM07
        // bonuses
        Array<UInt, 4> mBonuses = {};
    };
    // @since FM07
    // rounds
    Vector<Round> mRounds;

    FifamCompDbType GetDbType() const override;
    void Read(FifamReader &reader, FifamDatabase *database, FifamNation nationId) override;
    void Write(FifamWriter &writer, FifamDatabase *database, FifamNation nationId) override;
};
