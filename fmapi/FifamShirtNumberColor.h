#pragma once
#include "FifamTypes.h"
#include "FifamEnum.h"

ENUM_BEGIN(FifamShirtNumberColor, UChar)
    ENUM_MEMBER(0, Default, L"Default")
    ENUM_MEMBER(1, White,   L"White")
    ENUM_MEMBER(2, Black,   L"Black")
    ENUM_MEMBER(3, Yellow,  L"Yellow")
    ENUM_MEMBER(4, Red,     L"Red")
    ENUM_MEMBER(5, Green,   L"Green")
    ENUM_MEMBER(6, Blue,    L"Blue")
    ENUM_DEFAULT_VALUE(Default)
    ENUM_DEFAULT_READ_WRITE
ENUM_END(FifamShirtNumberColor)
