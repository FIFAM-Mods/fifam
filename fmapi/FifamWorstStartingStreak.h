#pragma once
#include "FifamClubLink.h"

class FifamCompetition;

// @since FM09
class FifamWorstStartingStreak {
public:
    // @since FM09
    FifamCompetition *mCompetition;
    // @since FM09
    FifamClubLink mClub;
    // @since FM09
    UShort mSeason = 0;
    // @since FM09
    UChar mMatches = 0;
};
