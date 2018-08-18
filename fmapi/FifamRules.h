#pragma once
#include "FifamTypes.h"
#include "FifamContinent.h"
#include "FifamClubLink.h"
#include "FifamReadWrite.h"

class FifamDatabase;

class FifamRules {
public:
    struct ContinentalCupNames {
        FifamTrArray<String> mFirstCup;
        FifamTrArray<String> mSecondCup;
    };

    struct ContinentalCupClubs {
        FifamClubLink mFirstCup;
        FifamClubLink mSecondCup;
        FifamClubLink mSuperCup;
    };
    
    // @since FM07
    Array<ContinentalCupNames, FifamContinent::NUM_CONTINENTS> mContinentalCupNames;
    // @since FM07
    FifamTrArray<String> mEuropeanSuperCupName;
    // @since FM07
    Array<ContinentalCupClubs, FifamContinent::NUM_CONTINENTS> mContinentalCupChampions;
    Array<ContinentalCupClubs, FifamContinent::NUM_CONTINENTS> mContinentalCupStadiums;
    // @since FM07
    UInt mPointsForHomeWin = 3;
    // @since FM07
    UInt mPointsForAwayWin = 3;
    // @since FM07
    Bool mBonusPointForWinAgainstChampion = false;
    // @since FM07
    Bool mBonusPointForMoreThan3Goals = false;
    // @since FM07
    UInt mNumSubsInFriendlyMatches = 7;
    // @since FM08
    Array<FifamClubLink, 3> mFairnessAwardWinners;
    // @since FM09
    Array<UShort, 16> mInternationalFriendliesFirstSeason;
    // @since FM09
    Array<UShort, 16> mInternationalFriendliesSecondSeason;

    struct {
        // @since FM07
        FifamClubLink _1;
        // @since FM07
        FifamClubLink _2;
        // @since FM07
        Bool _3 = false;
    } Unknown;

    void Read(FifamReader &reader, FifamDatabase *database);
    void Write(FifamWriter &writer, FifamDatabase *database);
};
