#pragma once
#include "FifamTypes.h"
#include "FifamNation.h"
#include "FifamClubLink.h"

// @since FM13
class FifamFifaWorldPlayer {
public:
    // @since FM13
    UShort mSeason = 0; // year
    // @since FM13
    FifamNation mNationality;
    // @since FM13
    FifamClubLink mClub;
    // @since FM13
    UChar mAge = 0;
    // @since FM13
    String mPlayerPic;
    // @since FM13
    String mLastNameOrPseudonym;
    // @since FM13
    String mFirstName;
};
