#pragma once
#include "FifamTypes.h"
#include "foom_team.h"
#include "foom_kit.h"

namespace foom {

struct city;
struct stadium;
struct language;
struct region;
struct comp;
struct club;

struct nation : public team {
    String mNationalityName;
    Array<String, 6> mTranslatedNationalityName;
    Int mContinent = -1;
    city *mCapitalCity = 0;
    stadium *mNationalStadium = 0;
    Int mYearsToGainNationality = 0;
    Int mFIFAMember = 0;
    Int mYearAssociationFormed = 0;
    Int mRankingPoints = 0;
    Int mYouthRating = 0;
    Float mEuroCoeff11 = 0;
    nation *mNationalTeamMen = 0;
    nation *mNationalTeamWomen = 0;

    struct nation_language {
        language *mLanguage = 0;
        Int mPercentage = 0;
    };

    Vector<nation_language> mVecLanguages;

    struct region_transfer_preference {
        region *mRegion = 0;
        Int mRating = 0;
        Int mMinimumAge = 0;
        Int mMaximumAge = 0;
    };

    Vector<region_transfer_preference> mVecRegionTransferPreferences;

    struct converter_data {
        void *mFifamCountry = nullptr;
        UInt mFIFAManagerID = 0;
        UInt mFIFAManagerReplacementID = 0;
        UInt mFIFAID = 0;
        UInt mFIFATeamID = 0;
        Set<UShort> mWorldCupWins;
        Set<UShort> mContinentalCupWins;
        Set<UShort> mWorldCupFinals;
        Set<UShort> mContinentalCupFinals;
        Vector<comp *> mMainLeagues;
        Vector<comp *> mMainCups;
        Vector<comp *> mLeagueCups;
        Vector<comp *> mSuperCups;
        struct {
            void *league = nullptr; // div ptr
            void *cup = nullptr; // cup ptr
            void *leagueCup = nullptr; // cup ptr
            void *supercup = nullptr; // cup ptr
            foom::team *cupWinner = nullptr;
            foom::team *cupRunnerUp = nullptr;
            foom::team *leagueWinner = nullptr;
        } mDomesticComps;
        Vector<city *> mCities;
        Vector<club *> mSpareClubs;
        nation *mHistoryNation = nullptr;
    } mConverterData;

    nation() {
        mIsNation = true;
    }
};

}
