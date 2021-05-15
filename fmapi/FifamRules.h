#pragma once
#include "FifamTypes.h"
#include "FifamContinent.h"
#include "FifamClubLink.h"
#include "FifamReadWrite.h"
#include "FifamStadium.h"

class FifamRules {
public:
    struct ContinentalCupNames {
        FifamTrArray<String> mFirstCup;
        FifamTrArray<String> mSecondCup;
        // @until FM08
        FifamTrArray<String> mFirstCupDup;
        FifamTrArray<String> mSecondCupDup;
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
    // @until FM08
    FifamTrArray<String> mEuropeanSuperCupNameDup;
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
    Vector<FifamClubLink> mFairnessAwardWinners;
    // @since FM09
    Array<UShort, 16> mInternationalFriendliesFirstSeason;
    // @since FM09
    Array<UShort, 16> mInternationalFriendliesSecondSeason;
    // @since FM03
    // @until FM05
    FifamStadium *mEuropeCup1Stadium = nullptr;
    FifamStadium *mEuropeCup2Stadium = nullptr;
    FifamStadium *mSouthAmericaCup1Stadium = nullptr;

    struct {
        // @since FM07
        FifamClubLink _1;
        // @since FM07
        FifamClubLink _2;
        // @since FM07
        Bool _3 = false;
    } Unknown;

    void Read(FifamReader &reader);
    void Write(FifamWriter &writer);
};
