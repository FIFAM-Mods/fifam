#pragma once
#include "FifamTypes.h"
#include "FifamReadWrite.h"
#include "FifamEqualPointsSorting.h"

// @since FM07
class FifamCountryLeagueLevelInfo {
public:
    // @since FM07
    FifamEqualPointsSorting mEqualPointsSorting;
    // @since FM07
    UChar mNumNonEUPlayersAllowed = 0;
    // @since FM07
    UChar mNumRelegatedTeams = 0;
    // @since FM07
    UChar mRating = 0;

    struct {
        UChar _1 = 0; // 0
        UChar _2 = 0; // 0
    } Unknown;

    void Read(FifamReader &reader);
    void Write(FifamWriter &writer, UInt minLeagueIndex, UInt maxLeagueIndex);
};
