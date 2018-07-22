#pragma once
#include "FifamTypes.h"
#include "FifamClubRecord.h"

// @since FM07
class FifamClubHistory {
public:
    // @since FM07
    Vector<UShort> mLeagueWinYears;
    // @since FM07
    Vector<UShort> mCupWinYears;
    // @since FM07
    Vector<UShort> mSuperCupsWinYears;
    // @since FM07
    Vector<UShort> mLeagueCupWinYears;
    // @since FM07
    Vector<UShort> mEuroTrophyWinYears;
    // @since FM07
    Vector<UShort> mChampionsCupWinYears;
    // @since FM07
    Vector<UShort> mWorldChampionshipWinYears;
    // @since FM07
    Vector<UShort> mWorldClubChampionshipWinYears;
    // @since FM07
    FifamClubRecord mRecordHomeWin;
    // @since FM07
    FifamClubRecord mRecordHomeDefeat;
    // @since FM07
    FifamClubRecord mRecordAwayWin;
    // @since FM07
    FifamClubRecord mRecordAwayDefeat;
    // @since FM07
    FifamClubRecord mRecordAttendance;

    void Read(FifamReader &reader);
    void Write(FifamWriter &writer);
};
