#pragma once
#include "FifamTypes.h"
#include "FifamEnum.h"

ENUM_BEGIN(FifamLeagueSystemType, UChar)
    ENUM_MEMBER( 0, None,     L"None")
    ENUM_MEMBER(14, English,  L"English")
    ENUM_MEMBER(18, French,   L"French")
    ENUM_MEMBER(21, German,   L"German")
    ENUM_MEMBER(42, Scottish, L"Scottish")
    ENUM_MEMBER(45, Spanish,  L"Spanish")
    ENUM_DEFAULT_VALUE(None)
    ENUM_DEFAULT_READ_WRITE
ENUM_END(FifamLeagueSystemType)
