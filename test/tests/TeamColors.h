#pragma once
#include "FifamDbEnvironment.h"
#include "FifaDbEnvironment.h"

class TeamColors {
public:
    TeamColors() {
        FifamDatabase::mReadingOptions.mReadPersons = false;
        FifamDatabase *db = new FifamDatabase(13, "D:\\Games\\FIFA Manager 21\\database");
        FifamWriter w(L"popup_teamcolors.csv");
        if (w.Available()) {
            auto whiteColor = GenColor(255, 255, 255);
            auto blackColor = GenColor(0, 0, 0);
            w.WriteLine(L"TeamID", L"BackgroundColor", L"TextColor", L"Comment");
            auto country = db->GetCountry(FifamNation::Germany);
            if (country) {
                auto countryComps = country->GetCompetitions(true);
                for (auto comp : countryComps) {
                    if (comp.second->GetDbType() == FifamCompDbType::League) {
                        FifamCompLeague *league = comp.second->AsLeague();
                        for (UInt t = 0; t < league->mTeams.size(); t++) {
                            auto const &club = league->mTeams[t];
                            if (club.IsValid() && club.IsFirstTeam() && !club.mPtr->HasProperty(L"sortingID"))
                                club.mPtr->SetProperty<UInt>(L"sortingID", league->mID.mIndex * 100 + t);
                        }
                    }
                }
                std::sort(country->mClubs.begin(), country->mClubs.end(), [](FifamClub *a, FifamClub *b) {
                    return a->GetProperty<UInt>(L"sortingID", 9999) <= b->GetProperty<UInt>(L"sortingID", 9999);
                });
                for (auto const &c : country->mClubs) {
                    auto backgroundColor = c->mBackgroundColour;
                    auto white = GenColor::Distance(backgroundColor, whiteColor);
                    auto black = GenColor::Distance(backgroundColor, blackColor);
                    GenColor textColor = (white < black) ? blackColor : whiteColor;
                    w.WriteLine(Utils::Format(L"0x%08X", c->mUniqueID),
                        Utils::Format(L"0xFF%02X%02X%02X", backgroundColor.r, backgroundColor.g, backgroundColor.b),
                        Utils::Format(L"0xFF%02X%02X%02X", textColor.r, textColor.g, textColor.b),
                        Quoted(c->mName[FifamTranslation::English]));
                }
            }
        }
    }
};
