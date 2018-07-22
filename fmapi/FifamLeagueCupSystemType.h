#pragma once
#include "FifamTypes.h"
#include "FifamEnum.h"

ENUM_BEGIN(FifamLeagueCupSystemType, UChar)
    ENUM_MEMBER( 0, None,       L"None")
    ENUM_MEMBER( 6, German,     L"German")
    ENUM_MEMBER( 7, English,    L"English")
    ENUM_MEMBER( 8, French,     L"French")
    ENUM_MEMBER(12, Scotland,   L"Scotland")
    ENUM_MEMBER(13, Div2_3,     L"Div2/3")
    ENUM_MEMBER(14, Conference, L"Conference")
    ENUM_DEFAULT_VALUE(None)
    ENUM_DEFAULT_READ_WRITE
ENUM_END(FifamLeagueCupSystemType)
