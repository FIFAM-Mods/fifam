#pragma once
#include "FifaDbEnvironment.h"
#include "FifamTypes.h"
#include "Utils.h"
#include "Error.h"

class SortFifaPortraits {
public:
    static void MyCopy(Path const &from, Path const &to) {
        create_directories(to.parent_path());
        copy(from, to);
    };
    
    SortFifaPortraits() {
        //FifaDatabase::m_firstSupportedGameVersion = FifaDatabase::m_lastSupportedGameVersion;
        FifaDatabase *fifadb = GetEnvironment<FifaDbEnvironment>().GetDatabase();
        path pp = "D:\\FC24_portraits";
        path out = pp / "sorted";
        create_directories(out);
        for (auto const &i : directory_iterator(pp)) {
            auto const &p = i.path();
            if (is_regular_file(p) && p.extension() == ".png") {
                unsigned int id = Utils::SafeConvertInt<unsigned int>(p.stem().string());
                if (id > 0) {
                    auto player = fifadb->GetPlayer(id);
                    if (player) {
                        String filename;
                        if (player->m_commonName.empty()) {
                            if (player->m_lastName.empty())
                                filename = player->m_firstName + L" - ";
                            else {
                                if (player->m_firstName.empty())
                                    filename = player->m_lastName + L" - ";
                                else
                                    filename = player->m_lastName + L" " + player->m_firstName + L" - ";
                            }
                        }
                        else
                            filename = player->m_commonName + L" - ";
                        filename += Utils::Format(L"%d.png", id);
                        if (player->m_gameId == FifaDatabase::m_currentGameVersion) {
                            FifaTeam *team = player->GetLeagueTeam();
                            if (!team)
                                team = player->GetInternationalTeam();
                            if (team) {
                                FifaLeague *league = team->m_league;
                                if (league) {
                                    if (league->m_name.empty()) {
                                        if (team->m_name.empty())
                                            MyCopy(p, out / "Empty_League_Name" / "Empty_Team_Name" / filename);
                                        else
                                            MyCopy(p, out / "Empty_League_Name" / team->m_name / filename);
                                    }
                                    else {
                                        if (team->m_name.empty())
                                            MyCopy(p, out / league->m_name / "Empty_Team_Name" / filename);
                                        else
                                            MyCopy(p, out / league->m_name / team->m_name / filename);
                                    }
                                }
                                else {
                                    if (team->m_name.empty())
                                        MyCopy(p, out / L"No_League" / "Empty_Team_Name" / filename);
                                    else
                                        MyCopy(p, out / L"No_League" / team->m_name / filename);
                                }
                            }
                            else
                                MyCopy(p, out / L"_Without_Team" / filename);
                        }
                        else
                            MyCopy(p, out / L"_Unknown_Named" / filename);
                    }
                    else
                        MyCopy(p, out / L"_Unknown_Unnamed" / p.filename());
                }
                else
                    ::Error("incorrect filename: %s", p.string().c_str());
            }
        }
    }
};
