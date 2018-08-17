#pragma once
#include "FifamDbEnvironment.h"
#include "FifamFlags.h"
#include "FifamBeg.h"

template<typename Game, typename DbType>
class FlagsTest {
public:
    FlagsTest() {
        // присвоение флагов из из перечисления
        FifamFlags<FifamBeg> flags = FifamBeg::WithPenalty | FifamBeg::WithExtraTime | FifamBeg::NoPremierLeagueTeams;
        std::wcout << flags.ToStr() << std::endl;

        // присвоение флагов из строки с разделителем
        flags = L"BEG_GOLDEN_GOAL,BEG_2ND_PLAYED,BEG_NO_SHUFFLE,BEG_WITHOUT_AWAY_GOAL";
        std::wcout << flags.ToStr() << std::endl;
        
        // присвоение флагов из массива строк
        flags = { L"BEG_1ST_PLAYED", L"BEG_END", L"BEG_WITH_2_LEG" };
        std::wcout << flags.ToStr();
    }
};
