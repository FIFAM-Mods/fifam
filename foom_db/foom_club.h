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
    Int mBadge = 0;
    Int mSupporterExpectations = 0;

    Bool mIsReserveDummyClub = false;
    Bool mIsReserveToCreateClub = false;

    struct reserve_team {
        club *mReserveClub = 0;
        Int mReserveTeamType = 0;
    };

    Vector<reserve_team> mVecReserveTeams;

    struct reserve_team_to_create {
        Int mReserveTeamType = 0;
        comp *mDivision = 0;
    };

    Vector<reserve_team_to_create> mVecReserveTeamsToCreate;

    struct affiliation {
        club *mAffiliatedClub = 0;
        Date mStartDate;
        Date mEndDate;
        Int mAffiliationType = 0;
        Bool mIsMainClub = false;
        Bool mPlayersMoveFreely = false;
        Bool mSameBoard = false;
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

    Vector<regional_division> mVecRegionalDivisions;

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

    struct nation_transfer_preference {
        nation *mNation = 0;
        Int mLevel = 0;
        Int mMinAge = 0;
        Int mMaxAge = 0;
    };

    Vector<nation_transfer_preference> mVecNationTransferPreferences;

    struct nation_newgen_specification {
        nation *mFirstNation = 0;
        nation *mSecondNation = 0;
        Int mPercentage = 0;
    };

    Vector<nation_newgen_specification> mVecNationNewgenSpecifications;

    Vector<club *> mVecNewClubs;

    struct converter_data {
        UInt mFIFAManagerID = 0;
        UInt mFIFAID = 0;
        void *mFifamClub = nullptr;
        Bool mHasFifamSquad = false;
        Int mLeaguePos = 0;
        Set<UShort> mLeagueWins;
        Set<UShort> mCupWins;
        club *mParentClub = nullptr;
        club *mMainChildClubInDB = nullptr;
        club *mParentClubInDB = nullptr;
        club *mOriginalStoredParentClub = nullptr;
        enum child_type {
            second,
            second_affiliated,
            third,
            u23,
            u21,
            u20,
            u19,
            u18,
            u21_b,
            u19_b,
            u18_b,
            other
        } mChildType = other;
        struct child_club {
            club *mClub = 0;
            Bool mIsAffiliated = false;
            Int mType = other;
        };
        Vector<child_club> mChildClubs;

        Vector<player *> mFutureSigns;
        Vector<player *> mLoanedPlayers;
        Vector<player *> mLoanedOutPlayers;
        Vector<player *> mContractedPlayers;

        bool IsChildTypeYouthTeam() {
            return mChildType == u18 || mChildType == u18_b || mChildType == u19 || mChildType == u19_b;
        }

        Int mAdditionalHistoryReserveTeamType = -1;
        club *mAdditionalHistoryParentTeam = nullptr;
        Bool mTablePromoted = false;
        Bool mTableRelegated = false;
        Bool mTableCupWinner = false;
        Bool mTableCupRunnerUp = false;
    } mConverterData;

    club () {
        mIsNation = false;
    }

    bool is_basque() {
        return mID == 1664 ||
            mID == 1742 ||
            mID == 1688 ||
            mID == 102029 ||
            mID == 1753 ||
            mID == 1667 ||
            mID == 670636 ||
            mID == 1761 ||
            mID == 1746 ||
            mID == 102014 ||
            mID == 811339 ||
            mID == 4200552 ||
            mID == 4200555 ||
            mID == 109009 ||
            mID == 4212148 ||
            mID == 4212658 ||
            mID == 6706354 ||
            mID == 6706369 ||
            mID == 4212213 ||
            mID == 4212262 ||
            mID == 4212637;
    }
};

}
