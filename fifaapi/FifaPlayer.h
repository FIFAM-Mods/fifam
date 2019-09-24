#pragma once
#include "FifaDate.h"
#include <vector>
#include <string>
#include "FifaDataFile.h"

class FifaTeam;

class FifaPlayer {
public:
    enum Position {
        POS_GK,
        POS_SW,
        POS_RWB,
        POS_RB,
        POS_RCB,
        POS_CB,
        POS_LCB,
        POS_LB,
        POS_LWB,
        POS_RDM,
        POS_CDM,
        POS_LDM,
        POS_RM,
        POS_RCM,
        POS_CM,
        POS_LCM,
        POS_LM,
        POS_RAM,
        POS_CAM,
        POS_LAM,
        POS_RF,
        POS_CF,
        POS_LF,
        POS_RW,
        POS_RS,
        POS_ST,
        POS_LS,
        POS_LW,
        POS_SUB,
        POS_RES
    };

    struct Internal {
        int firstnameid;
        int lastnameid;
        int playerjerseynameid;
        int commonnameid;
        int trait2;
        int haircolorcode;
        int facialhairtypecode;
        int curve;
        int jerseystylecode;
        int agility;
        int accessorycode4;
        int gksavetype;
        int positioning;
        int tattooleftarm;
        int hairtypecode;
        int standingtackle;
        int tattoobackneck;
        int preferredposition3;
        int longpassing;
        int penalties;
        int animfreekickstartposcode;
        int animpenaltieskickstylecode;
        int isretiring;
        int longshots;
        int gkdiving;
        int interceptions;
        int shoecolorcode2;
        int crossing;
        int potential;
        int gkreflexes;
        int finishingcode1;
        int reactions;
        int composure;
        int vision;
        int contractvaliduntil;
        int animpenaltiesapproachcode;
        int finishing;
        int dribbling;
        int slidingtackle;
        int accessorycode3;
        int accessorycolourcode1;
        int headtypecode;
        int sprintspeed;
        int height;
        int hasseasonaljersey;
        int preferredposition2;
        int strength;
        int shoetypecode;
        int birthdate;
        int preferredposition1;
        int ballcontrol;
        int shotpower;
        int trait1;
        int socklengthcode;
        int weight;
        int hashighqualityhead;
        int gkglovetypecode;
        int tattoorightarm;
        int balance;
        int gender;
        int gkkicking;
        int internationalrep;
        int animpenaltiesmotionstylecode;
        int shortpassing;
        int freekickaccuracy;
        int skillmoves;
        int usercaneditname;
        int attackingworkrate;
        int finishingcode2;
        int aggression;
        int acceleration;
        int headingaccuracy;
        int iscustomized;
        int eyebrowcode;
        int runningcode2;
        int modifier;
        int gkhandling;
        int eyecolorcode;
        int jerseysleevelengthcode;
        int accessorycolourcode3;
        int accessorycode1;
        int playerjointeamdate;
        int headclasscode;
        int defensiveworkrate;
        int nationality;
        int preferredfoot;
        int sideburnscode;
        int weakfootabilitytypecode;
        int jumping;
        int skintypecode;
        int tattoorightneck;
        int gkkickstyle;
        int stamina;
        int playerid;
        int marking;
        int accessorycolourcode4;
        int gkpositioning;
        int headvariation;
        int skintonecode;
        int shortstyle;
        int overallrating;
        int tattooleftneck;
        int emotion;
        int jerseyfit;
        int accessorycode2;
        int shoedesigncode;
        int shoecolorcode1;
        int hairstylecode;
        int bodytypecode;
        int animpenaltiesstartposcode;
        int runningcode1;
        int preferredposition4;
        int volleys;
        int accessorycolourcode2;
        int facialhaircolorcode;
        // @since FIFA19
        int headassetid;
        int faceposerpreset;
        int skillmoveslikelihood;
        // @since FIFA20
        int tattooback;
        int tattoofront;
        int tattoohead;
        int tattooleftleg;
        int tattoorightleg;
        int avatarpomid;
        int personality;
        int smallsidedshoetypecode;
        int runstylecode;

        void Read(FifaDataFile::Line &line);
    } internal;

    struct LinkedTeam {
        FifaTeam *m_team;
        Position m_position;
        unsigned char m_number;
    };

    FifaTeam *m_loanedFrom;
    FifaDate m_loanEndDate;
    FifaDate m_birthDate;
    bool m_inNationalTeam;
    bool m_linkedWithClub;
    std::vector<LinkedTeam> m_linkedTeams;
    std::wstring m_firstName;
    std::wstring m_lastName;
    std::wstring m_commonName;
    std::wstring m_playerJerseyName;
    std::wstring m_quickName;
    unsigned int m_gameId;

    FifaPlayer(FifaDataFile::Line &line);
    unsigned int GetId();
    bool PlayerInTeam(FifaTeam *team);
    bool IsMale();
    FifaTeam *GetLeagueTeam();
    FifaTeam *GetInternationalTeam();

    template <typename T>
    void ForAllTeams(T callback) {
        for (LinkedTeam &link : m_linkedTeams)
            callback(*link.m_team);
    }

    template <typename T>
    void ForAllTeamLinks(T callback) {
        for (LinkedTeam &link : m_linkedTeams)
            callback(link);
    }
};
