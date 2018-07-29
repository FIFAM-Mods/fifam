#pragma once
#include "FifamTypes.h"
#include "FifamEnum.h"
#include "Color.h"

ENUM_BEGIN(FifamClubMerchandiseColor, UChar)
    ENUM_MEMBER(0, Blue,   L"Blue")
    ENUM_MEMBER(1, Red,    L"Red")
    ENUM_MEMBER(2, Green,  L"Green")
    ENUM_MEMBER(3, Yellow, L"Yellow")
    ENUM_MEMBER(4, White,  L"White")
    ENUM_DEFAULT_VALUE(White)
ENUM_END(FifamClubMerchandiseColor)
