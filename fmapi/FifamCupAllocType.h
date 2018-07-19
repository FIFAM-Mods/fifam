#pragma once
#include "FifamTypes.h"
#include "FifamEnum.h"

ENUM_BEGIN(FifamCupAllocType, UChar)
    ENUM_MEMBER(0, FA,         L"FA")
    ENUM_MEMBER(1, League,     L"LEAGUE")
    ENUM_MEMBER(2, Supercup,   L"SUPERCUP")
    ENUM_MEMBER(3, Relegation, L"RELEGATION")
    ENUM_MEMBER(4, Misc,       L"MISC")
    ENUM_MEMBER(5, CC_FA,      L"CC_FA")
    ENUM_DEFAULT_VALUE(Misc)
    ENUM_DEFAULT_READ_WRITE_STRING
ENUM_END(FifamCupAllocType)
