#pragma once

// @since FM07
enum class FifamAssociation {
    UEFA = 0,
    CONMEBOL = 1,
    CONCACAF = 2,
    CAF = 3,
    AFC = 4,
    OFC = 5,
    None = 6
};

template<>
inline NameIdTable<FifamAssociation> const &GetNameIdTable<FifamAssociation>() {
    static NameIdTable<FifamAssociation> table = {
        { L"UEFA", FifamAssociation::UEFA },
        { L"CONMEBOL", FifamAssociation::CONMEBOL },
        { L"CONCACAF", FifamAssociation::CONCACAF },
        { L"CAF", FifamAssociation::CAF },
        { L"AFC", FifamAssociation::AFC },
        { L"OFC", FifamAssociation::OFC },
        { L"None", FifamAssociation::None }
    };
    return table;
}
