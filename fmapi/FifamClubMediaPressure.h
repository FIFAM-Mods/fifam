#pragma once
#include "FifamTypes.h"
#include "FifamEnum.h"

ENUM_BEGIN(FifamClubMediaPressure, UChar)
    ENUM_MEMBER(0, Low,     L"Low")
    ENUM_MEMBER(1, Normal,  L"Normal")
    ENUM_MEMBER(2, High,    L"High")
    ENUM_MEMBER(3, Extreme, L"Extreme")
    ENUM_DEFAULT_VALUE(Normal)
    ENUM_DEFAULT_READ_WRITE
ENUM_END(FifamClubMediaPressure)
