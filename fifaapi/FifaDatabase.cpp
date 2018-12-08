#include "FifaDatabase.h"
#include "FifaDataFile.h"
#include "Error.h"
#include "Utils.h"

unsigned int FifaDatabase::m_currentGameVersion = FIFA_DATABASE_LATEST_GAME_VERSION;

FifaDatabase::FifaDatabase(std::filesystem::path const &path) {
    m_path = path;
    FifaDataFile file;
    std::map<unsigned int, std::wstring> names;
    bool latestDbIsRead = false;
    bool playersRead = false;
    bool teamsRead = false;
    bool managersRead = false;
    bool refereesRead = false;
    bool stadiumsRead = false;
    bool leaguesRead = false;
    std::filesystem::path playerloansPath;
    std::filesystem::path leagueteamlinksPath;
    std::filesystem::path teamplayerlinksPath;
    std::filesystem::path leaguerefereelinksPath;
    std::filesystem::path teamstadiumlinksPath;
    for (unsigned int i = m_lastSupportedGameVersion; i >= m_firstSupportedGameVersion; i--) {
        m_currentGameVersion = i;
        std::filesystem::path gamedbpath = path / std::to_wstring(i);
        if (std::filesystem::exists(gamedbpath)) {
            std::wstring defaultFolderName;
            if (std::filesystem::exists(gamedbpath / L"default"))
                defaultFolderName = L"default";
            else if (std::filesystem::exists(gamedbpath / L"_default"))
                defaultFolderName = L"_default";
            if (defaultFolderName.empty()) {
                Error(L"Can't find default database for game id '%d'", i);
                continue;
            }
            // read default player names
            names.clear();
            std::wstring name;
            int nameid, commentaryid;
            if (!file.Open(gamedbpath / defaultFolderName / L"playernames.txt")) {
                Error("Can't open default database/playernames file for game id '%d'", i);
                continue;
            }
            for (FifaDataFile::Line line; file.NextLine(line); ) {
                line >> name >> nameid >> commentaryid;
                names[nameid] = name;
            }
            file.Close();
            // read default player additional names
            if (file.Open(gamedbpath / defaultFolderName / L"dcplayernames.txt")) {
                for (FifaDataFile::Line line; file.NextLine(line); ) {
                    line >> name >> nameid;
                    names[nameid] = name;
                }
                file.Close();
            }

            std::vector<std::wstring> dbFolders;

            for (auto const& dbs : std::filesystem::directory_iterator(gamedbpath)) {
                if (dbs.path().filename() != L"default" && dbs.path().filename() != L"_default")
                    dbFolders.push_back(dbs.path().filename().c_str());
            }
            std::sort(dbFolders.begin(), dbFolders.end(), std::greater<std::wstring>());
            dbFolders.push_back(defaultFolderName);

            for (auto const &strFolder : dbFolders) {
                bool isDefaultVersion = strFolder == defaultFolderName;
                // read version
                if (!latestDbIsRead) {
                    if (file.Open(gamedbpath / strFolder / L"version.txt")) {
                        FifaDataFile::Line line;
                        if (file.NextLine(line))
                            m_version.FromLine(line);
                        else
                            Error("%s: invalid database/version file format", __FUNCTION__);
                        file.Close();
                    }
                    else
                        Error("%s: can't open database/version file", __FUNCTION__);
                }
                auto playernames = names;
                if (!isDefaultVersion && file.Open(gamedbpath / strFolder / L"dcplayernames.txt")) {
                    for (FifaDataFile::Line line; file.NextLine(line); ) {
                        line >> name >> nameid;
                        playernames[nameid] = name;
                    }
                    file.Close();
                }
                // read players
                if (file.Open(gamedbpath / strFolder / L"players.txt")) {
                    for (FifaDataFile::Line line; file.NextLine(line); ) {
                        FifaPlayer *player = new FifaPlayer(line);
                        auto oldPlayer = m_players.find(player->GetId());
                        if (oldPlayer != m_players.end())
                            delete player;
                        else {
                            m_players[player->GetId()] = player;
                            player->m_gameId = m_currentGameVersion;
                            player->m_firstName = playernames[player->internal.firstnameid];
                            player->m_lastName = playernames[player->internal.lastnameid];
                            player->m_commonName = playernames[player->internal.commonnameid];
                            player->m_playerJerseyName = playernames[player->internal.playerjerseynameid];
                            player->m_quickName = Utils::GetQuickName(player->m_firstName, player->m_lastName, player->m_commonName);
                            player->m_birthDate.Set(player->internal.birthdate);
                        }
                    }
                    file.Close();
                    playersRead = true;
                }
                // read leagues
                if (!leaguesRead && file.Open(gamedbpath / strFolder / L"leagues.txt")) {
                    for (FifaDataFile::Line line; file.NextLine(line); )
                        AddEntity(m_leagues, line);
                    file.Close();
                    leaguesRead = true;
                }
                // read clubs
                if (!teamsRead && file.Open(gamedbpath / strFolder / L"teams.txt")) {
                    for (FifaDataFile::Line line; file.NextLine(line); )
                        AddEntity(m_teams, line);
                    file.Close();
                    teamsRead = true;
                }
                // read managers
                if (!managersRead && file.Open(gamedbpath / strFolder / L"manager.txt")) {
                    for (FifaDataFile::Line line; file.NextLine(line); )
                        AddEntity(m_managers, line);
                    file.Close();
                    managersRead = true;
                }
                // read referees
                if (!refereesRead && file.Open(gamedbpath / strFolder / L"referee.txt")) {
                    for (FifaDataFile::Line line; file.NextLine(line); )
                        AddEntity(m_referees, line);
                    file.Close();
                    refereesRead = true;
                }
                // read stadiums
                if (!stadiumsRead && file.Open(gamedbpath / strFolder / L"stadiums.txt")) {
                    for (FifaDataFile::Line line; file.NextLine(line); )
                        AddEntity(m_stadiums, line);
                    file.Close();
                    stadiumsRead = true;
                }
                if (leagueteamlinksPath.empty() && std::filesystem::exists(gamedbpath / strFolder / L"leagueteamlinks.txt"))
                    leagueteamlinksPath = gamedbpath / strFolder / L"leagueteamlinks.txt";
                if (playerloansPath.empty() && std::filesystem::exists(gamedbpath / strFolder / L"playerloans.txt"))
                    playerloansPath = gamedbpath / strFolder / L"playerloans.txt";
                if (teamplayerlinksPath.empty() && std::filesystem::exists(gamedbpath / strFolder / L"teamplayerlinks.txt"))
                    teamplayerlinksPath = gamedbpath / strFolder / L"teamplayerlinks.txt";
                if (leaguerefereelinksPath.empty() && std::filesystem::exists(gamedbpath / strFolder / L"leaguerefereelinks.txt"))
                    leaguerefereelinksPath = gamedbpath / strFolder / L"leaguerefereelinks.txt";
                if (teamstadiumlinksPath.empty() && std::filesystem::exists(gamedbpath / strFolder / L"teamstadiumlinks.txt"))
                    teamstadiumlinksPath = gamedbpath / strFolder / L"teamstadiumlinks.txt";

                if (!latestDbIsRead)
                    latestDbIsRead = true;
            }
        }
    }
    if (!latestDbIsRead) {
        Error(L"Can't find any database in the specified path:\n'%s'", path.c_str());
        return;
    }
    // read league links
    if (!leagueteamlinksPath.empty() && file.Open(leagueteamlinksPath)) {
        for (FifaDataFile::Line line; file.NextLine(line); ) {
            int homega, previousyeartableposition, homegf, currenttableposition, awaygf, awayga, teamshortform, hasachievedobjective, yettowin, unbeatenallcomps, unbeatenleague, champion, leagueid, prevleagueid, highestpossible, teamform, highestprobable, homewins, artificialkey, nummatchesplayed, teamid, grouping, awaywins, objective, points, actualvsexpectations, homelosses, unbeatenhome, lastgameresult, unbeatenaway, awaylosses, awaydraws, homedraws, teamlongform;
            line >> homega >> previousyeartableposition >> homegf >> currenttableposition >> awaygf >> awayga >> teamshortform >> hasachievedobjective >> yettowin >> unbeatenallcomps >> unbeatenleague >> champion >> leagueid >> prevleagueid >> highestpossible >> teamform >> highestprobable >> homewins >> artificialkey >> nummatchesplayed >> teamid >> grouping >> awaywins >> objective >> points >> actualvsexpectations >> homelosses >> unbeatenhome >> lastgameresult >> unbeatenaway >> awaylosses >> awaydraws >> homedraws >> teamlongform;
            if (leagueid && teamid) {
                FifaLeague *league = GetLeague(leagueid);
                if (league) {
                    FifaTeam *leagueTeam = GetTeam(teamid);
                    if (leagueTeam) {
                        league->m_teams.push_back(leagueTeam);
                        leagueTeam->m_league = league;
                        if (prevleagueid) {
                            FifaLeague *prevLeague = GetLeague(prevleagueid);
                            if (prevLeague) {
                                leagueTeam->m_previousLeague = prevLeague;
                                if (champion == 1)
                                    prevLeague->m_champion = leagueTeam;
                            }
                        }
                    }
                }
            }
        }
        file.Close();
    }
    else
        Error(L"Unable to read database/leagueteamlinks file");
    // read links
    if (!teamplayerlinksPath.empty() && file.Open(teamplayerlinksPath)) {
        for (FifaDataFile::Line line; file.NextLine(line); ) {
            int leaguegoals, isamongtopscorers, yellows, isamongtopscorersinteam, jerseynumber, position, artificialkey, teamid, leaguegoalsprevmatch, injury, leagueappearances, istopscorer, leaguegoalsprevthreematches, playerid, form, reds;
            line >> leaguegoals >> isamongtopscorers >> yellows >> isamongtopscorersinteam >> jerseynumber >> position >> artificialkey >> teamid >> leaguegoalsprevmatch >> injury >> leagueappearances >> istopscorer >> leaguegoalsprevthreematches >> playerid >> form >> reds;
            if (teamid && playerid) {
                FifaTeam *linkedTeam = GetTeam(teamid);
                if (linkedTeam) {
                    FifaPlayer *linkedPlayer = GetPlayer(playerid);
                    if (linkedPlayer) {
                        FifaTeam::LinkedPlayer playerLink;
                        playerLink.m_player = linkedPlayer;
                        playerLink.m_position = static_cast<FifaPlayer::Position>(position);
                        playerLink.m_number = jerseynumber;
                        linkedTeam->m_linkedPlayers.push_back(playerLink);
                        FifaPlayer::LinkedTeam teamLink;
                        teamLink.m_team = linkedTeam;
                        teamLink.m_position = static_cast<FifaPlayer::Position>(position);
                        teamLink.m_number = jerseynumber;
                        linkedPlayer->m_linkedTeams.push_back(teamLink);
                    }
                }
            }
        }
        file.Close();
    }
    else
        Error(L"Unable to read database/teamplayerlinks file");
    // read loans
    if (!playerloansPath.empty() && file.Open(playerloansPath)) {
        for (FifaDataFile::Line line; file.NextLine(line); ) {
            int teamidloanedfrom, playerid, loandateend;
            line >> teamidloanedfrom >> playerid >> loandateend;
            if (teamidloanedfrom && playerid) {
                FifaTeam *loanTeam = GetTeam(teamidloanedfrom);
                if (loanTeam) {
                    FifaPlayer *loanPlayer = GetPlayer(playerid);
                    if (loanPlayer) {
                        loanPlayer->m_loanedFrom = loanTeam;
                        loanPlayer->m_loanEndDate.Set(loandateend);
                        loanTeam->m_playersOnLoan.push_back(loanPlayer);
                    }
                }
            }
        }
        file.Close();
    }
    else
        Error(L"Unable to read database/playerloans file");
    // setup managers
    ForAllManagers([&](FifaManager &manager) {
        if (manager.internal.teamid) {
            FifaTeam *managerTeam = GetTeam(manager.internal.teamid);
            if (managerTeam) {
                manager.m_team = managerTeam;
                managerTeam->m_manager = manager.RawPtr();
            }
        }
    });
    // setup referees
    ForAllReferees([&](FifaReferee &referee) {
        if (referee.internal.leagueid) {
            FifaLeague *refereeLeague = GetLeague(referee.internal.leagueid);
            if (refereeLeague)
                referee.m_mainLeague = refereeLeague;
        }
    });
    if (!leaguerefereelinksPath.empty() && file.Open(leaguerefereelinksPath)) {
        for (FifaDataFile::Line line; file.NextLine(line); ) {
            int leagueid, refereeid;
            line >> leagueid >> refereeid;
            if (leagueid && refereeid) {
                FifaLeague *league = GetLeague(leagueid);
                if (league) {
                    FifaReferee *referee = GetReferee(refereeid);
                    if (referee) {
                        league->m_referees.push_back(referee);
                        referee->m_linkedLeagues.push_back(league);
                    }
                }
            }
        }
        file.Close();
    }
    else
        Error(L"Unable to read database/leaguerefereelinks file");
    // setup stadiums
    ForAllStadiums([&](FifaStadium &stadium) {
        if (stadium.internal.hometeamid) {
            FifaTeam *stadiumTeam = GetTeam(stadium.internal.hometeamid);
            if (stadiumTeam) {
                stadium.m_homeTeam = stadiumTeam;
                stadiumTeam->m_ownStadium = stadium.RawPtr();
            }
        }
    });
    if (!teamstadiumlinksPath.empty() && file.Open(teamstadiumlinksPath)) {
        for (FifaDataFile::Line line; file.NextLine(line); ) {
            std::wstring stadiumname;
            int stadiumid, teamid, forcedhome;
            line >> stadiumname >> stadiumid >> teamid >> forcedhome;
            if (teamid) {
                FifaStadium *stadium = nullptr;
                FifaTeam *stadiumTeam = GetTeam(teamid);
                if (stadiumTeam) {
                    if (stadiumid) {
                        stadium = GetStadium(stadiumid);
                        if (stadium) {
                            FifaStadium::StadiumLink stadiumLink;
                            stadiumLink.m_name = stadiumname;
                            stadiumLink.m_team = stadiumTeam;
                            stadium->m_linkedTeams.push_back(stadiumLink);
                        }
                    }
                    stadiumTeam->m_linkedStadium.m_name = stadiumname;
                    stadiumTeam->m_linkedStadium.m_stadium = stadium;
                }
            }
        }
        file.Close();
    }
    else
        Error(L"Unable to read database/teamstadiumlinks file");
    m_currentGameVersion = m_lastSupportedGameVersion;
}

FifaDatabase::~FifaDatabase() {
    ClearList(m_leagues);
    ClearList(m_teams);
    ClearList(m_players);
    ClearList(m_managers);
    ClearList(m_referees);
    ClearList(m_stadiums);
}

std::wstring FifaDatabase::GetVersionString() {
    return m_version.GetVersionString();
}

std::wstring FifaDatabase::GetVersionDateString() {
    return m_version.GetDateString();
}

FifaDate FifaDatabase::GetVersionDate() {
    return m_version.GetDate();
}

FifaLeague *FifaDatabase::GetLeague(unsigned int leagueId) {
    return GetFromList(m_leagues, leagueId);
}

FifaTeam *FifaDatabase::GetTeam(unsigned int teamId) {
    return GetFromList(m_teams, teamId);
}

FifaPlayer *FifaDatabase::GetPlayer(unsigned int playerId) {
    return GetFromList(m_players, playerId);
}

FifaReferee *FifaDatabase::GetReferee(unsigned int refereeId) {
    return GetFromList(m_referees, refereeId);
}

FifaStadium *FifaDatabase::GetStadium(unsigned int stadiumId) {
    return GetFromList(m_stadiums, stadiumId);
}

FifaManager *FifaDatabase::GetManager(unsigned int managerId) {
    return GetFromList(m_managers, managerId);
}
