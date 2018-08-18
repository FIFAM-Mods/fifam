#pragma once
#include "FifamDbEnvironment.h"

template<typename Game, typename DbType>
class WritePlayerHistoryTest {
public:
    WritePlayerHistoryTest() {
        FifamDatabase *db = GetEnvironment<FifamDbEnvironment<Game, DbType>>().GetDatabase();

        FifamWriter writer(Utils::Format(L"player_hist_%02d.csv", Game::id()), Game::id(), Game::year(), Game::vernum(), Game::unicode());
        if (writer.Available()) {
            writer.WriteLine(
                L"Country",
                L"Club",
                L"PlayerId",
                L"PlayerName",
                L"EntryIndex",
                L"StartDate",
                L"EndDate",
                L"CareerClub",
                L"Matches",
                L"Goals",
                L"ReserveMatches",
                L"ReserveGoals",
                L"TransferFee",
                L"Loan",
                L"StillInThisClub",
                L"Unknown_1"
            );
            for (auto country : db->mCountries) {
                if (country) {
                    for (auto club : country->mClubs) {
                        for (auto player : club->mPlayers) {
                            for (UInt i = 0; i < player->mHistory.mEntries.size(); i++) {
                                writer.WriteLine(
                                    Quoted(FifamTr(country->mName)),
                                    Quoted(FifamTr(club->mName)),
                                    player->mID,
                                    Quoted(player->GetName()),
                                    i,
                                    player->mHistory.mEntries[i].mStartDate.ToString(),
                                    player->mHistory.mEntries[i].mEndDate.ToString(),
                                    Quoted(player->mHistory.mEntries[i].mClub.GetTeamName()),
                                    player->mHistory.mEntries[i].mMatches,
                                    player->mHistory.mEntries[i].mGoals,
                                    player->mHistory.mEntries[i].mReserveMatches,
                                    player->mHistory.mEntries[i].mReserveGoals,
                                    player->mHistory.mEntries[i].mTransferFee,
                                    player->mHistory.mEntries[i].mLoan,
                                    player->mHistory.mEntries[i].mStillInThisClub,
                                    player->mHistory.mEntries[i].Unknown._1
                                );
                            }
                        }
                    }
                }
            }
        }
    }
};
