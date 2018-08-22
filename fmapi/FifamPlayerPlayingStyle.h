#pragma once
#include "FifamTypes.h"
#include "FifamEnum.h"

ENUM_BEGIN(FifamPlayerPlayingStyle, UChar)
    ENUM_MEMBER( 0, None,               L"None")
    ENUM_MEMBER( 1, AttackingFB,        L"AttackingFB")
    ENUM_MEMBER( 2, DefenceFB,          L"DefenceFB")
    ENUM_MEMBER( 3, Libero,             L"Libero")        // Builds up
    ENUM_MEMBER( 4, SimplePasser,       L"SimplePasser")
    ENUM_MEMBER( 5, BallWinner,         L"BallWinner")
    ENUM_MEMBER( 6, HardMan,            L"HardMan")       // Fighter
    ENUM_MEMBER( 7, Holding,            L"Holding")       // Closes gaps
    ENUM_MEMBER( 8, BallWinnerMidfield, L"BallWinnerMidfield")
    ENUM_MEMBER( 9, BoxToBox,           L"BoxToBox")
    ENUM_MEMBER(10, Busy,               L"Busy")
    ENUM_MEMBER(11, PlayMaker,          L"PlayMaker")
    ENUM_MEMBER(12, Dribbler,           L"Dribbler")
    ENUM_MEMBER(13, Winger,             L"Winger")
    ENUM_MEMBER(14, TargetMan,          L"TargetMan")
    ENUM_MEMBER(15, PenaltyBox,         L"PenaltyBox")
    ENUM_MEMBER(16, RunsChannels,       L"RunsChannels")
    ENUM_MEMBER(17, PullsWideLeft,      L"PullsWideLeft")
    ENUM_MEMBER(18, PullsWideRight,     L"PullsWideRight")
    ENUM_MEMBER(19, DribblerAttack,     L"DribblerAttack")
    ENUM_MEMBER(20, HoldsUp,            L"HoldsUp")
    ENUM_MEMBER(21, BusyAttacker,       L"BusyAttacker")
    ENUM_MEMBER(22, TowerStrength,      L"TowerStrength")
    ENUM_MEMBER(23, DistanceShooter,    L"DistanceShooter")
    ENUM_DEFAULT_READ_WRITE
ENUM_END(FifamPlayerPlayingStyle)
