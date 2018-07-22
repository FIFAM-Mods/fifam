#pragma once

#include "FifamPlayer.h"
#include "FifamStaff.h"
#include "FifamTranslation.h"

// @since FM07
class FifamClub {
public:
    // @since FM08
    UInt mUID = 0;

    // @since FM07
    // @maxsize 29
    TrArray<String> mName;

    // @since FM07
    // @maxsize 10
    TrArray<String> mShortName;

    // @since FM07
    // @maxsize 4
    TrArray<String> mAbbreviation;

    // @since FM07
    // @maxsize 29
    TrArray<String> mCityName;

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
    TrArray<Bool> mUsesTheForClubAbbreviation = {};

    // @since FM07
    TrArray<Bool> mUsesTheForTermForFans = {};

    // @since FM07
    TrArray<Bool> mUsesTheForSecondTeamTerm = {};

    // @since FM07
    TrArray<Bool> mUsesTheForClubNameInAPhrase = {};

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
        Vector<Short> mChampionshipsYears;

        // @since FM07
        Vector<Short> mCupWinsYears;

        // @since FM07
        Vector<Short> mSuperCupsWinsYears;

        // @since FM07
        Vector<Short> mLeagueCupWinsYears;

        // @since FM07
        Vector<Short> mEuropeanCupWinsYears;

        // @since FM07
        Vector<Short> mChampionsLeagueYears;

        // @since FM07
        Vector<Short> mWorldChampionshipYears;

        // @since FM07
        Vector<Short> mWorldClubChampionshipYears;

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
        Vector<Record> mRecords;

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
