#pragma once
#include "FifamTypes.h"
#include "Date.h"
#include "foom_team.h"
#include "foom_kit.h"

namespace foom {

struct nation;
struct city;
struct stadium;
struct comp;
struct player;

struct club : public team {
    String mSixLetterName;
    Bool mExtinct = 0;
    nation *mNation = 0;
    nation *mBasedNation = 0;
    nation *mContinentalCupNation = 0;
    city *mCity = 0;
    Int mYearFounded = 0;
    Int mStatus = 0;
    Float mLatitude = 0;
    Float mLongitude = 0;
    Int mChairmanTitle = 0;
    Int mOwnershipType = 0;
    stadium *mStadium = 0;
    Int mTraining = 0;
    Int mYouthCoaching = 0;
    Int mYouthFacilities = 0;
    Int mYouthRecruitment = 0;
    Int mYouthImportance = 0;
    Int mCorporateFacilities = 0;
    Int mBalance = 0;
    Int mTransferBudget = 0;
    Int mSugarDaddy = 0;
    Int mNumberOfSeasonTicketHolders = 0;
    comp *mDivision = 0;
    comp *mLastDivision = 0;
    Int mLastPosition = 0;
    comp *mNextDivision = 0;
    Int mLastPositionForNextDivision = 0;
    comp *mSecondaryDivision = 0;
    club *mRecordWinOpposition = 0;
    Int mRecordWinTG = 0;
    Int mRecordWinOG = 0;
    Int mRecordWinYear = 0;
    club *mRecordDefeatOpposition = 0;
    Int mRecordDefeatTG = 0;
    Int mRecordDefeatOG = 0;
    Int mRecordDefeatYear = 0;
    club *mRecordAttendanceOpposition = 0;
    Int mRecordAttendance = 0;
    Int mRecordAttendanceYear = 0;

    struct affiliation {
        club *mAffiliatedClub = 0;
        Bool mIsMainClub = 0;
        Date mStartDate;
        Date mEndDate;
        Int mAffiliationType = 0;
    };

    Vector<affiliation> mVecAffiliations;

    struct buy_back_clause {
        player *mPlayer = 0;
        club *mFromClub = 0;
        Int mBuyBackFee = 0;
    };

    Vector<buy_back_clause> mVecBuyBackClauses;

    struct income {
        Int mAmount = 0;
        Int mIncomeType = 0;
        Date mStartDate;
        Date mEndDate;
        Bool mRenewIncome = 0;
        Bool mFixedValue = 0;
    };

    Vector<income> mVecIncomes;

    struct regional_division {
        Int mLevel = 0;
        comp *mDivision = 0;
        Int mYear = 0;
    };

    Vector<kit> mVecRegionalDivisions;

    struct rival_club {
        club *mRivalClub = 0;
        Int mLevel = 0;
        Int mReason = 0;
        Bool mClubDontTransferPlayers = 0;
    };

    Vector<rival_club> mVecRivalClubs;

    struct team_league_history {
        Int mYear = 0;
        Int mOrder = 0;
        comp *mDivision = 0;
        Int mPosition = 0;
        Int mMaxTeams = 0;
        Int mPoints = 0;
        Int mGamesPlayed = 0;
        Int mGamesWon = 0;
        Int mGamesDrawn = 0;
        Int mGamesLost = 0;
        Int mGoalsFor = 0;
        Int mGoalsAgainst = 0;
    };

    Vector<team_league_history> mVecTeamLeagueHistory;

    struct converter_data {
        Int mFIFAManagerID = -1;
        Int mFIFAID = -1;
    } mConverterData;

    club () {
        mIsNation = false;
    }
};

}
