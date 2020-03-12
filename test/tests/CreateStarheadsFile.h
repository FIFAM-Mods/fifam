#pragma once
#include "FifaDbEnvironment.h"
#include "FifamReadWrite.h"

using namespace std;

class CreateStarheadsFile {
public:
    CreateStarheadsFile() {
        std::wcout << L"Reading FIFA database..." << std::endl;
        FifaDatabase *fifadb = GetEnvironment<FifaDbEnvironment>().GetDatabase();
        Path headsPath = "D:\\Games\\FIFA Manager 13\\data\\assets";
        vector<FifaPlayer *> starHeads;
        fifadb->ForAllPlayers([&](FifaPlayer &player) {
            if (exists(headsPath / ("m228__" + to_string(player.GetId()) + ".o")))
                starHeads.push_back(&player);
        });
        std::wcout << L"Writing FaceIDs..." << std::endl;
        FifamWriter writer(L"D:\\Games\\FIFA Manager 13\\fmdata\\eng\\FaceIDs.txt", 14, FifamVersion());
        writer.WriteLine(starHeads.size());
        for (FifaPlayer *p : starHeads) {
            if (!p->m_commonName.empty())
                writer.WriteLine(p->m_commonName);
            else
                writer.WriteLine(p->m_lastName, p->m_firstName);
            writer.WriteLine(p->GetId());
        }
    }
};
