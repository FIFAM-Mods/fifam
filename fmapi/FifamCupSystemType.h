#pragma once
#include "FifamTypes.h"
#include "FifamEnum.h"

ENUM_BEGIN(FifamCupSystemType, UInt)
    ENUM_MEMBER( 0, None,       L"None")
    ENUM_MEMBER( 1, German,     L"German")
    ENUM_MEMBER( 2, FA_England, L"FA_England")
    ENUM_MEMBER( 3, French,     L"French")
    ENUM_MEMBER( 4, Spain,      L"Spain")
    ENUM_MEMBER( 5, Italian,    L"Italian")
    ENUM_MEMBER(11, Scotland,   L"Scotland")
    ENUM_MEMBER(22, Unknown22,  L"Unknown22")
    ENUM_DEFAULT_VALUE(None)
    ENUM_DEFAULT_READ_WRITE
ENUM_END(FifamCupSystemType)
