#pragma once
#include "FifamTypes.h"
#include "FifamEnum.h"

ENUM_BEGIN(FifamClubTeamType, UChar)
    ENUM_MEMBER(0,  First,   L"First")
    ENUM_MEMBER(1,  Reserve, L"Reserve")
    ENUM_MEMBER(2,  YouthA,  L"YouthA")
    ENUM_MEMBER(4,  YouthB,  L"YouthB")
    ENUM_DEFAULT_VALUE(First)
    ENUM_DEFAULT_READ_WRITE
ENUM_END(FifamClubTeamType)
