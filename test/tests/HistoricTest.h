#pragma once
#include "FifamDbEnvironment.h"

template<typename Game, typename DbType>
class HistoricTest {
public:
    HistoricTest() {
        FifamDatabase *db = GetEnvironment<FifamDbEnvironment<Game, DbType>>().GetDatabase();
        for (auto &entry : db->mHistoric.mWorstStartingStreaks) {
            std::wcout <<
                entry.mCompID.ToStr() << L", " <<
                entry.mClub.GetTeamName() << L", " <<
                entry.mSeason << L", " <<
                (UInt)entry.mMatches << std::endl;
        }
        std::wcout << std::endl;
        for (auto &entry : db->mHistoric.mFifaWorldPlayers) {
            std::wcout <<
                entry.mSeason << L", " <<
                entry.mNationality.ToStr() << L", " <<
                entry.mClub.GetTeamName() << L", " <<
                (UInt)entry.mAge << L", " <<
                entry.mPlayerPic << L", " <<
                entry.mLastNameOrPseudonym << L", " <<
                entry.mFirstName << std::endl;
        }
    }
};
