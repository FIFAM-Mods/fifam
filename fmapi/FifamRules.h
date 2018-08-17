#pragma once
#include "FifamTypes.h"
#include "FifamContinent.h"
#include "FifamClubLink.h"
#include "FifamReadWrite.h"

class FifamRules {
public:
    struct ContinentalCupNames {
        String mFirstCup;
        String mSecondCup;
        String mSuperCup;
    };

    struct ContinentalCupClubs {
        FifamClubLink mFirstCup;
        FifamClubLink mSecondCup;
        FifamClubLink mSuperCup;
    };
    
    // @since FM07
    ContinentalCupNames mContinentalCupNames[FifamContinent::NUM_CONTINENTS];
    // @since FM07
    ContinentalCupClubs mContinentalCupChampions[FifamContinent::NUM_CONTINENTS];
    // @since FM07
    UInt mPointsForHomeWin = 3;
    // @since FM07
    UInt mPointsForAwayWin = 3;
    // @since FM07
    Bool mBonusPointForWinAgainsChampion = false;
    // @since FM07
    Bool mBonusPointForMoreThan3Goals = false;
    // @since FM07
    LimitedUnorderedSet<UShort, 16, 0> mInternationalFriendliesFirstSeason;
    // @since FM07
    LimitedUnorderedSet<UShort, 16, 0> mInternationalFriendliesSecondSeason;

    struct {
        // @since FM07
        FifamClubLink _1 = nullptr;
        // @since FM07
        FifamClubLink _2 = nullptr;
        // @since FM07
        Bool _3 = false;
    } Unknown;

    void Read(FifamReader &reader) {
        if (reader.ReadStartIndex(L"RULES_AND_MISC")) {
            if (reader.ReadVersion()) {

            }
            reader.ReadEndIndex(L"RULES_AND_MISC");
        }
    }
};
