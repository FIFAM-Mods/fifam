#pragma once
#include "FifamDbEnvironment.h"
#include "FifamPlayerLevel.h"

template<typename Game, typename DbType>
class WritePlayerIDs {
public:
    WritePlayerIDs() {
        FifamDatabase *db = GetEnvironment<FifamDbEnvironment<Game, DbType>>().GetDatabase();

        Map<String, Vector<FifamPlayer *>> playerStrIDsMap;
        for (auto player : db->mPlayers)
            playerStrIDsMap[player->GetStringUniqueId(Game::id(), false)].push_back(player);

        FifamWriter writer(Utils::Format(L"players_ids_%02d.csv", Game::id()), Game::id(), FifamVersion(Game::year(), Game::vernum()), Game::unicode());
        if (writer.Available()) {
            writer.WriteLine(
                L"Country",
                L"Club",
                L"FoomID",
                L"PlayerID",
                L"LastName",
                L"FirstName",
                L"Pseudonym",
                L"Birthday",
                L"Age",
                L"Nationality1",
                L"Nationality2",
                L"Position",
                L"Level",
                L"Apps",
                L"Goals"
            );
            for (auto player : db->mPlayers) {
                String countryName;
                String clubName;
                if (player->mClub) {
                    clubName = FifamTr(player->mClub->mName);
                    if (player->mClub->mCountry)
                        countryName = FifamTr(player->mClub->mCountry->mName);
                }
                String playerIdStr;
                auto it = playerStrIDsMap.find(player->GetStringUniqueId(Game::id(), false));
                if (it != playerStrIDsMap.end()) {
                    if ((*it).second.size() == 1)
                        playerIdStr = (*it).first;
                    else
                        playerIdStr = (*it).first + Utils::Format(L"-%d", player->mEmpicsId);
                }
                writer.WriteLine(
                    Quoted(countryName),
                    Quoted(clubName),
                    player->mEmpicsId,
                    Quoted(playerIdStr),
                    Quoted(player->mLastName),
                    Quoted(player->mFirstName),
                    Quoted(player->mPseudonym),
                    player->mBirthday.ToString(),
                    player->GetAge(FifamDate(1, 7, 2018)),
                    Quoted(player->mNationality[0].ToStr()),
                    Quoted(player->mNationality[1].ToStr()),
                    player->mMainPosition.ToStr(),
                    FifamPlayerLevel::GetPlayerLevel13(player, player->mMainPosition, player->mPlayingStyle) / 5 + 1,
                    player->mNationalTeamMatches,
                    player->mNationalTeamGoals
                );
            }
        }
    }
};
