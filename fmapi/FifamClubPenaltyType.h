#pragma once
#include "FifamTypes.h"
#include "FifamEnum.h"

ENUM_BEGIN(FifamClubPenaltyType, UChar)
    ENUM_MEMBER(0, None,   L"None")
    ENUM_MEMBER(3, Points, L"Points")
    ENUM_DEFAULT_VALUE(None)
    ENUM_DEFAULT_READ_WRITE
ENUM_END(FifamClubPenaltyType)
