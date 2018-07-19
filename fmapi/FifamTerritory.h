#pragma once
#include "FifamTypes.h"
#include "FifamEnum.h"

// @since FM07
ENUM_BEGIN(FifamTerritory, UChar)
    ENUM_MEMBER(0,  None,                 L"None")
    ENUM_MEMBER(1,  Africa,               L"Africa")
    ENUM_MEMBER(2,  Asia,                 L"Asia")
    ENUM_MEMBER(3,  Caribean,             L"Caribean")
    ENUM_MEMBER(4,  CentralAmerica,       L"Central America")
    ENUM_MEMBER(5,  CentralWesternEurope, L"Central Western Europe")
    ENUM_MEMBER(6,  EasternEurope,        L"Eastern Europe")
    ENUM_MEMBER(7,  MiddleEast,           L"Middle East")
    ENUM_MEMBER(8,  NorthAmerica,         L"North America")
    ENUM_MEMBER(9,  Oceania,              L"Oceania")
    ENUM_MEMBER(10, Scandinavia,          L"Scandinavia")
    ENUM_MEMBER(11, SouthAmerica,         L"South America")
    ENUM_MEMBER(12, SouthernEurope,       L"Southern Europe")
    ENUM_DEFAULT_VALUE(None);
    ENUM_DEFAULT_READ_WRITE
ENUM_END(FifamTerritory)
