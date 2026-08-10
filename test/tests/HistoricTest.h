#pragma once
#include "TestRegistry.h"
#include "FifamDbEnvironment.h"

class HistoricTest {
public:
    HistoricTest() {
        FifamDatabase db(13, "db");
        for (auto &entry : db.mHistoric.mWorstStartingStreaks) {
            std::wcout <<
                entry.mCompetition.ToStr() << L", " <<
                entry.mClub.GetTeamName() << L", " <<
                entry.mSeason << L", " <<
                (UInt)entry.mMatches << std::endl;
        }
        std::wcout << std::endl;
        for (auto &entry : db.mHistoric.mFifaWorldPlayers) {
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

REGISTER_TEST(HistoricTest)
