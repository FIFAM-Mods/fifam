#pragma once
#include "FifamCompID.h"
#include "FifamClubLink.h"

// @since FM09
class FifamWorstStartingStreak {
public:
    // @since FM09
    FifamCompID mCompID;
    // @since FM09
    FifamClubLink mClub;
    // @since FM09
    UShort mSeason = 0;
    // @since FM09
    UChar mMatches = 0;
};
