#pragma once
#include "FifamTypes.h"
#include "FifamEnum.h"

ENUM_BEGIN(FifamPoolSorting, UChar)
    ENUM_MEMBER( 0, None,               L"None")
    ENUM_MEMBER( 1, Indoor,             L"POOL_SORT_INDOOR")
    ENUM_MEMBER( 2, NatUefa5,           L"POOL_SORT_NAT_UEFA5")
    ENUM_MEMBER( 3, MapNorthSouth,      L"POOL_SORT_MAP_NORTH_SOUTH")
    ENUM_MEMBER( 4, MapWestEast,        L"POOL_SORT_MAP_WEST_EAST")
    ENUM_MEMBER( 5, MapLeague,          L"POOL_SORT_MAP_LEAGUE")
    ENUM_MEMBER( 6, Nationality,        L"POOL_SORT_NATIONALITY")
    ENUM_MEMBER( 7, BestTabPos,         L"POOL_SORT_BEST_TAB_POS")
    ENUM_MEMBER( 8, CountryStrength,    L"POOL_SORT_COUNTRY_STRENGTH")
    ENUM_MEMBER( 9, Uefa5Shuffle,       L"POOL_SORT_UEFA5_SHUFFLE")
    ENUM_MEMBER(10, Random,             L"POOL_SORT_RANDOM")
    ENUM_MEMBER(11, ChampionscupLast16, L"POOL_SORT_CHAMPIONSCUP_LAST_16") // POOL_SORT_LAST_16 in FM11
    ENUM_MEMBER(12, NorthSouth,         L"POOL_SORT_NORTH_SOUTH")
    ENUM_MEMBER(13, ChampionscupGroup,  L"POOL_SORT_CHAMPIONSCUP_GROUP")
    ENUM_MEMBER(14, PreviousPointsDiff, L"POOL_SORT_PREVIOUS_POINTS_DIFF")
    ENUM_MEMBER(15, Continent,          L"POOL_SORT_CONTINENT")
    ENUM_MEMBER(16, DrawWorldCup,       L"POOL_SORT_DRAW_WORLD_CUP")
    ENUM_MEMBER(17, DrawEuroCup,        L"POOL_SORT_DRAW_EURO_CUP")
    ENUM_MEMBER(18, CopaAmerica,        L"POOL_SORT_COPA_AMERICA")
    ENUM_DEFAULT_VALUE(None)
ENUM_END(FifamPoolSorting)
