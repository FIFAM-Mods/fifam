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
        int balltype = 0;
        int managerid = 0;
        int physioid_secondary = 0;
        int physioid_primary = 0;
        int fancrowdhairskintexturecode = 0;
        int stafftracksuitcolorcode = 0;
        int trait1 = 0;
        int busbuildupspeedvequal = 0;
        int busbuildupspeedvweak = 0;
        int defmentalityvsweak = 0;
        int buspassingvequal = 0;
        int defmentalityvstrong = 0;
        int busbuildupspeedvstrong = 0;
        int defmentalityvequal = 0;
        int buspassingvstrong = 0;
        int buspassingvweak = 0;

        int assetid = 0;
        int ballid = 0;
        int teamcolor1g = 0;
        int teamcolor1r = 0;
        int clubworth = 0;
        int teamcolor2b = 0;
        int teamcolor2r = 0;
        int foundationyear = 0;
        int teamcolor3r = 0;
        int teamcolor1b = 0;
        int opponentweakthreshold = 0;
        int latitude = 0;
        int teamcolor3g = 0;
        int opponentstrongthreshold = 0;
        int teamcolor2g = 0;
        std::wstring teamname;
        int adboardid = 0;
        int teamcolor3b = 0;
        int defmentality = 0;
        int powid = 0;
        int rightfreekicktakerid = 0;
        int domesticprestige = 0;
        int genericint2 = 0;
        int jerseytype = 0;
        int popularity = 0;
        int teamstadiumcapacity = 0;
        int iscompetitionscarfenabled = 0;
        int cityid = 0;
        int rivalteam = 0;
        int isbannerenabled = 0;
        int midfieldrating = 0;
        int matchdayoverallrating = 0;
        int matchdaymidfieldrating = 0;
        int attackrating = 0;
        int longitude = 0;
        int buspassing = 0;
        int matchdaydefenserating = 0;
        int defenserating = 0;
        int iscompetitionpoleflagenabled = 0;
        int skinnyflags = 0;
        int defteamwidth = 0;
        int longkicktakerid = 0;
        int bodytypeid = 0;
        int trait1vweak = 0;
        int busdribbling = 0;
        int rightcornerkicktakerid = 0;
        int suitvariationid = 0;
        int domesticcups = 0;
        int defaggression = 0;
        int ethnicity = 0;
        int leftcornerkicktakerid = 0;
        int youthdevelopment = 0;
        int teamid = 0;
        int trait1vequal = 0;
        int suittypeid = 0;
        int numtransfersin = 0;
        int stanchionflamethrower = 0;
        int captainid = 0;
        int personalityid = 0;
        int leftfreekicktakerid = 0;
        int leaguetitles = 0;
        int genericbanner = 0;
        int buspositioning = 0;
        int ccpositioning = 0;
        int busbuildupspeed = 0;
        int transferbudget = 0;
        int ccshooting = 0;
        int overallrating = 0;
        int ccpassing = 0;
        int profitability = 0;
        int utcoffset = 0;
        int penaltytakerid = 0;
        int freekicktakerid = 0;
        int crowdskintonecode = 0;
        int defdefenderline = 0;
        int internationalprestige = 0;
        int trainingstadium = 0;
        int form = 0;
        int genericint1 = 0;
        int cccrossing = 0;
        int trait1vstrong = 0;
        int matchdayattackrating = 0;
        // @since FIFA19
        int defensivestyle = 0;
        int flamethrowercannon = 0;
        int defensivedepth = 0;
        int hasvikingclap = 0;
        int hastifo = 0;
        int defensivewidth = 0;
        int playersinboxcorner = 0;
        int uefa_cl_wins = 0;
        int uefa_el_wins = 0;
        int playersinboxfk = 0;
        int offensivestyle = 0;
        int playersinboxcross = 0;
        int prev_el_champ = 0;
        int offensivewidth = 0;
        // @since FIFA20
        int hasstandingcrowd = 0;
        int favoriteteamsheetid = 0;
        int uefa_consecutive_wins = 0;
        int iscompetitioncrowdcardsenabled = 0;
        // @since FIFA21
        int goalnetstanchioncolor2g = 0;
        int goalnetstanchioncolor2r = 0;
        int goalnetstanchioncolor1b = 0;
        int goalnetstanchioncolor2b = 0;
        int goalnetstanchioncolor1r = 0;
        int goalnetstanchioncolor1g = 0;
        int presassetone = 0;
        int pitchcolor = 0;
        int pitchwear = 0;
        int presassettwo = 0;
        int stadiumgoalnetstyle = 0;
        int playsurfacetype = 0;
        int stadiummowpattern_code = 0;
        int stadiumgoalnetpattern = 0;
        int pitchlinecolor = 0;
        // @since FIFA22
        int hassubstitutionboard = 0;
        int chancecreation = 0;
        int buildupplay = 0;
        int crowdregion = 0;
        int uefa_uecl_wins = 0;
        int haslargeflag = 0;


        void Read(FifaDataFile::Line &line);
    } internal;

    struct LinkedPlayer {
        FifaPlayer *m_player = nullptr;
        FifaPlayer::Position m_position = FifaPlayer::POS_RES;
        unsigned char m_number = 0;
    };

    std::wstring m_name;
    std::vector<LinkedPlayer> m_linkedPlayers;
    FifaManager *m_manager = nullptr;
    std::vector<FifaPlayer *> m_playersOnLoan;
    FifaLeague *m_league = nullptr;
    FifaLeague *m_previousLeague = nullptr;
    FifaStadium *m_ownStadium = nullptr;
    struct StadiumLink {
        std::wstring m_name;
        FifaStadium *m_stadium = nullptr;
    } m_linkedStadium;
    std::vector<FifaKit *> m_kits;
    unsigned int m_gameId = 0;

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
