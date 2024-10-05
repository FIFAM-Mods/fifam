#include "FifaTeam.h"
#include "FifaDatabase.h"
#include <sstream>
#include <string>

unsigned int FifaTeam::GetId() {
    return internal.teamid;
}

void FifaTeam::Internal::Read(FifaDataFile::Line &line) {
    switch (FifaDatabase::m_currentGameVersion) {
    case 11:
        line >> balltype >> teamcolor1g >> teamcolor1r >> teamcolor2b >> teamcolor2r >> teamcolor3r >> teamcolor1b >> teamcolor3g >> teamcolor2g >> teamname >> adboardid >> teamcolor3b >> defmentality >> managerid >> physioid_secondary >> domesticprestige >> jerseytype >> rivalteam >> assetid >> physioid_primary >> buspassing >> defteamwidth >> longkicktakerid >> rightcornerkicktakerid >> defaggression >> leftcornerkicktakerid >> teamid >> fancrowdhairskintexturecode >> numtransfersin >> captainid >> genericbanner >> buspositioning >> stafftracksuitcolorcode >> ccpositioning >> busbuildupspeed >> transferbudget >> ccshooting >> ccpassing >> penaltytakerid >> freekicktakerid >> defdefenderline >> internationalprestige >> form >> cccrossing;
        break;
    case 12:
        line >> assetid >> balltype >> teamcolor1g >> teamcolor1r >> teamcolor2b >> teamcolor2r >> teamcolor3r >> teamcolor1b >> teamcolor3g >> teamcolor2g >> teamname >> adboardid >> teamcolor3b >> defmentality >> physioid_secondary >> domesticprestige >> genericint2 >> jerseytype >> rivalteam >> physioid_primary >> buspassing >> defteamwidth >> longkicktakerid >> rightcornerkicktakerid >> defaggression >> leftcornerkicktakerid >> teamid >> fancrowdhairskintexturecode >> numtransfersin >> captainid >> genericbanner >> buspositioning >> stafftracksuitcolorcode >> ccpositioning >> busbuildupspeed >> transferbudget >> ccshooting >> ccpassing >> penaltytakerid >> freekicktakerid >> defdefenderline >> internationalprestige >> form >> genericint1 >> cccrossing;
        break;
    case 13:
        line >> assetid >> balltype >> teamcolor1g >> teamcolor1r >> teamcolor2b >> teamcolor2r >> teamcolor3r >> teamcolor1b >> latitude >> teamcolor3g >> teamcolor2g >> teamname >> adboardid >> teamcolor3b >> defmentality >> powid >> physioid_secondary >> domesticprestige >> genericint2 >> jerseytype >> rivalteam >> midfieldrating >> matchdayoverallrating >> matchdaymidfieldrating >> attackrating >> physioid_primary >> longitude >> buspassing >> matchdaydefenserating >> defenserating >> defteamwidth >> longkicktakerid >> bodytypeid >> rightcornerkicktakerid >> suitvariationid >> defaggression >> ethnicity >> leftcornerkicktakerid >> teamid >> fancrowdhairskintexturecode >> suittypeid >> numtransfersin >> captainid >> personalityid >> genericbanner >> buspositioning >> stafftracksuitcolorcode >> ccpositioning >> busbuildupspeed >> transferbudget >> ccshooting >> overallrating >> ccpassing >> utcoffset >> penaltytakerid >> freekicktakerid >> defdefenderline >> internationalprestige >> form >> genericint1 >> cccrossing >> matchdayattackrating;
        break;
    case 14:
        line >> assetid >> balltype >> teamcolor1g >> teamcolor1r >> teamcolor2b >> teamcolor2r >> teamcolor3r >> teamcolor1b >> latitude >> teamcolor3g >> teamcolor2g >> teamname >> adboardid >> teamcolor3b >> defmentality >> powid >> physioid_secondary >> domesticprestige >> genericint2 >> jerseytype >> rivalteam >> midfieldrating >> matchdayoverallrating >> matchdaymidfieldrating >> attackrating >> physioid_primary >> longitude >> buspassing >> matchdaydefenserating >> defenserating >> defteamwidth >> longkicktakerid >> bodytypeid >> trait1 >> busdribbling >> rightcornerkicktakerid >> suitvariationid >> defaggression >> ethnicity >> leftcornerkicktakerid >> teamid >> fancrowdhairskintexturecode >> suittypeid >> numtransfersin >> captainid >> personalityid >> genericbanner >> buspositioning >> stafftracksuitcolorcode >> ccpositioning >> busbuildupspeed >> transferbudget >> ccshooting >> overallrating >> ccpassing >> utcoffset >> penaltytakerid >> freekicktakerid >> defdefenderline >> internationalprestige >> form >> genericint1 >> cccrossing >> matchdayattackrating;
        break;
    case 15:
        line >> assetid >> balltype >> teamcolor1g >> teamcolor1r >> teamcolor2b >> teamcolor2r >> teamcolor3r >> teamcolor1b >> latitude >> teamcolor3g >> teamcolor2g >> teamname >> adboardid >> teamcolor3b >> defmentality >> powid >> rightfreekicktakerid >> physioid_secondary >> domesticprestige >> genericint2 >> jerseytype >> rivalteam >> midfieldrating >> matchdayoverallrating >> matchdaymidfieldrating >> attackrating >> physioid_primary >> longitude >> buspassing >> matchdaydefenserating >> defenserating >> defteamwidth >> longkicktakerid >> bodytypeid >> trait1 >> busdribbling >> rightcornerkicktakerid >> suitvariationid >> defaggression >> ethnicity >> leftcornerkicktakerid >> teamid >> fancrowdhairskintexturecode >> suittypeid >> numtransfersin >> captainid >> personalityid >> leftfreekicktakerid >> genericbanner >> buspositioning >> stafftracksuitcolorcode >> ccpositioning >> busbuildupspeed >> transferbudget >> ccshooting >> overallrating >> ccpassing >> utcoffset >> penaltytakerid >> freekicktakerid >> defdefenderline >> internationalprestige >> form >> genericint1 >> cccrossing >> matchdayattackrating;
        break;
    case 16:
        line >> assetid >> balltype >> teamcolor1g >> teamcolor1r >> teamcolor2b >> teamcolor2r >> teamcolor3r >> teamcolor1b >> latitude >> teamcolor3g >> teamcolor2g >> teamname >> adboardid >> teamcolor3b >> defmentality >> powid >> rightfreekicktakerid >> physioid_secondary >> domesticprestige >> genericint2 >> jerseytype >> rivalteam >> midfieldrating >> matchdayoverallrating >> matchdaymidfieldrating >> attackrating >> physioid_primary >> longitude >> buspassing >> matchdaydefenserating >> defenserating >> defteamwidth >> longkicktakerid >> bodytypeid >> trait1 >> busdribbling >> rightcornerkicktakerid >> suitvariationid >> defaggression >> ethnicity >> leftcornerkicktakerid >> teamid >> fancrowdhairskintexturecode >> suittypeid >> numtransfersin >> captainid >> personalityid >> leftfreekicktakerid >> genericbanner >> buspositioning >> stafftracksuitcolorcode >> ccpositioning >> busbuildupspeed >> transferbudget >> ccshooting >> overallrating >> ccpassing >> utcoffset >> penaltytakerid >> freekicktakerid >> defdefenderline >> internationalprestige >> form >> genericint1 >> cccrossing >> matchdayattackrating;
        break;
    case 17:
        line >> assetid >> balltype >> teamcolor1g >> teamcolor1r >> clubworth >> teamcolor2b >> teamcolor2r >> foundationyear >> teamcolor3r >> teamcolor1b >> opponentweakthreshold >> latitude >> teamcolor3g >> opponentstrongthreshold >> teamcolor2g >> teamname >> adboardid >> teamcolor3b >> busbuildupspeedvequal >> powid >> rightfreekicktakerid >> busbuildupspeedvweak >> defmentalityvsweak >> physioid_secondary >> domesticprestige >> genericint2 >> jerseytype >> buspassingvequal >> defmentalityvstrong >> popularity >> teamstadiumcapacity >> cityid >> rivalteam >> midfieldrating >> matchdayoverallrating >> matchdaymidfieldrating >> attackrating >> physioid_primary >> longitude >> matchdaydefenserating >> busbuildupspeedvstrong >> defenserating >> defteamwidth >> longkicktakerid >> bodytypeid >> trait1 >> defmentalityvequal >> busdribbling >> rightcornerkicktakerid >> suitvariationid >> domesticcups >> defaggression >> ethnicity >> leftcornerkicktakerid >> youthdevelopment >> teamid >> fancrowdhairskintexturecode >> suittypeid >> buspassingvstrong >> numtransfersin >> captainid >> personalityid >> leftfreekicktakerid >> leaguetitles >> genericbanner >> buspositioning >> stafftracksuitcolorcode >> ccpositioning >> transferbudget >> ccshooting >> overallrating >> ccpassing >> profitability >> utcoffset >> penaltytakerid >> buspassingvweak >> freekicktakerid >> defdefenderline >> internationalprestige >> trainingstadium >> form >> genericint1 >> cccrossing >> matchdayattackrating;
        break;
    case 18:
        line >> assetid >> ballid >> teamcolor1g >> teamcolor1r >> clubworth >> teamcolor2b >> teamcolor2r >> foundationyear >> teamcolor3r >> teamcolor1b >> opponentweakthreshold >> latitude >> teamcolor3g >> opponentstrongthreshold >> teamcolor2g >> teamname >> adboardid >> teamcolor3b >> defmentality >> powid >> rightfreekicktakerid >> domesticprestige >> genericint2 >> jerseytype >> popularity >> teamstadiumcapacity >> iscompetitionscarfenabled >> cityid >> rivalteam >> isbannerenabled >> midfieldrating >> matchdayoverallrating >> matchdaymidfieldrating >> attackrating >> longitude >> buspassing >> matchdaydefenserating >> defenserating >> iscompetitionpoleflagenabled >> skinnyflags >> defteamwidth >> longkicktakerid >> bodytypeid >> trait1vweak >> busdribbling >> rightcornerkicktakerid >> suitvariationid >> domesticcups >> defaggression >> ethnicity >> leftcornerkicktakerid >> youthdevelopment >> teamid >> trait1vequal >> suittypeid >> numtransfersin >> stanchionflamethrower >> captainid >> personalityid >> leftfreekicktakerid >> leaguetitles >> genericbanner >> buspositioning >> ccpositioning >> busbuildupspeed >> transferbudget >> ccshooting >> overallrating >> ccpassing >> profitability >> utcoffset >> penaltytakerid >> freekicktakerid >> crowdskintonecode >> defdefenderline >> internationalprestige >> trainingstadium >> form >> genericint1 >> cccrossing >> trait1vstrong >> matchdayattackrating;
        break;
    case 19:
        line >> assetid >> ballid >> teamcolor1g >> teamcolor1r >> clubworth >> teamcolor2b >> teamcolor2r >> foundationyear >> teamcolor3r >> teamcolor1b >> opponentweakthreshold >> latitude >> teamcolor3g >> opponentstrongthreshold >> teamcolor2g >> teamname >> teamcolor3b >> defmentality >> powid >> defensivestyle >> rightfreekicktakerid >> flamethrowercannon >> domesticprestige >> genericint2 >> defensivedepth >> hasvikingclap >> jerseytype >> popularity >> hastifo >> teamstadiumcapacity >> iscompetitionscarfenabled >> cityid >> defensivewidth >> rivalteam >> isbannerenabled >> midfieldrating >> matchdayoverallrating >> matchdaymidfieldrating >> attackrating >> playersinboxcorner >> longitude >> buspassing >> matchdaydefenserating >> defenserating >> iscompetitionpoleflagenabled >> skinnyflags >> defteamwidth >> longkicktakerid >> bodytypeid >> trait1vweak >> busdribbling >> rightcornerkicktakerid >> suitvariationid >> uefa_cl_wins >> domesticcups >> defaggression >> ethnicity >> leftcornerkicktakerid >> youthdevelopment >> teamid >> uefa_el_wins >> trait1vequal >> suittypeid >> numtransfersin >> playersinboxfk >> stanchionflamethrower >> captainid >> offensivestyle >> personalityid >> playersinboxcross >> prev_el_champ >> leftfreekicktakerid >> leaguetitles >> genericbanner >> buspositioning >> ccpositioning >> busbuildupspeed >> transferbudget >> ccshooting >> overallrating >> offensivewidth >> ccpassing >> profitability >> utcoffset >> penaltytakerid >> freekicktakerid >> crowdskintonecode >> defdefenderline >> internationalprestige >> trainingstadium >> form >> genericint1 >> cccrossing >> trait1vstrong >> matchdayattackrating;
        break;
    case 20:
        line >> assetid >> teamcolor1g >> teamcolor1r >> clubworth >> teamcolor2b >> teamcolor2r >> foundationyear >> teamcolor3r >> teamcolor1b >> opponentweakthreshold >> latitude >> teamcolor3g >> opponentstrongthreshold >> ballid >> teamcolor2g >> teamname >> teamcolor3b >> powid >> defensivestyle >> rightfreekicktakerid >> flamethrowercannon >> domesticprestige >> genericint2 >> defensivedepth >> hasvikingclap >> jerseytype >> popularity >> hastifo >> teamstadiumcapacity >> iscompetitionscarfenabled >> cityid >> defensivewidth >> rivalteam >> isbannerenabled >> midfieldrating >> matchdayoverallrating >> matchdaymidfieldrating >> attackrating >> playersinboxcorner >> longitude >> matchdaydefenserating >> hasstandingcrowd >> favoriteteamsheetid >> defenserating >> iscompetitionpoleflagenabled >> skinnyflags >> uefa_consecutive_wins >> longkicktakerid >> bodytypeid >> trait1vweak >> iscompetitioncrowdcardsenabled >> rightcornerkicktakerid >> suitvariationid >> uefa_cl_wins >> domesticcups >> ethnicity >> leftcornerkicktakerid >> youthdevelopment >> teamid >> uefa_el_wins >> trait1vequal >> suittypeid >> numtransfersin >> playersinboxfk >> stanchionflamethrower >> captainid >> offensivestyle >> personalityid >> playersinboxcross >> prev_el_champ >> leftfreekicktakerid >> leaguetitles >> genericbanner >> transferbudget >> overallrating >> offensivewidth >> profitability >> utcoffset >> penaltytakerid >> freekicktakerid >> crowdskintonecode >> internationalprestige >> trainingstadium >> form >> genericint1 >> trait1vstrong >> matchdayattackrating;
        break;
    case 21:
        line >> assetid >> teamcolor1g >> teamcolor1r >> clubworth >> teamcolor2b >> goalnetstanchioncolor2g >> teamcolor2r >> foundationyear >> goalnetstanchioncolor2r >> teamcolor3r >> goalnetstanchioncolor1b >> teamcolor1b >> opponentweakthreshold >> latitude >> teamcolor3g >> opponentstrongthreshold >> goalnetstanchioncolor2b >> goalnetstanchioncolor1r >> ballid >> teamcolor2g >> goalnetstanchioncolor1g >> teamname >> teamcolor3b >> presassetone >> powid >> defensivestyle >> rightfreekicktakerid >> flamethrowercannon >> domesticprestige >> genericint2 >> defensivedepth >> hasvikingclap >> jerseytype >> pitchcolor >> pitchwear >> popularity >> hastifo >> presassettwo >> teamstadiumcapacity >> stadiumgoalnetstyle >> iscompetitionscarfenabled >> cityid >> defensivewidth >> rivalteam >> playsurfacetype >> isbannerenabled >> midfieldrating >> stadiummowpattern_code >> matchdayoverallrating >> matchdaymidfieldrating >> attackrating >> playersinboxcorner >> longitude >> matchdaydefenserating >> hasstandingcrowd >> favoriteteamsheetid >> defenserating >> iscompetitionpoleflagenabled >> skinnyflags >> uefa_consecutive_wins >> longkicktakerid >> bodytypeid >> trait1vweak >> iscompetitioncrowdcardsenabled >> rightcornerkicktakerid >> suitvariationid >> uefa_cl_wins >> domesticcups >> ethnicity >> leftcornerkicktakerid >> youthdevelopment >> teamid >> uefa_el_wins >> trait1vequal >> suittypeid >> numtransfersin >> playersinboxfk >> stanchionflamethrower >> stadiumgoalnetpattern >> captainid >> offensivestyle >> personalityid >> playersinboxcross >> prev_el_champ >> leftfreekicktakerid >> leaguetitles >> genericbanner >> transferbudget >> overallrating >> offensivewidth >> profitability >> utcoffset >> penaltytakerid >> pitchlinecolor >> freekicktakerid >> crowdskintonecode >> internationalprestige >> trainingstadium >> form >> genericint1 >> trait1vstrong >> matchdayattackrating;
        break;
    case 22:
        line >> assetid >> teamcolor1g >> teamcolor1r >> clubworth >> teamcolor2b >> goalnetstanchioncolor2g >> teamcolor2r >> foundationyear >> goalnetstanchioncolor2r >> teamcolor3r >> goalnetstanchioncolor1b >> teamcolor1b >> opponentweakthreshold >> latitude >> teamcolor3g >> opponentstrongthreshold >> goalnetstanchioncolor2b >> goalnetstanchioncolor1r >> teamcolor2g >> goalnetstanchioncolor1g >> teamname >> teamcolor3b >> presassetone >> powid >> defensivestyle >> hassubstitutionboard >> rightfreekicktakerid >> flamethrowercannon >> domesticprestige >> genericint2 >> defensivedepth >> hasvikingclap >> jerseytype >> chancecreation >> pitchcolor >> pitchwear >> popularity >> hastifo >> presassettwo >> teamstadiumcapacity >> stadiumgoalnetstyle >> iscompetitionscarfenabled >> cityid >> defensivewidth >> rivalteam >> playsurfacetype >> isbannerenabled >> midfieldrating >> stadiummowpattern_code >> matchdayoverallrating >> matchdaymidfieldrating >> attackrating >> playersinboxcorner >> longitude >> buildupplay >> matchdaydefenserating >> hasstandingcrowd >> favoriteteamsheetid >> defenserating >> iscompetitionpoleflagenabled >> skinnyflags >> uefa_consecutive_wins >> longkicktakerid >> trait1vweak >> iscompetitioncrowdcardsenabled >> rightcornerkicktakerid >> uefa_cl_wins >> domesticcups >> ethnicity >> leftcornerkicktakerid >> youthdevelopment >> teamid >> uefa_el_wins >> trait1vequal >> numtransfersin >> playersinboxfk >> stanchionflamethrower >> stadiumgoalnetpattern >> captainid >> offensivestyle >> personalityid >> playersinboxcross >> prev_el_champ >> leftfreekicktakerid >> leaguetitles >> genericbanner >> crowdregion >> transferbudget >> uefa_uecl_wins >> overallrating >> ballid >> offensivewidth >> profitability >> utcoffset >> penaltytakerid >> pitchlinecolor >> freekicktakerid >> crowdskintonecode >> internationalprestige >> haslargeflag >> trainingstadium >> form >> genericint1 >> trait1vstrong >> matchdayattackrating;
        break;
    case 23:
        line >> assetid >> teamcolor1g >> teamcolor1r >> clubworth >> teamcolor2b >> goalnetstanchioncolor2g >> teamcolor2r >> foundationyear >> goalnetstanchioncolor2r >> teamcolor3r >> goalnetstanchioncolor1b >> teamcolor1b >> opponentweakthreshold >> latitude >> teamcolor3g >> opponentstrongthreshold >> goalnetstanchioncolor2b >> goalnetstanchioncolor1r >> teamcolor2g >> goalnetstanchioncolor1g >> teamname >> teamcolor3b >> presassetone >> powid >> defensivestyle >> hassubstitutionboard >> rightfreekicktakerid >> flamethrowercannon >> domesticprestige >> genericint2 >> defensivedepth >> hasvikingclap >> jerseytype >> chancecreation >> pitchcolor >> pitchwear >> popularity >> hastifo >> presassettwo >> teamstadiumcapacity >> stadiumgoalnetstyle >> iscompetitionscarfenabled >> cityid >> defensivewidth >> rivalteam >> playsurfacetype >> isbannerenabled >> midfieldrating >> stadiummowpattern_code >> matchdayoverallrating >> matchdaymidfieldrating >> attackrating >> playersinboxcorner >> longitude >> buildupplay >> matchdaydefenserating >> hasstandingcrowd >> favoriteteamsheetid >> defenserating >> iscompetitionpoleflagenabled >> skinnyflags >> uefa_consecutive_wins >> longkicktakerid >> trait1vweak >> iscompetitioncrowdcardsenabled >> rightcornerkicktakerid >> uefa_cl_wins >> domesticcups >> ethnicity >> leftcornerkicktakerid >> youthdevelopment >> teamid >> uefa_el_wins >> trait1vequal >> numtransfersin >> playersinboxfk >> stanchionflamethrower >> stadiumgoalnetpattern >> captainid >> offensivestyle >> personalityid >> playersinboxcross >> prev_el_champ >> leftfreekicktakerid >> leaguetitles >> genericbanner >> crowdregion >> uefa_uecl_wins >> overallrating >> ballid >> offensivewidth >> profitability >> utcoffset >> penaltytakerid >> pitchlinecolor >> freekicktakerid >> crowdskintonecode >> internationalprestige >> haslargeflag >> trainingstadium >> form >> genericint1 >> trait1vstrong >> matchdayattackrating;
        break;
    case 24:
        line >> assetid >> teamcolor1g >> teamcolor1r >> clubworth >> teamcolor2b >> goalnetstanchioncolor2g >> teamcolor2r >> foundationyear >> goalnetstanchioncolor2r >> teamcolor3r >> goalnetstanchioncolor1b >> teamcolor1b >> opponentweakthreshold >> latitude >> teamcolor3g >> opponentstrongthreshold >> goalnetstanchioncolor2b >> goalnetstanchioncolor1r >> teamcolor2g >> goalnetstanchioncolor1g >> teamname >> teamcolor3b >> presassetone >> powid >> defensivestyle >> hassubstitutionboard >> rightfreekicktakerid >> flamethrowercannon >> domesticprestige >> genericint2 >> defensivedepth >> hasvikingclap >> jerseytype >> chancecreation >> pitchcolor >> pitchwear >> popularity >> hastifo >> presassettwo >> teamstadiumcapacity >> stadiumgoalnetstyle >> iscompetitionscarfenabled >> cityid >> defensivewidth >> rivalteam >> playsurfacetype >> isbannerenabled >> midfieldrating >> stadiummowpattern_code >> matchdayoverallrating >> matchdaymidfieldrating >> attackrating >> playersinboxcorner >> longitude >> buildupplay >> matchdaydefenserating >> hasstandingcrowd >> favoriteteamsheetid >> defenserating >> iscompetitionpoleflagenabled >> skinnyflags >> uefa_consecutive_wins >> longkicktakerid >> trait1vweak >> iscompetitioncrowdcardsenabled >> rightcornerkicktakerid >> uefa_cl_wins >> hassuncanthem >> domesticcups >> ethnicity >> leftcornerkicktakerid >> youthdevelopment >> teamid >> uefa_el_wins >> trait1vequal >> numtransfersin >> playersinboxfk >> stanchionflamethrower >> stadiumgoalnetpattern >> captainid >> offensivestyle >> personalityid >> playersinboxcross >> prev_el_champ >> leftfreekicktakerid >> leaguetitles >> genericbanner >> crowdregion >> uefa_uecl_wins >> overallrating >> ballid >> offensivewidth >> profitability >> utcoffset >> penaltytakerid >> pitchlinecolor >> freekicktakerid >> crowdskintonecode >> internationalprestige >> haslargeflag >> trainingstadium >> form >> genericint1 >> trait1vstrong >> matchdayattackrating;
        break;
    case 25:
        line >> assetid >> teamcolor1g >> teamcolor1r >> clubworth >> teamcolor2b >> goalnetstanchioncolor2g >> teamcolor2r >> foundationyear >> goalnetstanchioncolor2r >> teamcolor3r >> goalnetstanchioncolor1b >> teamcolor1b >> opponentweakthreshold >> latitude >> teamcolor3g >> opponentstrongthreshold >> goalnetstanchioncolor2b >> goalnetstanchioncolor1r >> teamcolor2g >> goalnetstanchioncolor1g >> teamname >> teamcolor3b >> presassetone >> powid >> hassubstitutionboard >> rightfreekicktakerid >> flamethrowercannon >> domesticprestige >> genericint2 >> defensivedepth >> hasvikingclap >> jerseytype >> pitchcolor >> pitchwear >> popularity >> hastifo >> presassettwo >> teamstadiumcapacity >> stadiumgoalnetstyle >> iscompetitionscarfenabled >> cityid >> rivalteam >> playsurfacetype >> isbannerenabled >> midfieldrating >> stadiummowpattern_code >> matchdayoverallrating >> matchdaymidfieldrating >> attackrating >> longitude >> buildupplay >> matchdaydefenserating >> hasstandingcrowd >> favoriteteamsheetid >> defenserating >> iscompetitionpoleflagenabled >> skinnyflags >> uefa_consecutive_wins >> longkicktakerid >> trait1vweak >> iscompetitioncrowdcardsenabled >> rightcornerkicktakerid >> gender >> cksupport1 >> uefa_cl_wins >> hassuncanthem >> domesticcups >> ethnicity >> leftcornerkicktakerid >> youthdevelopment >> teamid >> uefa_el_wins >> trait1vequal >> numtransfersin >> stanchionflamethrower >> stadiumgoalnetpattern >> captainid >> personalityid >> prev_el_champ >> leftfreekicktakerid >> cksupport2 >> leaguetitles >> genericbanner >> crowdregion >> uefa_uecl_wins >> overallrating >> ballid >> profitability >> utcoffset >> penaltytakerid >> pitchlinecolor >> freekicktakerid >> crowdskintonecode >> internationalprestige >> cksupport3 >> haslargeflag >> trainingstadium >> form >> genericint1 >> trait1vstrong >> matchdayattackrating;
        break;
    }
}

FifaTeam::FifaTeam(FifaDataFile::Line &line) {
    internal.Read(line);
    m_gameId = FifaDatabase::m_currentGameVersion;
    m_name = internal.teamname;
    m_manager = nullptr;
    m_ownStadium = nullptr;
    m_linkedStadium.m_stadium = nullptr;
    m_league = nullptr;
    m_previousLeague = nullptr;
}
