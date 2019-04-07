#pragma once
#include "FifamDbEnvironment.h"
#include "FifamCompLeague.h"
#include "FifamCompCup.h"
#include "FifamUtils.h"

template<typename Game>
class ExportCompInfo {
public:
    ExportCompInfo() {
        FifamDatabase::mReadingOptions.mReadInternationalCompetitions = false;
        FifamDatabase::mReadingOptions.mReadPersons = false;
        FifamDatabase *db = GetEnvironment<FifamDbEnvironment<Game, Default>>().GetDatabase();
        db->SetupWriteableStatus(13);
        create_directories("data");
        for (auto[compId, comp] : db->mCompMap) {
            if (compId.mRegion.ToInt() > 0 && compId.mRegion.ToInt() <= 207 &&
                (comp->GetDbType() == FifamCompDbType::League || comp->GetDbType() == FifamCompDbType::Cup))
            {
                FifamWriter writer(L"data\\" + compId.ToHexStr() + L".txt", 13, 0, 0);
                writer.WriteLine(L"name", Quoted(FifamTr(comp->mName)));
                if (comp->GetDbType() == FifamCompDbType::League) {
                    FifamCompLeague *lg = comp->AsLeague();
                    if (!lg->mTeams.empty()) {
                        writer.Write(L"teams");
                        auto teams = FifamUtils::MakeWriteableIDsList(lg->mTeams);
                        for (UInt i = 0; i < lg->mTeams.size(); i++)
                            writer.Write(L"," + Utils::Format(L"%x", teams[i]));
                        writer.WriteNewLine();
                    }
                    //writer.WriteLine(L"bonuses", lg->mBonuses[0], lg->mBonuses[1], lg->mBonuses[2], lg->mBonuses[3]);
                    //if (!lg->mFirstSeasonMatchdays.empty()) {
                    //    writer.Write(L"matchdays,");
                    //    writer.WriteLineArray(lg->mFirstSeasonMatchdays);
                    //}
                    //if (!lg->mSecondSeasonMatchdays.empty()) {
                    //    writer.Write(L"matchdays2,");
                    //    writer.WriteLineArray(lg->mSecondSeasonMatchdays);
                    //}
                    //if (!lg->mFixtures.empty()) {
                    //    writer.Write(L"fixtures");
                    //    for (UInt i = 0; i < lg->mFixtures.size(); i++) {
                    //        for (UInt f = 0; f < lg->mFixtures[i].size(); f++) {
                    //            writer.Write(L",");
                    //            writer.Write(lg->mFixtures[i][f].first, lg->mFixtures[i][f].second);
                    //        }
                    //    }
                    //    writer.WriteNewLine();
                    //}
                    writer.WriteLine(L"attendance", lg->mAttendanceMp);
                    writer.WriteLine(L"transfers", lg->mTransferMarketMp);
                }
                //else if (comp->GetDbType() == FifamCompDbType::Cup) {
                //    FifamCompCup *cup = comp->AsCup();
                //    if (!cup->mRounds.empty()) {
                //        writer.Write(L"bonuses");
                //        for (auto const &r : cup->mRounds) {
                //            writer.Write(L",");
                //            writer.WriteArray(r.mBonuses);
                //        }
                //        writer.WriteNewLine();
                //    }
                //    if (!cup->mFirstSeasonMatchdays.empty()) {
                //        writer.Write(L"matchdays,");
                //        writer.WriteLineArray(cup->mFirstSeasonMatchdays);
                //    }
                //    if (!cup->mSecondSeasonMatchdays.empty()) {
                //        writer.Write(L"matchdays2,");
                //        writer.WriteLineArray(cup->mSecondSeasonMatchdays);
                //    }
                //}
            }
        }
    }
};
