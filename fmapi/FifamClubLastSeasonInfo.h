#pragma once

#include "FifamTypes.h"
#include "FifamEnum.h"

ENUM_BEGIN(FifamClubLastSeasonLeague, UChar)
    ENUM_MEMBER( 0, None,      L"None")
    ENUM_MEMBER( 1, Promoted,  L"Promoted")
    ENUM_MEMBER( 2, Relegated, L"Relegated")
ENUM_END(FifamClubLastSeasonLeague)

ENUM_BEGIN(FifamClubLastSeasonCup, UChar)
    ENUM_MEMBER( 0, None,     L"None")
    ENUM_MEMBER( 1, RunnerUp, L"RunnerUp")
    ENUM_MEMBER( 2, Winner,   L"Winner")
ENUM_END(FifamClubLastSeasonCup)

class FifamClubLastSeasonInfo {
public:
    FifamClubLastSeasonLeague mLeague;
    FifamClubLastSeasonCup mCup;
};
