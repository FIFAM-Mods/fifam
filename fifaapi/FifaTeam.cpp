#include "FifaTeam.h"
#include "FifaDatabase.h"
#include <sstream>
#include <string>

unsigned int FifaTeam::GetId() {
    return internal.teamid;
}

void FifaTeam::Internal::Read(FifaDataFile::Line &line) {
    switch (FifaDatabase::m_currentGameVersion) {
    case 18:
        line >> assetid >> ballid >> teamcolor1g >> teamcolor1r >> clubworth >> teamcolor2b >> teamcolor2r >> foundationyear >> teamcolor3r >> teamcolor1b >> opponentweakthreshold >> latitude >> teamcolor3g >> opponentstrongthreshold >> teamcolor2g >> teamname >> adboardid >> teamcolor3b >> defmentality >> powid >> rightfreekicktakerid >> domesticprestige >> genericint2 >> jerseytype >> popularity >> teamstadiumcapacity >> iscompetitionscarfenabled >> cityid >> rivalteam >> isbannerenabled >> midfieldrating >> matchdayoverallrating >> matchdaymidfieldrating >> attackrating >> longitude >> buspassing >> matchdaydefenserating >> defenserating >> iscompetitionpoleflagenabled >> skinnyflags >> defteamwidth >> longkicktakerid >> bodytypeid >> trait1vweak >> busdribbling >> rightcornerkicktakerid >> suitvariationid >> domesticcups >> defaggression >> ethnicity >> leftcornerkicktakerid >> youthdevelopment >> teamid >> trait1vequal >> suittypeid >> numtransfersin >> stanchionflamethrower >> captainid >> personalityid >> leftfreekicktakerid >> leaguetitles >> genericbanner >> buspositioning >> ccpositioning >> busbuildupspeed >> transferbudget >> ccshooting >> overallrating >> ccpassing >> profitability >> utcoffset >> penaltytakerid >> freekicktakerid >> crowdskintonecode >> defdefenderline >> internationalprestige >> trainingstadium >> form >> genericint1 >> cccrossing >> trait1vstrong >> matchdayattackrating;
        break;
    case 19:
        line >> assetid >> ballid >> teamcolor1g >> teamcolor1r >> clubworth >> teamcolor2b >> teamcolor2r >> foundationyear >> teamcolor3r >> teamcolor1b >> opponentweakthreshold >> latitude >> teamcolor3g >> opponentstrongthreshold >> teamcolor2g >> teamname >> teamcolor3b >> defmentality >> powid >> defensivestyle >> rightfreekicktakerid >> flamethrowercannon >> domesticprestige >> genericint2 >> defensivedepth >> hasvikingclap >> jerseytype >> popularity >> hastifo >> teamstadiumcapacity >> iscompetitionscarfenabled >> cityid >> defensivewidth >> rivalteam >> isbannerenabled >> midfieldrating >> matchdayoverallrating >> matchdaymidfieldrating >> attackrating >> playersinboxcorner >> longitude >> buspassing >> matchdaydefenserating >> defenserating >> iscompetitionpoleflagenabled >> skinnyflags >> defteamwidth >> longkicktakerid >> bodytypeid >> trait1vweak >> busdribbling >> rightcornerkicktakerid >> suitvariationid >> uefa_cl_wins >> domesticcups >> defaggression >> ethnicity >> leftcornerkicktakerid >> youthdevelopment >> teamid >> uefa_el_wins >> trait1vequal >> suittypeid >> numtransfersin >> playersinboxfk >> stanchionflamethrower >> captainid >> offensivestyle >> personalityid >> playersinboxcross >> prev_el_champ >> leftfreekicktakerid >> leaguetitles >> genericbanner >> buspositioning >> ccpositioning >> busbuildupspeed >> transferbudget >> ccshooting >> overallrating >> offensivewidth >> ccpassing >> profitability >> utcoffset >> penaltytakerid >> freekicktakerid >> crowdskintonecode >> defdefenderline >> internationalprestige >> trainingstadium >> form >> genericint1 >> cccrossing >> trait1vstrong >> matchdayattackrating;
        break;
    case 20:
        line >> assetid >> teamcolor1g >> teamcolor1r >> clubworth >> teamcolor2b >> teamcolor2r >> foundationyear >> teamcolor3r >> teamcolor1b >> opponentweakthreshold >> latitude >> teamcolor3g >> opponentstrongthreshold >> ballid >> teamcolor2g >> teamname >> teamcolor3b >> powid >> defensivestyle >> rightfreekicktakerid >> flamethrowercannon >> domesticprestige >> genericint2 >> defensivedepth >> hasvikingclap >> jerseytype >> popularity >> hastifo >> teamstadiumcapacity >> iscompetitionscarfenabled >> cityid >> defensivewidth >> rivalteam >> isbannerenabled >> midfieldrating >> matchdayoverallrating >> matchdaymidfieldrating >> attackrating >> playersinboxcorner >> longitude >> matchdaydefenserating >> hasstandingcrowd >> favoriteteamsheetid >> defenserating >> iscompetitionpoleflagenabled >> skinnyflags >> uefa_consecutive_wins >> longkicktakerid >> bodytypeid >> trait1vweak >> iscompetitioncrowdcardsenabled >> rightcornerkicktakerid >> suitvariationid >> uefa_cl_wins >> domesticcups >> ethnicity >> leftcornerkicktakerid >> youthdevelopment >> teamid >> uefa_el_wins >> trait1vequal >> suittypeid >> numtransfersin >> playersinboxfk >> stanchionflamethrower >> captainid >> offensivestyle >> personalityid >> playersinboxcross >> prev_el_champ >> leftfreekicktakerid >> leaguetitles >> genericbanner >> transferbudget >> overallrating >> offensivewidth >> profitability >> utcoffset >> penaltytakerid >> freekicktakerid >> crowdskintonecode >> internationalprestige >> trainingstadium >> form >> genericint1 >> trait1vstrong >> matchdayattackrating;
        break;
    }
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
