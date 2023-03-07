#pragma once
#include "FifamDbEnvironment.h"
#include "FifamCompLeague.h"

class CreateWorldCupDatabase {
public:
    CreateWorldCupDatabase() {
        FifamDatabase::mReadingOptions.mUseCustomFormations = true;
        FifamDatabase db(13, L"db");
        db.SetupWriteableStatus(13);
        Map<FifamClub *, FifamCompLeague *> clubLeague;
        for (auto const &[id, c] : db.mCompMap) {
            FifamCompLeague *l = c->AsLeague();
            if (l && l->mID.mType == FifamCompType::League) {
                for (auto const &t : l->mTeams) {
                    if (t.IsValid())
                        clubLeague[t.mPtr] = l;
                }
            }
        }
        Set<UInt> wcCountries = {
            182, 14, 52, 18, 45, 7, 54, 38, 57, 161, 183, 195, 72, 70, 51, 117, 136, 95, 83, 13, 21, 129, 47, 60, 34, 50, 37, 145, 163, 10, 103, 167
        };
        Set<String> additionalPlayers = {
            L"RodriguezKe04032000",
            L"AguileraBr28062003",
            L"SequeiraPa01031999",
            L"HernandezAn11102001",
            L"DengTh20031997"
        };
        Map<String, FifamPlayer *> playersMap;
        Map<UInt, Vector<Pair<FifamPlayer *, UInt>>> mapCountries;
        for (auto p : db.mPlayers) {
            if (playersMap.contains(p->mWriteableStringID))
                ::Error(L"Duplicate player ID '%s'", p->mWriteableStringID.c_str());
            playersMap[p->mWriteableStringID] = p;
            p->SetProperty<Bool>(L"ignore", false);
            p->mStartingConditions.mInjury.Disable();
            p->mStartingConditions.mBanUntil.Disable();
            p->mStartingConditions.mLeagueBan.Disable();
            p->mStartingConditions.mLoan.Disable();
            p->mStartingConditions.mFutureJoin.Disable();
            p->mStartingConditions.mFutureLeave.Disable();
            p->mStartingConditions.mFutureLoan.Disable();
            p->mStartingConditions.mFutureReLoan.Disable();
            p->mStartingConditions.mFutureTransfer.Disable();
            if (p->mClub) {
                UInt maxClubIndex = 0;
                switch (p->mClub->mCountry->mId) {
                case FifamNation::England:
                case FifamNation::Spain:
                case FifamNation::Germany:
                case FifamNation::Italy:
                case FifamNation::France:
                case FifamNation::Poland:
                    maxClubIndex = db.GetCompetition(FifamCompID(p->mClub->mCountry->mId, FifamCompType::League, 0))->mNumTeams + 5;
                    break;
                case FifamNation::Qatar:
                    maxClubIndex = 9999;
                    break;
                default:
                    maxClubIndex = 8;
                    break;
                }
                if ((p->mClub->GetWriteableID() & 0xFFFF) > maxClubIndex)
                    p->SetProperty<Bool>(L"ignore", true);
            }
            else
                p->SetProperty<Bool>(L"ignore", true);
            mapCountries[p->mNationality[0].ToInt()].emplace_back(p, p->GetLevel());
        }
        auto IncludePlayer = [](FifamPlayer *p) {
            if (p->GetProperty<Bool>(L"ignore")) {
                if (p->mClub) {
                    p->mClub->mPlayers.erase(std::remove(p->mClub->mPlayers.begin(), p->mClub->mPlayers.end(), p), p->mClub->mPlayers.end());
                    p->mClub = nullptr;
                    p->mInReserveTeam = false;
                    p->mInYouthTeam = false;
                    p->mShirtNumberFirstTeam = 0;
                    p->mShirtNumberReserveTeam = 0;
                }
                p->SetProperty<Bool>(L"ignore", false);
            }
        };
        for (auto &[countryId, pl] : mapCountries) {
            std::sort(pl.begin(), pl.end(), [](Pair<FifamPlayer *, UInt> const &a, Pair<FifamPlayer *, UInt> const &b) {
                return a.second > b.second;
            });
            UInt plCount = wcCountries.contains(countryId) ? 200 : 100;
            for (UInt i = 0; i < Utils::Min(plCount, pl.size()); i++)
                IncludePlayer(pl[i].first);
        }
        for (auto const &s : additionalPlayers) {
            if (playersMap.contains(s))
                IncludePlayer(playersMap[s]);
            else
                ::Error(L"Can't find player with ID '%s'", s.c_str());
        }
        db.Write(13, FifamDatabase::GetGameDbVersion(13), "db_wc");
    }
};
