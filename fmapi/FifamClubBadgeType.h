#pragma once
#include "FifamTypes.h"
#include "FifamEnum.h"

ENUM_BEGIN(FifamClubBadgeType, UChar)
    ENUM_MEMBER(0, NoBadge, L"NoBadge")
    ENUM_MEMBER(1, Color,   L"Color")
    ENUM_MEMBER(2, Club,    L"Club")
    ENUM_MEMBER(3, EALogo,  L"EALogo")
    ENUM_DEFAULT_VALUE(Color)
    ENUM_DEFAULT_READ_WRITE
ENUM_END(FifamClubBadgeType)
