#pragma once

enum class FifamRoundID : UChar {
    Quali = 1,
    Quali2 = 2,
    Quali3 = 3,
    PreRound1 = 4,
    _1 = 5,
    _2 = 6,
    _3 = 7,
    _4 = 8,
    _5 = 9,
    Group1 = 10,
    Group2 = 11,
    Last16 = 12,
    Quarterfinal = 13,
    Semifinal = 14,
    Final = 15
};

template<>
inline NameIdTable<FifamRoundID> const &GetNameIdTable<FifamRoundID>() {
    static NameIdTable<FifamRoundID> table = {
        { L"ROUND_QUALI", FifamRoundID::Quali },
        { L"ROUND_QUALI2", FifamRoundID::Quali2 },
        { L"ROUND_QUALI3", FifamRoundID::Quali3 },
        { L"ROUND_PREROUND1", FifamRoundID::PreRound1 },
        { L"ROUND_1", FifamRoundID::_1 },
        { L"ROUND_2", FifamRoundID::_2 },
        { L"ROUND_3", FifamRoundID::_3 },
        { L"ROUND_4", FifamRoundID::_4 },
        { L"ROUND_5", FifamRoundID::_5 },
        { L"ROUND_GROUP1", FifamRoundID::Group1 },
        { L"ROUND_GROUP2", FifamRoundID::Group2 },
        { L"ROUND_LAST_16", FifamRoundID::Last16 },
        { L"ROUND_QUARTERFINAL", FifamRoundID::Quarterfinal },
        { L"ROUND_SEMIFINAL", FifamRoundID::Semifinal },
        { L"ROUND_FINAL", FifamRoundID::Final }
    };
    return table;
}
