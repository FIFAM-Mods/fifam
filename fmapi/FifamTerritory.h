#pragma once

// @since FM07
enum class FifamTerritory {
    None = 0,
    Africa = 1,
    Asia = 2,
    Caribean = 3,
    CentralAmerica = 4,
    CentralWesternEurope = 5,
    EasternEurope = 6,
    MiddleEast = 7,
    NorthAmerica = 8,
    Oceania = 9,
    Scandinavia = 10,
    SouthAmerica = 11,
    SouthernEurope = 12,
};

template<>
inline NameIdTable<FifamTerritory> const &GetNameIdTable<FifamTerritory>() {
    static NameIdTable<FifamTerritory> table = {
        { L"None", FifamTerritory::None },
        { L"Africa", FifamTerritory::Africa },
        { L"Asia", FifamTerritory::Asia },
        { L"Caribean", FifamTerritory::Caribean },
        { L"Central America", FifamTerritory::CentralAmerica },
        { L"Central Western Europe", FifamTerritory::CentralWesternEurope },
        { L"Eastern Europe", FifamTerritory::EasternEurope },
        { L"Middle East", FifamTerritory::MiddleEast },
        { L"North America", FifamTerritory::NorthAmerica },
        { L"Oceania", FifamTerritory::Oceania },
        { L"Scandinavia", FifamTerritory::Scandinavia },
        { L"South America", FifamTerritory::SouthAmerica },
        { L"Southern Europe", FifamTerritory::SouthernEurope }
    };
    return table;
}
