#pragma once
#include "FifamDbEnvironment.h"
#include "FifamReadWrite.h"
#include "FifamPlayer.h"
#include "FifaPlayer.h"
#include "FifamPlayerAppearance.h"
#include "FifaDatabase.h"
#include "Random.h"

class CopyHistoric {
public:
    String LineData(String const &line) {
        String t = line;
        if (auto pos = t.find(L"//"); pos != String::npos)
            t = t.substr(0, pos);
        if (auto pos = t.find(L'='); pos != String::npos)
            t = t.substr(pos + 1);
        Utils::Trim(t);
        return t;
    }

    String ClubName(FifamClub *club) {
        if (club) {
            String countryName = L"N/A";
            if (club->mCountry)
                countryName = FifamTr(club->mCountry->mName);
            return FifamTr(club->mName) + L" (" + countryName + L")";
        }
        return L"N/A";
    }

    void UpdateWinYearsList(Vector<UShort> &vec) {
        sort(vec.begin(), vec.end());
        vec.erase(unique(vec.begin(), vec.end()), vec.end());
    }

    CopyHistoric() {
        Path historicPath = L"D:\\Games\\FIFA Manager 25\\fmdata\\historic";
        FifamDatabase::mReadingOptions.mUseCustomFormations = true;
        FifamDatabase db(13, "db");
        for (UInt countryId = 1; countryId <= FifamDatabase::NUM_COUNTRIES; countryId++) {
            String filename = Utils::Format(L"%dcf.txt", countryId);
            FifamReader r(historicPath / filename);
            if (r.Available()) {
                wcout << L"Reading " << filename << endl;
                Map<UInt, FifamClub *> teams;
                Map<String, FifamCompType> comps;
                Map<String, Set<UShort>> uniqueYears;
                while (!r.IsEof()) {
                    String line = r.ReadFullLine();
                    if (Utils::StartsWith(line, L"#TEAM")) {
                        String t = LineData(line);
                        auto p = Utils::Split(t, L',');
                        if (p.size() == 2) {
                            UInt teamUID = Utils::SafeConvertInt<UInt>(p[1], true);
                            if (teamUID != 0) {
                                auto team = db.GetClubFromUID(teamUID);
                                if (team)
                                    teams[Utils::SafeConvertInt<UInt>(p[0])] = team;
                            }
                        }
                    }
                    else if (Utils::StartsWith(line, L"#COMP")) {
                        String t = LineData(line);
                        auto p = Utils::Split(t, L',');
                        if (p.size() == 5 && p[4] == L"ROUND_FINAL" && p[3] == L"0") {
                            UInt compTypeInt = Utils::SafeConvertInt<UInt>(p[2]);
                            if (FifamCompType::Present(compTypeInt))
                                comps[p[0]].SetFromInt(compTypeInt);
                        }
                    }
                    else if (Utils::StartsWith(line, L"#MATCH")) {
                        String t = LineData(line);
                        auto p = Utils::Split(t, L',');
                        if (p.size() >= 4) {
                            UInt year = Utils::SafeConvertInt<UInt>(p[1]);
                            UInt teamIndex = Utils::SafeConvertInt<UInt>(p[2]);
                            if (Utils::Contains(comps, p[0]) && year != 0 && Utils::Contains(teams, teamIndex)) {
                                switch (comps[p[0]].ToInt()) {
                                case FifamCompType::League:
                                    teams[teamIndex]->mHistory.mLeagueWinYears.push_back(year);
                                    break;
                                case FifamCompType::FaCup:
                                    teams[teamIndex]->mHistory.mCupWinYears.push_back(year);
                                    break;
                                case FifamCompType::LeagueCup:
                                    teams[teamIndex]->mHistory.mLeagueCupWinYears.push_back(year);
                                    break;
                                case FifamCompType::SuperCup:
                                    teams[teamIndex]->mHistory.mSuperCupsWinYears.push_back(year);
                                    break;
                                }
                                if (Utils::Contains(uniqueYears[p[0]], year))
                                    wcout << L"Duplicated year: " << year << L" in " << p[0] << endl;
                                else
                                    uniqueYears[p[0]].insert(year);
                            }
                        }
                    }
                }
            }
        }
        for (FifamClub *c : db.mClubs) {
            UpdateWinYearsList(c->mHistory.mLeagueWinYears);
            UpdateWinYearsList(c->mHistory.mCupWinYears);
            UpdateWinYearsList(c->mHistory.mLeagueCupWinYears);
            UpdateWinYearsList(c->mHistory.mSuperCupsWinYears);
        }
        db.mWritingOptions.mWriteFixtures = false;
        db.mWritingOptions.mWriteExternalScripts = false;
        db.Write(13, FifamDatabase::GetGameDbVersion(13), "output\\database");

        Pair<FifamClub *, UInt> maxLeagueWins = { nullptr, 0 };
        Pair<FifamClub *, UInt> maxCupWins = { nullptr, 0 };
        Pair<FifamClub *, UInt> maxLeagueCupWins = { nullptr, 0 };
        Pair<FifamClub *, UInt> maxSuperCupWins = { nullptr, 0 };
        for (FifamClub *c : db.mClubs) {
            if (c->mHistory.mLeagueWinYears.size() > maxLeagueWins.second) {
                maxLeagueWins.first = c;
                maxLeagueWins.second = c->mHistory.mLeagueWinYears.size();
            }
            if (c->mHistory.mCupWinYears.size() > maxCupWins.second) {
                maxCupWins.first = c;
                maxCupWins.second = c->mHistory.mCupWinYears.size();
            }
            if (c->mHistory.mLeagueCupWinYears.size() > maxLeagueCupWins.second) {
                maxLeagueCupWins.first = c;
                maxLeagueCupWins.second = c->mHistory.mLeagueCupWinYears.size();
            }
            if (c->mHistory.mSuperCupsWinYears.size() > maxSuperCupWins.second) {
                maxSuperCupWins.first = c;
                maxSuperCupWins.second = c->mHistory.mSuperCupsWinYears.size();
            }
        }
        wcout << endl;
        wcout << L"Max league titles: " << ClubName(maxLeagueWins.first) << L" - " << maxLeagueWins.second << endl;
        wcout << L"Max cup titles: " << ClubName(maxCupWins.first) << L" - " << maxCupWins.second << endl;
        wcout << L"Max leaguecup titles: " << ClubName(maxLeagueCupWins.first) << L" - " << maxLeagueCupWins.second << endl;
        wcout << L"Max supercup titles: " << ClubName(maxSuperCupWins.first) << L" - " << maxSuperCupWins.second << endl;
    }
};
