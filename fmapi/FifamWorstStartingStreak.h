#pragma once
#include "FifamClubLink.h"
#include "FifamCompID.h"

// @since FM09
class FifamWorstStartingStreak {
public:
    // @since FM09
    FifamCompID mCompetition;
    // @since FM09
    FifamClubLink mClub;
    // @since FM09
    UShort mSeason = 0;
    // @since FM09
    UChar mMatches = 0;
};
