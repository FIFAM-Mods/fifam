#pragma once

enum class FifamCupAllocType : UChar {
    FA,
    League,
    Supercup,
    Relegation,
    Misc,
    CC_FA,
};

template<>
inline NameIdTable<FifamCupAllocType> const &GetNameIdTable<FifamCupAllocType>() {
    static NameIdTable<FifamCupAllocType> table = {
        { L"FA", FifamCupAllocType::FA },
        { L"LEAGUE", FifamCupAllocType::League },
        { L"SUPERCUP", FifamCupAllocType::Supercup },
        { L"RELEGATION", FifamCupAllocType::Relegation },
        { L"MISC", FifamCupAllocType::Misc },
        { L"CC_FA", FifamCupAllocType::CC_FA }
    };
    return table;
}
