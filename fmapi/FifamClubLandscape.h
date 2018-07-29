#pragma once
#include "FifamTypes.h"
#include "FifamEnum.h"

ENUM_BEGIN(FifamClubLandscape, UChar)
    ENUM_MEMBER(0, Coast,     L"Coast")
    ENUM_MEMBER(1, River,     L"River")
    ENUM_MEMBER(2, Mountains, L"Mountains")
    ENUM_MEMBER(3, Hills,     L"Hills")
    ENUM_MEMBER(4, Flat,      L"Flat")
    ENUM_DEFAULT_VALUE(Flat)
    ENUM_DEFAULT_READ_WRITE
ENUM_END(FifamClubLandscape)
