#include "FifaDatabase.h"
#include "FifaDataFile.h"
#include "Error.h"
#include "Utils.h"

FifaDatabase::FifaDatabase(std::wstring path) {
    m_path = path;
    FifaDataFile file;
    // read version
    if (!file.Open(path + L"\\version.txt")) {
        Error("%s: can't open database/version file", __FUNCTION__);
        return;
    }
    {
        FifaDataFile::Line line;
        if (file.NextLine(line))
            m_version.FromLine(line);
        else {
            Error("%s: invalid database/version file format", __FUNCTION__);
            file.Close();
            return;
        }
    }
    file.Close();
    // read leagues
    if (!file.Open(path + L"\\leagues.txt")) {
        Error("%s: can't open database/leagues file", __FUNCTION__);
        return;
    }
    for (FifaDataFile::Line line; file.NextLine(line); )
        AddEntity(m_leagues, line);
    file.Close();
    // read clubs
    if (!file.Open(path + L"\\teams.txt")) {
        Error("%s: can't open database/teams file", __FUNCTION__);
        return;
    }
    for (FifaDataFile::Line line; file.NextLine(line); )
        AddEntity(m_teams, line);
    file.Close();
    // read players
    if (!file.Open(path + L"\\players.txt")) {
        Error("%s: can't open database/players file", __FUNCTION__);
        return;
    }
    for (FifaDataFile::Line line; file.NextLine(line); )
        AddEntity(m_players, line);
    file.Close();
    // read managers
    if (!file.Open(path + L"\\manager.txt")) {
        Error("%s: can't open database/manager file", __FUNCTION__);
        return;
    }
    for (FifaDataFile::Line line; file.NextLine(line); )
        AddEntity(m_managers, line);
    file.Close();
    // read referees
    if (!file.Open(path + L"\\referee.txt")) {
        Error("%s: can't open database/referee file", __FUNCTION__);
        return;
    }
    for (FifaDataFile::Line line; file.NextLine(line); )
        AddEntity(m_referees, line);
    file.Close();
    // read stadiums
    if (!file.Open(path + L"\\stadiums.txt")) {
        Error("%s: can't open database/stadiums file", __FUNCTION__);
        return;
    }
    for (FifaDataFile::Line line; file.NextLine(line); )
        AddEntity(m_stadiums, line);
    file.Close();
    // read names
    std::map<unsigned int, std::wstring> names;
    std::wstring name;
    int nameid, commentaryid;
    if (!file.Open(path + L"\\playernames.txt")) {
        Error("%s: can't open database/playernames file", __FUNCTION__);
        return;
    }
    for (FifaDataFile::Line line; file.NextLine(line); ) {
        line >> name >> nameid >> commentaryid;
        names[nameid] = name;
    }
    file.Close();
    // read additional names
    if (file.Open(path + L"\\dcplayernames.txt")) {
        for (FifaDataFile::Line line; file.NextLine(line); ) {
            line >> name >> nameid;
            names[nameid] = name;
        }
        file.Close();
    }
    // setup player names
    ForAllPlayers([&](FifaPlayer &player) {
        player.m_firstName = names[player.internal.firstnameid];
        player.m_lastName = names[player.internal.lastnameid];
        player.m_commonName = names[player.internal.commonnameid];
        player.m_playerJerseyName = names[player.internal.playerjerseynameid];
        player.m_quickName = Utils::GetQuickName(player.m_firstName, player.m_lastName, player.m_commonName);
    });
    names.clear();
    // read league links
    if (!file.Open(path + L"\\leagueteamlinks.txt")) {
        Error("%s: can't open database/leagueteamlinks file", __FUNCTION__);
        return;
    }
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
    // read links
    if (!file.Open(path + L"\\teamplayerlinks.txt")) {
        Error("%s: can't open database/teamplayerlinks file", __FUNCTION__);
        return;
    }
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
    // read loans
    if (!file.Open(path + L"\\playerloans.txt")) {
        Error("%s: can't open database/playerloans file", __FUNCTION__);
        return;
    }
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
    if (!file.Open(path + L"\\leaguerefereelinks.txt")) {
        Error("%s: can't open database/leaguerefereelinks file", __FUNCTION__);
        return;
    }
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
    if (!file.Open(path + L"\\teamstadiumlinks.txt")) {
        Error("%s: can't open database/teamstadiumlinks file", __FUNCTION__);
        return;
    }
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