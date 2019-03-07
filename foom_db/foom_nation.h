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

struct nation : public team {
    String mThreeLetterName;
    String mNationalityName;
    Int mContinent = -1;
    city *mCapitalCity = 0;
    stadium *mNationalStadium = 0;
    Int mYearsToGainNationality = 0;
    Int mFIFAMember = 0;
    Int mYearAssociationFormed = 0;
    Int mRankingPoints = 0;
    Int mYouthRating = 0;
    Float mEuroCoeff11 = 0;

    struct language {
        language *mLanguage = 0;
        Int mPercentage = 0;
    };

    Vector<language> mVecLanguages;

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

        Vector<city *> mCities;
    } mConverterData;

    nation() {
        mIsNation = true;
    }
};

}
