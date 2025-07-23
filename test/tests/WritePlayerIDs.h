#pragma once
#include "FifamDatabase.h"
#include "FifamPlayerLevel.h"
#include "TextFileTable.h"

class WritePlayerIDs {
public:
    WritePlayerIDs() {
        FifamDatabase db(13, "D:\\Games\\FIFA Manager 25\\database");
        TextFileTable table;
        table.AddRow("country", "club", "nation", "lastname", "firstname", "birthdate", "level", "talent");
        for (auto player : db.mPlayers) {
            String countryName;
            String clubName;
            if (player->mClub) {
                clubName = FifamTr(player->mClub->mName);
                if (player->mClub->mCountry)
                    countryName = FifamTr(player->mClub->mCountry->mName);
            }
            String firstName = player->mPseudonym.empty() ? player->mFirstName : String();
            String lastName = player->mPseudonym.empty() ? player->mLastName : player->mPseudonym;
            table.AddRow(countryName, clubName, player->mNationality[0].ToStr(), lastName, firstName,
                player->mBirthday.ToString(), player->GetLevel(), player->mTalent);
        }
        table.WriteUnicodeText("players.txt");
    }
};
