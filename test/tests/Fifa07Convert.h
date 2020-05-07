#pragma once

#pragma once
#include "FifaDbEnvironment.h"
#include "FifamReadWrite.h"

class Fifa07Convert {
public:
    Fifa07Convert() {
        FifaDatabase *db = GetEnvironment<FifaDbEnvironment>().GetDatabase();
        
        Path outPath = "C:\\Users\\Dmitri\\Desktop\\db\\edited";

        {
            //FifamWriter w(outPath / "teams.txt");
            //db->ForAllTeams([&](FifaPlayer &player) {
            //    String name = player.m_quickName;
            //    if (!player.IsMale())
            //        name += L" f";
            //    w.WriteLine(player.GetId(), Quoted(name));
            //});
        }
    }
};
