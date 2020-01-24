#pragma once
#include "FifamDbEnvironment.h"
#include "FifamReadWrite.h"

class ExportJapanLeaguePlayers {
public:
    ExportJapanLeaguePlayers() {
        FifamDatabase *db = GetEnvironment<FifamDbEnvironment<FM13, Default>>().GetDatabase();

        FifamWriter writer(L"D:\\Projects\\fifam\\content\\JPlayers.txt", 14, FifamVersion());
        if (writer.Available()) {
            for (auto c : db->GetCountry(FifamNation::Japan)->mClubs) {
                for (auto p : c->mPlayers) {
                    writer.WriteLine(p->GetStringUniqueId(13, true) + L".png");
                    writer.WriteLine(p->GetStringUniqueId(13, false) + L".png");
                }
            }
        }
    }
};
