#pragma once

#include <vector>
#include <string>
#include "FifaPlayer.h"
#include "FifaManager.h"
#include "FifaStadium.h"
#include "FifaKit.h"
#include "FifaDataFile.h"

class FifaLeague;

class FifaTeam {
public:
    struct Internal {
        int assetid;
        int ballid;
        int teamcolor1g;
        int teamcolor1r;
        int clubworth;
        int teamcolor2b;
        int teamcolor2r;
        int foundationyear;
        int teamcolor3r;
        int teamcolor1b;
        int opponentweakthreshold;
        int latitude;
        int teamcolor3g;
        int opponentstrongthreshold;
        int teamcolor2g;
        std::wstring teamname;
        int adboardid;
        int teamcolor3b;
        int defmentality;
        int powid;
        int rightfreekicktakerid;
        int domesticprestige;
        int genericint2;
        int jerseytype;
        int popularity;
        int teamstadiumcapacity;
        int iscompetitionscarfenabled;
        int cityid;
        int rivalteam;
        int isbannerenabled;
        int midfieldrating;
        int matchdayoverallrating;
        int matchdaymidfieldrating;
        int attackrating;
        int longitude;
        int buspassing;
        int matchdaydefenserating;
        int defenserating;
        int iscompetitionpoleflagenabled;
        int skinnyflags;
        int defteamwidth;
        int longkicktakerid;
        int bodytypeid;
        int trait1vweak;
        int busdribbling;
        int rightcornerkicktakerid;
        int suitvariationid;
        int domesticcups;
        int defaggression;
        int ethnicity;
        int leftcornerkicktakerid;
        int youthdevelopment;
        int teamid;
        int trait1vequal;
        int suittypeid;
        int numtransfersin;
        int stanchionflamethrower;
        int captainid;
        int personalityid;
        int leftfreekicktakerid;
        int leaguetitles;
        int genericbanner;
        int buspositioning;
        int ccpositioning;
        int busbuildupspeed;
        int transferbudget;
        int ccshooting;
        int overallrating;
        int ccpassing;
        int profitability;
        int utcoffset;
        int penaltytakerid;
        int freekicktakerid;
        int crowdskintonecode;
        int defdefenderline;
        int internationalprestige;
        int trainingstadium;
        int form;
        int genericint1;
        int cccrossing;
        int trait1vstrong;
        int matchdayattackrating;
        // @since FIFA19
        int defensivestyle;
        int flamethrowercannon;
        int defensivedepth;
        int hasvikingclap;
        int hastifo;
        int defensivewidth;
        int playersinboxcorner;
        int uefa_cl_wins;
        int uefa_el_wins;
        int playersinboxfk;
        int offensivestyle;
        int playersinboxcross;
        int prev_el_champ;
        int offensivewidth;
        // @since FIFA20
        int hasstandingcrowd;
        int favoriteteamsheetid;
        int uefa_consecutive_wins;
        int iscompetitioncrowdcardsenabled;

        void Read(FifaDataFile::Line &line);
    } internal;

    struct LinkedPlayer {
        FifaPlayer *m_player;
        FifaPlayer::Position m_position;
        unsigned char m_number;
    };

    std::wstring m_name;
    std::vector<LinkedPlayer> m_linkedPlayers;
    FifaManager *m_manager;
    std::vector<FifaPlayer *> m_playersOnLoan;
    FifaLeague *m_league;
    FifaLeague *m_previousLeague;
    FifaStadium *m_ownStadium;
    struct StadiumLink {
        std::wstring m_name;
        FifaStadium *m_stadium;
    } m_linkedStadium;
    std::vector<FifaKit *> m_kits;
    unsigned int m_gameId;

    FifaTeam(FifaDataFile::Line &line);
    unsigned int GetId();

    template <typename T>
    void ForAllPlayersOnLoan(T callback) {
        for (FifaPlayer *player : m_playersOnLoan)
            callback(*player);
    }

    template <typename T>
    void ForAllPlayers(T callback) {
        for (LinkedPlayer &linkedPlayer : m_linkedPlayers)
            callback(*linkedPlayer.m_player);
    }

    template <typename T>
    void ForAllPlayersEx(T callback) {
        for (LinkedPlayer &linkedPlayer : m_linkedPlayers)
            callback(*linkedPlayer.m_player, linkedPlayer.m_position, linkedPlayer.m_number);
    }

    template <typename T>
    void ForAllPlayerLinks(T callback) {
        for (LinkedPlayer &linkedPlayer : m_linkedPlayers)
            callback(linkedPlayer);
    }

    FifaKit *GetKit(int kitType) {
        for (FifaKit *kit : m_kits) {
            if (kit->internal.teamkittypetechid == kitType)
                return kit;
        }
        return nullptr;
    }
};
