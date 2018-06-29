#include "FifaTeam.h"
#include <sstream>
#include <string>

unsigned int FifaTeam::GetId() {
    return internal.teamid;
}

void FifaTeam::Internal::Read(FifaDataFile::Line &line) {
    line >> assetid >> balltype >> teamcolor1g >> teamcolor1r >> clubworth >> teamcolor2b >> teamcolor2r >> foundationyear >> teamcolor3r >> teamcolor1b >> opponentweakthreshold >> latitude >> teamcolor3g >> opponentstrongthreshold >> teamcolor2g >> teamname >> adboardid >> teamcolor3b >> defmentality >> powid >> rightfreekicktakerid >> domesticprestige >> genericint2 >> jerseytype >> popularity >> teamstadiumcapacity >> iscompetitionscarfenabled >> cityid >> rivalteam >> isbannerenabled >> midfieldrating >> matchdayoverallrating >> matchdaymidfieldrating >> attackrating >> longitude >> buspassing >> matchdaydefenserating >> defenserating >> iscompetitionpoleflagenabled >> skinnyflags >> defteamwidth >> longkicktakerid >> bodytypeid >> trait1vweak >> busdribbling >> rightcornerkicktakerid >> suitvariationid >> domesticcups >> defaggression >> ethnicity >> leftcornerkicktakerid >> youthdevelopment >> teamid >> trait1vequal >> suittypeid >> numtransfersin >> stanchionflamethrower >> captainid >> personalityid >> leftfreekicktakerid >> leaguetitles >> genericbanner >> buspositioning >> ccpositioning >> busbuildupspeed >> transferbudget >> ccshooting >> overallrating >> ccpassing >> profitability >> utcoffset >> penaltytakerid >> freekicktakerid >> crowdskintonecode >> defdefenderline >> internationalprestige >> trainingstadium >> form >> genericint1 >> cccrossing >> trait1vstrong >> matchdayattackrating;
}

FifaTeam::FifaTeam(FifaDataFile::Line &line) {
    internal.Read(line);
    m_name = internal.teamname;
    m_manager = nullptr;
    m_ownStadium = nullptr;
    m_linkedStadium.m_stadium = nullptr;
    m_league = nullptr;
    m_previousLeague = nullptr;
}