#pragma once
#include "FifamTypes.h"
#include "FifamEnum.h"

ENUM_BEGIN(FifamClimate, UChar)
    ENUM_MEMBER(0, Cold,     L"Cold")
    ENUM_MEMBER(1, Cool,     L"Cool")
    ENUM_MEMBER(2, Moderate, L"Moderate")
    ENUM_MEMBER(3, Warm,     L"Warm")
    ENUM_MEMBER(4, Hot,      L"Hot")
    ENUM_DEFAULT_VALUE(Cold)
    ENUM_DEFAULT_READ_WRITE
ENUM_END(FifamClimate)
