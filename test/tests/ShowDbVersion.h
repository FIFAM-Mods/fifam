#pragma once

#include "FifaDbEnvironment.h"

class ShowDbVersion {
public:
    ShowDbVersion() {
        FifaDatabase *db = GetEnvironment<FifaDbEnvironment>().GetDatabase();
        std::wcout << db->GetVersionString() << L" " << db->GetVersionDateString();

        std::vector<FifaPlayer *> players[5];
        db->ForAllMalePlayers([&](FifaPlayer &p) { if (p.internal.personality != 0) players[p.internal.personality - 1].push_back(&p); });
        for (size_t i = 0; i < std::size(players); i++) {
            std::sort(players[i].begin(), players[i].end(), [](FifaPlayer *a, FifaPlayer *b) { return a->internal.overallrating > b->internal.overallrating; });
            std::wcout << std::endl << L"Personality " << i + 1 << L":" << std::endl;
            for (size_t j = 0; j < min(10, players[i].size()); j++)
                std::wcout << players[i][j]->m_quickName << std::endl;
        }
    }
};
