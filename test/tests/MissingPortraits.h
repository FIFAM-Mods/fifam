#pragma once
#include "FifamDbEnvironment.h"
#include "FifamPlayerLevel.h"

class MissingPortraits {
public:
    static Bool HasPortrait(FifamPlayer *p) {
        static String portraitFolders[] = { L"art_02", L"art_03", L"art_06", L"art_07", L"art_08" };
        static Path contentPath = L"D:\\Projects\\fifam\\content\\fm13";
        for (UInt i = 0; i < 2; i++) {
            String portraitName = p->GetStringUniqueId(FM13::id(), i == 0) + L".png";
            for (auto const &s : portraitFolders) {
                if (exists(contentPath / s / L"portraits" / L"club" / L"160x160" / portraitName))
                    return true;
            }
        }
        return false;
    };

    MissingPortraits() {
        FifamDatabase db(13, "D:\\Games\\FIFA Manager 22\\database");
        FifamWriter writer(Utils::Format(L"players_missing_portraits.csv", FM13::id()), FM13::id(), FifamVersion(FM13::year(), FM13::vernum()), FM13::unicode());
        if (writer.Available()) {
            writer.WriteLine(
                L"Country",
                L"Club",
                L"FoomID",
                L"Creator",
                L"Name",
                L"Level",
                L"Talent"
            );
            for (auto player : db.mPlayers) {
                if (!HasPortrait(player)) {
                    String countryName;
                    String clubName;
                    if (player->mClub) {
                        clubName = FifamTr(player->mClub->mName);
                        if (player->mClub->mCountry)
                            countryName = FifamTr(player->mClub->mCountry->mName);
                    }
                    String playerName = player->GetName();
                    writer.WriteLine(
                        Quoted(countryName),
                        Quoted(clubName),
                        player->mFootballManagerID,
                        player->mCreator,
                        Quoted(playerName),
                        player->GetLevel(),
                        player->mTalent
                    );
                }
            }
        }
    }
};
