#pragma once

#include "FifaDbEnvironment.h"

class ShowPlayers {
public:
    ShowPlayers() {
        FifaDatabase *db = GetEnvironment<FifaDbEnvironment>().GetDatabase();
        std::wcout << L"FIFA DB Version " << db->GetVersionDateString() << std::endl;
        db->ForAllMalePlayers([](FifaPlayer &player) {
            if (player.m_gameId == 18) {
                std::wcout << player.GetId() << ". " <<player.m_quickName << " |";
                player.ForAllTeams([](FifaTeam &team) {
                    if (team.m_league && !team.m_league->IsInternational())
                        std::wcout << " " << team.m_name;
                });
                std::wcout << std::endl;
            }
        });
    }
};
