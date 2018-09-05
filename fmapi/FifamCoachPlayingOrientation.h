#pragma once
#include "FifamTypes.h"
#include "FifamEnum.h"

ENUM_BEGIN(FifamCoachPlayingOrientation, UChar)
    ENUM_MEMBER(0, VeryDefensive, L"Very Defensive")
    ENUM_MEMBER(1, Defensive,     L"Defensive")
    ENUM_MEMBER(2, Normal,        L"Normal")
    ENUM_MEMBER(3, Offensive,     L"Offensive")
    ENUM_MEMBER(4, VeryOffensive, L"Very Offensive")
    ENUM_DEFAULT_VALUE(Normal)
    ENUM_DEFAULT_READ_WRITE
ENUM_END(FifamCoachPlayingOrientation)
