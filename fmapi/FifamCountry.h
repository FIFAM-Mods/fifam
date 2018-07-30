#pragma once

#include "FifamClub.h"
#include "FifamStadium.h"
#include "FifamSponsor.h"
#include "FifamReferee.h"
#include "FifamContinent.h"
#include "FifamTerritory.h"
#include "FifamSuspensionRule.h"
#include "FifamRedCardRule.h"
#include "FifamCountryLeagueLevelInfo.h"
#include "FifamLeagueSystemType.h"
#include "FifamCupSystemType.h"
#include "FifamLeagueCupSystemType.h"
#include "FifamSuperCupType.h"
#include "FifamClimate.h"
#include "FifamAmateurRule.h"
#include "FifamManager.h"
#include "FifamFlags.h"
#include "FifamCountryFlags.h"
#include "FifamCountryAppearanceData.h"

/*
TODO: resolve club UID (>206)
*/

class FifamDatabase;

class FifamCountry {
public:
    // @since FM07
    // @maxsize 30
    TrArray<String> mName;
    // @since FM07
    // @maxsize 4
    TrArray<String> mAbbr;
    // @since FM07
    TrArray<Bool> mNameGender = {}; // 0: in % 1: in the % 2: on % 3: on the % 4: - 5: - 6: -
    // @since FM07
    FifamContinent mContinent;
    // @since FM07
    FifamLanguage mFirstLanguageForNames;
    // @since FM07
    FifamLanguage mSecondLanguageForNames;
    // @since FM07
    FifamTerritory mTerritory;
    // @since FM07
    // @until FM11
    // ID is same as country ID
    FifamLeagueSystemType mOriginalLeagueSystem;
    // @since FM07
    // @maxsize 9
    String mFA_Name;
    // @since FM07
    Array<FifamLanguage, 4> mLanguages;
    // @since FM07
    Array<UChar, 4> mLanguageFrequency = {};
    // @since FM07
    UChar mLeagueLevelWithReserveTeams = 0;
    // @since FM07
    // @until FM11
    // @maxsize 16
    String mFA_PresidentFirstName;
    // @since FM07
    // @until FM11
    // @maxsize 20
    String mFA_PresidentLastName;
    // @since FM07
    // @until FM11
    FifamDate mFA_PresidentBirthDate;
    // @since FM07
    FifamSuspensionRule mYellowCardsLeagueRule;
    // @since FM07
    Bool mYellowCardsLeagueAfterYelRed = false;
    // @since FM07
    FifamSuspensionRule mYellowCardsCupRule;
    // @since FM07
    Bool mYellowCardsCupAfterYelRed = false;
    // @since FM07
    FifamRedCardRule mRedCardRule;
    // @since FM07
    Bool mYellowCardsBanForAllComp = false;
    // @since FM07
    Bool mSecondYellowCardBanForAllComp = false;
    // @since FM07
    Bool mRedCardsBanForAllComp = false;
    // @since FM07
    Bool mRedCardBanFixedDuration = false;
    // @since FM07
    Array<Bool, 5> mSecondYellowCardLeagueState = {};
    // @since FM07
    Array<FifamCountryLeagueLevelInfo, 16> mLeagueLevels;
    // @since FM07
    FifamCupSystemType mCupSystemType;
    // @since FM07
    Bool mCupHomeAdvantageForTeamsInLowerLeagues = false;
    // @since FM07
    Bool mCupCanPlayersBeCupTied = false;
    // @since FM07
    UInt mCupFinalStadium;
    // @since FM07
    FifamSuperCupType mSuperCupType;
    // @since FM07
    Array<FifamLeagueCupSystemType, 4> mLeagueCupSystemType;
    // @since FM07
    UChar mYearsForNaturalization = 0;
    // @since FM07
    Flags<FifamCountryFlags> mFlags;
    // @since FM07
    // move between teams rule
    FifamAmateurRule mAmateurRule;
    // @since FM07
    FifamClimate mClimate;
    // @since FM07
    Array<UShort, 6> mAssessmentInfo = {};
    // @since FM07
    UChar mProbabilityForOverseasTransfers = 0;
    // @since FM07
    // @until FM11
    FifamManager mManager;
    // @since FM07
    UChar mAveragePlayerRating = 0;
    // @since FM07
    // 2 - black, 4 - brown, 8 - medium blonde, 16 - blonde, 32 - red
    UInt mAppearanceHairColor = 0;
    // @since FM07
    // 2 - Hairstyle1, 4 - Hairstyle2, ...
    Array<UInt, 2> mAppearanceHairStyle = {};
    // @since FM07
    // 2 - none, 4 - goatee, 8 - tash, 16 - shadow, 32 - full goatee, 64 - full beard
    UInt mAppearanceBeard = 0;
    // @since FM07
    // 2 - Caucasian1, 4 - Caucasian2, ...
    Array<UInt, 4> mAppearanceFaceType = {};
    // @since FM07
    UChar mAverageHeight = 0;
    // @since FM07
    UChar mAverageWeight = 0;
    // @since FM11
    Vector<FifamCountryAppearanceData> mAppearanceData;
    // @since FM08
    FifamTerritory mPreferredTransfersTerritory;
    // @since FM08
    UShort mFifaRanking = 0;
    // @since FM07
    UShort mNumContinentalChampions = 0;
    // @since FM07
    UShort mNumWorldCups = 0;
    // @since FM07
    UShort mNumContinentalRunnersUp = 0;
    // @since FM07
    UShort mNumWorldCupRunnersUp = 0;
    // @since FM07
    // @maxsize infinite in FM07
    // @maxsize 128 in FM11
    // @maxsize 196 in FM13
    String mNotes;

    struct {
        // @since FM07
        UChar _1 = 0;
        // @since FM07
        // @until FM08
        // @maxsize 16
        String _2;
        // @since FM07
        // @until FM08
        // @maxsize 20
        String _3;
        // @since FM07
        // @until FM08
        // @maxsize 16
        String _4;
        // @since FM07
        // @until FM08
        // @maxsize 20
        String _5;
        // @since FM07
        // @until FM08
        // @maxsize 16
        String _6;
        // @since FM07
        // @until FM08
        // @maxsize 20
        String _7;
        // @since FM07
        // @until FM08
        Bool _8 = 0;
        // @since FM07
        // @until FM08
        Bool _9 = 0;
        // @since FM07
        Array<UChar, 2> _10 = {};
        // @since FM07
        UChar _11 = 0;
        // @since FM07
        UChar _12 = 0;
        // @since FM07
        UChar _13 = 0;
        // @since FM07
        UChar _14 = 0;
        // @since FM07
        // compared with 205, maybe some country id? ; ignored in 13
        UChar _15 = 0;
        // @since FM07
        UInt _16 = 0;
        // @since FM07
        UInt _17 = 0;
        // @since FM07
        // 1 or 3
        UInt _18 = 0;
        // @since FM07
        // @maxsize 28
        String _19;
    } Unknown;

    // Country .sav files were read
    Bool mHasCountryData = false;
    // Country id (first country = 1)
    UInt mId = 0;
    // our database
    FifamDatabase *mDatabase = nullptr;
    Vector<FifamReferee *> mReferees;
    Vector<FifamStadium *> mStadiums;
    Vector<FifamSponsor *> mSponsors;
    Vector<FifamClub *> mClubs;
    FifamClub mNationalTeam;

    FifamReferee *AddReferee();
    FifamStadium *AddStadium();
    FifamSponsor *AddSponsor();
    FifamCountry(UInt id, FifamDatabase *db);
    void Read(FifamReader &reader);
    void ReadFixtures(FifamReader &reader);
    void ReadScript(FifamReader &reader);
    void Write(FifamWriter &writer);
};
