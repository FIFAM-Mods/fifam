#pragma once

#include "FifaDbEnvironment.h"

class ShowPlayers {
public:
    ShowPlayers() {
        FifaDatabase *db = GetEnvironment<FifaDbEnvironment>().GetDatabase();
        db->ForAllMalePlayers([](FifaPlayer &player) {
            std::wcout << player.m_quickName << " |";
            player.ForAllTeams([] (FifaTeam &team) {
                if (team.m_league && !team.m_league->IsInternational())
                    std::wcout << " " << team.m_name;
            });
            std::wcout << std::endl;
        });
    }
};
