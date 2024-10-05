#pragma once

#include "FifaDbEnvironment.h"

void WriteCurrentPlayersToHistory(FifaDatabase *fifaDb, std::filesystem::path const &dbPath, unsigned int versionNumber) {
    std::filesystem::path filePath = dbPath / L"fifa_player_ratings" / Utils::Format(L"%d.bin", versionNumber);
    FILE *file = fopen(filePath.string().c_str(), "wb");
#pragma pack(push, 1)
    struct playerRatingInfo {
        unsigned int id;
        unsigned char rating;
        unsigned char potential;
    };
#pragma pack(pop)
    fifaDb->ForAllPlayers([&](FifaPlayer &p) {
        playerRatingInfo info;
        info.id = p.GetId();
        info.rating = p.internal.overallrating;
        info.potential = p.internal.potential;
        fwrite(&info, 6, 1, file);
        });
    fclose(file);
}


class ShowDbVersion {
public:
    ShowDbVersion() {
        FifaDatabase *db = GetEnvironment<FifaDbEnvironment>().GetDatabase();
        std::wcout << db->GetVersionString() << L" " << db->GetVersionDateString();

        //WriteCurrentPlayersToHistory(db, L"D:\\Projects\\fifam\\db", 18);
    }
};
