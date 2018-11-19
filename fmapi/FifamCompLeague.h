#pragma once
#include "FifamCompetition.h"
#include "FifamEqualPointsSorting.h"
#include "FifamRoundID.h"
#include "FifamClubLink.h"

// @since FM07
class FifamCompLeague : public FifamCompetition {
public:
    // @since FM07
    // league level
    UChar mLeagueLevel = 0;
    // @since FM07
    // number of relegated teams
    UChar mNumRelegatedTeams = 0;
    // @since FM07
    // indoor relegation flag
    Bool mIndoorRelegation = false;
    // @since FM07
    // reserve relegation flag
    Bool mReserveRelegation = false;
    // @since FM07
    // take points from previous league
    Bool mTakePoints = false;
    // @since FM07
    // equal points sorting rule
    FifamEqualPointsSorting mEqualPointsSorting;
    // @since FM07
    // number of rounds
    UChar mNumRounds = 0;
    // @since FM07
    // round type - usually Group1 for leagues
    FifamRoundID mRoundType = FifamRoundID::Group1;
    // @since FM07
    // attendance multiplier
    UChar mAttendanceMp = 0;
    // @since FM07
    // transfers multiplier
    UChar mTransferMarketMp = 0;
    // @since FM07
    // teams
    Vector<FifamClubLink> mTeams;
    // @since FM07
    // first season calendar
    Vector<UShort> mFirstSeasonMatchdays;
    // @since FM07
    // second season calendar
    Vector<UShort> mSecondSeasonMatchdays;
    // @since FM07
    // fixtures
    Vector<Vector<Pair<UChar, UChar>>> mFixtures;
    // @since FM07
    // bonuses
    Array<UInt, 4> mBonuses = {};
    // @since FM07
    Bool mFreeAgentsCanBeTranferredAtAnyTime = false;
    // @since FM07
    Bool mForeignFreeAgentsCanBeTransferredAtAnyTime = false;
    // @since FM07
    Bool mPlayerLoansCanBeCancelledAtAnyTime = false;
    // @since FM07
    // @range 0-20
    UChar mMaxNumberOfNonEUSigns = 0;
    // @since FM07
    // @range 18-31
    UChar mAgeLimitForNonEUSigns = 0;
    // @since FM07
    // @range 0-15
    UChar mShortTermLoansTotalLimit = 0;
    // @since FM07
    // @range 0-15
    UChar mShortTermLoansSimultaneosLimit = 0;
    // @since FM07
    // @range 0-15
    UChar mLongTermLoansTotalLimit = 0;
    // @since FM07
    // @range 0-15
    UChar mLongTermLoansSimultaneosLimit = 0;
    // @since FM07
    // @range 0-15
    UChar mOverallTotalLoansLimit = 0;
    // @since FM07
    // @range 0-15
    UChar mOverallTotalSimultaneosLoansLimit = 0;
    // @since FM07
    // @range 0-15
    UChar mLoanedPlayersPerSeason = 0;
    // @since FM07
    // @range 0-15
    UChar mLoanPlayerPerSquad = 0;
    // @since FM07
    // @range 0-15
    UChar mSimLoanOtherLeague = 0;
    // @since FM07
    // @range 0-15
    UChar mSimLoanSameLeague = 0;
    // @since FM07
    // @range 0-15
    UChar mLoanOtherLeagueCount = 0;
    // @since FM07
    // @range 0-15
    UChar mLoanSameLeagueCount = 0;
    // @since FM07
    // @range 0-31
    UChar mMinDomesticPlayerCount = 0;
    // @since FM07
    // @range 0-31
    UChar mMinU21PlayerCount = 0;
    // @since FM07
    // @range 0-31
    UChar mMinU24PlayerCount = 0;

    FifamCompDbType GetDbType() const override;
    void Read(FifamReader &reader, FifamDatabase *database, FifamNation nationId) override;
    void Write(FifamWriter &writer, FifamDatabase *database, FifamNation nationId) override;
    void GenerateFixtures();
    void GenerateCalendar(UInt startDay, UInt endDay, UInt winterBreakStartDay, UInt winterBreakEndDay);
    UInt GetNumMatchesInMatchday();
    UInt GetNumMatchdaysInRound();
    UInt GetNumMatchdays();
    UInt GetTotalNumMatches();
    bool ValidateFixtures(String &outErrors);
};
