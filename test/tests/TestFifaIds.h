#pragma once
#include "FifamDatabase.h"

class TestFifaIds {
public:
    TestFifaIds() {
        FifamDatabase::mReadingOptions.mUseCustomFormations = true;
        FifamDatabase db(13, "D:\\Games\\FIFA Manager 22\\database");
        db.SetupWriteableStatus(13);
        Map<UInt, Vector<FifamPlayer *>> fifaIds;
        for (FifamPlayer *p : db.mPlayers) {
            if (p->mFifaID != 0)
                fifaIds[p->mFifaID].push_back(p);
        }
        Vector<String> duplicatedPlayers;
        for (auto const &[fifaId, players] : fifaIds) {
            if (players.size() > 1) {
                for (auto const &p : players)
                    duplicatedPlayers.push_back(Utils::Format(L"%u - %s (%s, %u)", fifaId, p->GetName(), p->mWriteableStringID, p->mFootballManagerID));
            }
        }
        if (!duplicatedPlayers.empty()) {
            std::wcout << "Duplicated players:" << std::endl;
            for (auto const &s : duplicatedPlayers)
                std::wcout << s << std::endl;
        }
    }
};
