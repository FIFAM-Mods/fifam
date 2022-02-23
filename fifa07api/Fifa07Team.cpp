#include "Fifa07Team.h"
#include "Fifa07Database.h"
#include <sstream>
#include <string>

unsigned int Fifa07Team::GetId() {
    return teamid;
}

void Fifa07Team::Parse(Fifa07DataFile::Line &line) {
    switch (Fifa07Database::m_currentGameVersion) {
    case 7:
        line >> teamid >> teamname >> generickitid >> balltype >> adboardid >> stadiumid >> genericbanner >> trainingstadiumid >> speechcountryid >> opponent >> assetid >> teamchemistry >> overallrating >> signsyouthplayersfrom >> signsplayersfromprimary >> signsplayersfromsecondary >> initialtransferbudget >> internationalprestige >> domesticprestige >> homecrowdsize >> teamprimarycolor >> teamsecondarycolor; 
        break;
    case 8:
        line >> teamid >> teamname >> generickitid >> balltype >> adboardid >> stadiumid >> genericbanner >> trainingstadiumid >> speechcountryid >> opponent >> assetid >> teamchemistry >> overallrating >> signsyouthplayersfrom >> signsplayersfromprimary >> signsplayersfromsecondary >> initialtransferbudget >> internationalprestige >> domesticprestige >> homecrowdsize >> teamprimarycolor >> teamsecondarycolor >> defaultformationid;
        break;
    }
}

void Fifa07Team::ParseTeamWrite(Fifa07DataFile::Line &line) {
    line >> teamid >> offsidetrap >> withoutball >> formationid >> attack >> teammentality >> attacktactic1 >> attacktactic2 >> defensetactic1 >> defensetactic2 >> captain >> penaltytakerid >> kicktakerid >> leftcornerkicktakerid >> rightcornerkicktakerid >> numtransfersin;
}

Fifa07Team::Fifa07Team(Fifa07DataFile::Line &line) {
    Parse(line);
}
