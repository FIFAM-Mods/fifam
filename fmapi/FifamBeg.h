#pragma once

enum class FifamBeg : UInt {
    _1stLeg = 1,
    With1stLeg = 1,
    _2ndLeg = 2,
    With2ndLeg = 2,
    WithReplay = 4,
    WithExtraTime = 8,
    WithGoldenGoal = 16,
    WithPenalty = 32,
    End = 64,
    _1stPlayed = 256,
    _2ndPlayed = 512,
    ExtraTime = 4096,
    GoldenGoal = 8192,
    Penalty = 16384,
    Finished = 32768,
    WithSilverGoal = 67108864,
    WithoutAwayGoal = 134217728,
    NoShuffle = 268435456,
    NoPremierLeagueTeams = 536870912
};

inline FifamBeg operator|(FifamBeg lhs, FifamBeg rhs) {
    return static_cast<FifamBeg>(static_cast<UInt>(lhs) | static_cast<UInt>(rhs));
}

inline FifamBeg& operator|=(FifamBeg& lhs, FifamBeg rhs) {
    lhs = static_cast<FifamBeg>(static_cast<UInt>(lhs) | static_cast<UInt>(rhs));
    return lhs;
}

template<>
inline NameIdTable<FifamBeg> const &GetNameIdTable<FifamBeg>() {
    static NameIdTable<FifamBeg> table = {
        { L"BEG_1ST_LEG", FifamBeg::_1stLeg },
        { L"BEG_WITH_1_LEG", FifamBeg::With1stLeg },
        { L"BEG_2ND_LEG", FifamBeg::_2ndLeg },
        { L"BEG_WITH_2_LEG", FifamBeg::With2ndLeg },
        { L"BEG_WITH_REPLAY", FifamBeg::WithReplay },
        { L"BEG_WITH_EXTRA_TIME", FifamBeg::WithExtraTime },
        { L"BEG_WITH_GOLDEN_GOAL", FifamBeg::WithGoldenGoal },
        { L"BEG_WITH_PENALTY", FifamBeg::WithPenalty },
        { L"BEG_END", FifamBeg::End },
        { L"BEG_1ST_PLAYED", FifamBeg::_1stPlayed },
        { L"BEG_2ND_PLAYED", FifamBeg::_2ndPlayed },
        { L"BEG_EXTRA_TIME", FifamBeg::ExtraTime },
        { L"BEG_GOLDEN_GOAL", FifamBeg::GoldenGoal },
        { L"BEG_PENALTY", FifamBeg::Penalty },
        { L"BEG_FINISHED", FifamBeg::Finished },
        { L"BEG_WITH_SILVER_GOAL", FifamBeg::WithSilverGoal },
        { L"BEG_WITHOUT_AWAY_GOAL", FifamBeg::WithoutAwayGoal },
        { L"BEG_NO_SHUFFLE", FifamBeg::NoShuffle },
        { L"BEG_NO_PREMIERLEAGUE_TEAMS", FifamBeg::NoPremierLeagueTeams }
    };
    return table;
};
