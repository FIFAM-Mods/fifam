#pragma once
#include "Magick++.h"
#include "FifamDbEnvironment.h"
#include "FifamNames.h"
#include "Fifa07Database.h"

class FM07toFIFA07 {
public:
    FM07toFIFA07() {
        Path gamePath = "D:\\Games\\FIFA 07";
        Path namesPath = gamePath / "plugins";
        Path remasterPath = "fifa07_remaster";
        Path outDbPath = remasterPath / "db";
        create_directories(namesPath);
        create_directories(outDbPath);
        Pair<FifamNation, UInt> leaguesToConvert[] = {
            { FifamNation::Argentina, 1 },
            { FifamNation::Finland, 1 },
            { FifamNation::Greece, 1 },
            { FifamNation::Hungary, 1 },
            { FifamNation::Ireland, 1 },
            { FifamNation::Romania, 1 },
            { FifamNation::Ukraine, 1 },
            { FifamNation::Israel, 1 },
            { FifamNation::Israel, 2 },
            { FifamNation::Russia, 1 },
            { FifamNation::Russia, 2 },
            { FifamNation::Austria, 2 },
            { FifamNation::Belgium, 2 },
            { FifamNation::Denmark, 2 },
            { FifamNation::Netherlands, 2 },
            { FifamNation::Poland, 2 },
            { FifamNation::Switzerland, 2 },
            { FifamNation::Scotland, 2 },
            { FifamNation::Scotland, 3 },
            { FifamNation::Germany, 3 },
            { FifamNation::Spain, 3 }
        };
        //Pair<FifamNation, UInt> restOfWorldTeams[] = {
        //
        //};
        Vector<Fifa07Team> mTeams;
        Vector<Fifa07Player> mPlayers;
        Vector<Fifa07League> mLeagues;
        FifamDatabase db(7, L"D:\\Games\\FIFA Manager 07\\database");

        UInt fifaLeagueNextId = 3'000;
        UInt fifaTeamNextId = 150'000;

        for (auto const& ltc : leaguesToConvert) {

        }

        std::sort(mLeagues.begin(), mLeagues.end(), [](Fifa07League const& a, Fifa07League const& b) {
            return a.leagueid < b.leagueid;
            });
        std::sort(mTeams.begin(), mTeams.end(), [](Fifa07Team const& a, Fifa07Team const& b) {
            return a.teamid < b.teamid;
            });

        Fifa07DataFile fifa07file;
        if (fifa07file.Open(outDbPath / "leagues.txt", Fifa07DataFile::Write, L"leagueid	leaguename	level	countryid	leagueprestige")) {
            for (auto& l : mLeagues) {
                Fifa07DataFile::Line line;
                fifa07file.BeforeLine(line);
                l.Parse(line);
                fifa07file.NextLine(line);
            }
            fifa07file.Close();
        }
        //if (fifa07file.Open(outDbPath / "teams.txt", Fifa07DataFile::Write, Fifa07Database::m_currentGameVersion == 7 ?
        //    L"teamid	teamname	generickitid	balltype	adboardid	stadiumid	genericbanner	trainingstadiumid	speechcountryid	opponent	assetid	teamchemistry	overallrating	signsyouthplayersfrom	signsplayersfromprimary	signsplayersfromsecondary	initialtransferbudget	internationalprestige	domesticprestige	homecrowdsize	teamprimarycolor	teamsecondarycolor" :
        //    L"teamid	teamname	generickitid	balltype	adboardid	stadiumid	genericbanner	trainingstadiumid	speechcountryid	opponent	assetid	teamchemistry	overallrating	signsyouthplayersfrom	signsplayersfromprimary	signsplayersfromsecondary	initialtransferbudget	internationalprestige	domesticprestige	homecrowdsize	teamprimarycolor	teamsecondarycolor	defaultformationid"))
        //{
        //    for (auto& t : mTeams) {
        //        Fifa07DataFile::Line line;
        //        fifa07file.BeforeLine(line);
        //        t.Parse(line);
        //        fifa07file.NextLine(line);
        //    }
        //    fifa07file.Close();
        //}
        if (fifa07file.Open(outDbPath / "leagueteamlinks.txt", Fifa07DataFile::Write, L"leagueid	teamid")) {
            for (auto& l : mLeagues) {
                for (auto teamid : l.teams) {
                    Fifa07DataFile::Line line;
                    fifa07file.BeforeLine(line);
                    line >> l.leagueid >> teamid;
                    fifa07file.NextLine(line);
                }
            }
            fifa07file.Close();
        }

        // nation names - localized
        // stadium names - localized
        FifamWriter translation(namesPath / "names.tr");
        for (auto& l : mLeagues) {
            translation.WriteLineWithSeparator(L' ', 10000000 + l.leagueid * 10 + 2, l.leaguename);
            translation.WriteLineWithSeparator(L' ', 10000000 + l.leagueid * 10 + 3, l.leaguename);
            translation.WriteLineWithSeparator(L' ', 10000000 + l.leagueid * 10 + 4, l.GetProperty<String>("NationalityName") + L" Leagues");
            translation.WriteLineWithSeparator(L' ', 10000000 + l.leagueid * 10 + 5, l.GetProperty<String>("CountryName"));
        }
        for (auto& t : mTeams) {
            translation.WriteLineWithSeparator(L' ', 30000000 + t.teamid * 10 + 0, t.GetProperty<String>("FullName"));
            translation.WriteLineWithSeparator(L' ', 30000000 + t.teamid * 10 + 1, t.teamname);
            translation.WriteLineWithSeparator(L' ', 30000000 + t.teamid * 10 + 2, t.teamname);
        }
        translation.Close();
    }
};
