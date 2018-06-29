#pragma once

#include "FifamPlayer.h"
#include "FifamStaff.h"

// @since FM07
class FifamClub {
public:
    // @since FM08
    Int mID = 0;

    // @since FM07
    // @maxsize 29
    String mName[FifamTranslation::NUM_TRANSLATIONS];

    // @since FM07
    // @maxsize 10
    String mShortName[FifamTranslation::NUM_TRANSLATIONS];

    // @since FM07
    // @maxsize 4
    String mAbbreviation[FifamTranslation::NUM_TRANSLATIONS];

    // @since FM07
    // @maxsize 29
    String mCityName[FifamTranslation::NUM_TRANSLATIONS];

    // @since FM07
    // @maxsize 19
    String mTermForFans;

    // @since FM07
    // @maxsize 19
    String mSecondTeamTerm;

    // @since FM07
    // @maxsize 119
    String mAddress;

    // @since FM07
    // @maxsize 19
    String mTelephone;

    // @since FM07
    // @maxsize 119
    String mWebsiteAndMail;

    // @since FM07
    // @maxsize 255
    String mFansites;

    // @since FM07
    // @maxsize 19
    String mMascotName;

    // @since FM07
    Bool mUsesTheForClubAbbreviation[FifamTranslation::NUM_TRANSLATIONS] = {};

    // @since FM07
    Bool mUsesTheForTermForFans[FifamTranslation::NUM_TRANSLATIONS] = {};

    // @since FM07
    Bool mUsesTheForSecondTeamTerm[FifamTranslation::NUM_TRANSLATIONS] = {};

    // @since FM07
    Bool mUsesTheForClubNameInAPhrase[FifamTranslation::NUM_TRANSLATIONS] = {};

    // @since FM07
    FifamClub *mPartnershipClub = nullptr;
    
    // @since FM07
    FifamClub *mRivalClubs[4] = {};

    // @since FM07
    Short mYearOfFoundation = 1900;

    // @since FM07
    Int mInitialCapital = 0;

    // @since FM07
    Bool mJointStockCompany = false;

    // @since FM07
    Char mStartRule = 0;

    // @since FM07
    Char mPointsOnNewGame = 0;

    // @since FM07
    Int mPotentialFansCount = 0;

    // @since FM07
    Int mAverageAttendanceLastSeason = 0;
    
    // @since FM07
    Int mCountOfSoldSeasonTickets = 0;

    // @since FM07
    Int m1stLeagueTotalPoints = 0;

    // @since FM07
    Int m1stLeagueTotalWins = 0;

    // @since FM07
    Int m1stLeagueTotalDraws = 0;

    // @since FM07
    Int m1stLeagueTotalLoses = 0;

    // @since FM07
    Int m1stLeagueTotalGoals = 0;

    // @since FM07
    Int m1stLeagueTotalGoalsAgainst = 0;

    // @since FM07
    Int m1stLeagueTotalLeadershipsInTable = 0;

    // @since FM07
    struct History {

        // @since FM07
        std::vector<Short> mChampionshipsYears;

        // @since FM07
        std::vector<Short> mCupWinsYears;

        // @since FM07
        std::vector<Short> mSuperCupsWinsYears;

        // @since FM07
        std::vector<Short> mLeagueCupWinsYears;

        // @since FM07
        std::vector<Short> mEuropeanCupWinsYears;

        // @since FM07
        std::vector<Short> mChampionsLeagueYears;

        // @since FM07
        std::vector<Short> mWorldChampionshipYears;

        // @since FM07
        std::vector<Short> mWorldClubChampionshipYears;

        // @since FM07
        struct Record {
            // @since FM07
            Char mScore1 = 0;

            // @since FM07
            Char mScore2 = 0;

            // @since FM07
            Int mAttendance = 0;

            // @since FM07
            String mClubName;

            // @since FM07
            Short mYear = 0;
        };

        // @since FM07
        std::vector<Record> mRecords;

    } mHistory;

    // @since FM07
    struct Kit {

    } mKit;

    struct {
        // @since FM07
        // @maxsize 29
        String _1; // empty

        // @since FM07
        Char _2; // 0

        // @since FM07
        Char _3; // 3
    } Unknown;
};
