#pragma once
#include "FifamEnum.h"
#include "FifamTypes.h"

// @since FM07
ENUM_BEGIN(FifamContinent, UChar)
    ENUM_MEMBER(0, Europe,       L"Europe")
    ENUM_MEMBER(1, SouthAmerica, L"South America")
    ENUM_MEMBER(2, NorthAmerica, L"North America")
    ENUM_MEMBER(3, Africa,       L"Africa")
    ENUM_MEMBER(4, Asia,         L"Asia")
    ENUM_MEMBER(5, Oceania,      L"Oceania")
    ENUM_MEMBER(6, None,         L"None")
    ENUM_DEFAULT_VALUE(None)
    ENUM_DEFAULT_READ_WRITE

    static const UInt NUM_CONTINENTS = 6;
ENUM_END(FifamContinent)
