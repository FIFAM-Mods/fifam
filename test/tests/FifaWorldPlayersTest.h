#pragma once
#include "FifamDbEnvironment.h"

template<typename Game, typename DbType>
class FifaWorldPlayersTest {
public:
    FifaWorldPlayersTest() {
        FifamDatabase *db = GetEnvironment<FifamDbEnvironment<Game, DbType>>().GetDatabase();
        for (auto &worldPlayer : db->mHistoric.mFifaWorldPlayers) {
            std::wcout <<
                worldPlayer.mSeason << L", " <<
                worldPlayer.mNationality.ToStr() << L", " <<
                worldPlayer.mClub.GetTeamName() << L", " <<
                (UInt)worldPlayer.mAge << L", " <<
                worldPlayer.mPlayerPic << L", " <<
                worldPlayer.mLastNameOrPseudonym << L", " <<
                worldPlayer.mFirstName << std::endl;
        }
    }
};
