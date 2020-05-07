#pragma once
#include "FifaDbEnvironment.h"
#include "FifamReadWrite.h"

class ShowPlayers {
public:
    ShowPlayers() {
        FifaDatabase *db = GetEnvironment<FifaDbEnvironment>().GetDatabase();
        std::wcout << L"FIFA DB Version " << db->GetVersionDateString() << std::endl;
        FifamWriter w("fifa_players.csv");
        db->ForAllPlayers([&](FifaPlayer &player) {
            String name = player.m_quickName;
            if (!player.IsMale())
                name += L" f";
            w.WriteLine(player.GetId(), Quoted(name));
            //if (player.m_gameId == 18) {
            //    std::wcout << player.GetId() << ". " <<player.m_quickName << " |";
            //    player.ForAllTeams([](FifaTeam &team) {
            //        if (team.m_league && !team.m_league->IsInternational())
            //            std::wcout << " " << team.m_name;
            //    });
            //    std::wcout << std::endl;
            //}
        });
    }
};
