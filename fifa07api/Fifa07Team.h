#pragma once
#include <vector>
#include <string>
#include "Fifa07DataFile.h"
#include "Fifa07Kit.h"
#include "Fifa07Propertied.h"

class Fifa07Team : public Fifa07Propertied {
public:
    int teamid = 0;
    std::wstring teamname;
    int generickitid = 0;
    int balltype = 0;
    int adboardid = 0;
    int stadiumid = 0;
    int genericbanner = 0;
    int trainingstadiumid = 0;
    int speechcountryid = 0;
    int opponent = 0;
    int assetid = 0;
    int teamchemistry = 0;
    int overallrating = 0;
    int signsyouthplayersfrom = 0;
    int signsplayersfromprimary = 0;
    int signsplayersfromsecondary = 0;
    int initialtransferbudget = 0;
    int internationalprestige = 0;
    int domesticprestige = 0;
    int homecrowdsize = 0;
    int teamprimarycolor = 0;
    int teamsecondarycolor = 0;
    // 08
    int defaultformationid = 0;

    // teamwrite
    int offsidetrap = 0;
    int withoutball = 0;
    int formationid = 0;
    int attack = 0;
    int teammentality = 0;
    int attacktactic1 = 0;
    int attacktactic2 = 0;
    int defensetactic1 = 0;
    int defensetactic2 = 0;
    int captain = 0;
    int penaltytakerid = 0;
    int kicktakerid = 0;
    int leftcornerkicktakerid = 0;
    int rightcornerkicktakerid = 0;
    int numtransfersin = 0;

    std::vector<Fifa07Kit> kits;

    struct LinkedPlayer {
        int playerid = 0;
        int jerseynumber = 0;
        int position = 0;
        int yearcontractvalidtil = 0;
        int transferdone = 0;
    };

    std::vector<LinkedPlayer> players;

    Fifa07Team() {}
    Fifa07Team(Fifa07DataFile::Line &line);
    void Parse(Fifa07DataFile::Line &line);
    void ParseTeamWrite(Fifa07DataFile::Line &line);
    unsigned int GetId();
};
