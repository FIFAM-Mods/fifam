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
        int tacticalawareness = 0;
        int perfconsistencytype = 0;
        int lowmedhairtypeid = 0;
        int dribspeed = 0;
        int retirementage = 0;
        int trait3 = 0;
        int developmentstylecode = 0;
        int haswintergloves = 0;
        int gkoneonone = 0;
        int developmenttypecode = 0;
        int faceposercode = 0;

        int firstnameid = 0;
        int lastnameid = 0;
        int playerjerseynameid = 0;
        int commonnameid = 0;
        int trait2 = 0;
        int haircolorcode = 0;
        int facialhairtypecode = 0;
        int curve = 0;
        int jerseystylecode = 0;
        int agility = 0;
        int accessorycode4 = 0;
        int gksavetype = 0;
        int positioning = 0;
        int tattooleftarm = 0;
        int hairtypecode = 0;
        int standingtackle = 0;
        int tattoobackneck = 0;
        int preferredposition3 = 0;
        int longpassing = 0;
        int penalties = 0;
        int animfreekickstartposcode = 0;
        int animpenaltieskickstylecode = 0;
        int isretiring = 0;
        int longshots = 0;
        int gkdiving = 0;
        int interceptions = 0;
        int shoecolorcode2 = 0;
        int crossing = 0;
        int potential = 0;
        int gkreflexes = 0;
        int finishingcode1 = 0;
        int reactions = 0;
        int composure = 0;
        int vision = 0;
        int contractvaliduntil = 0;
        int animpenaltiesapproachcode = 0;
        int finishing = 0;
        int dribbling = 0;
        int slidingtackle = 0;
        int accessorycode3 = 0;
        int accessorycolourcode1 = 0;
        int headtypecode = 0;
        int sprintspeed = 0;
        int height = 0;
        int hasseasonaljersey = 0;
        int preferredposition2 = 0;
        int strength = 0;
        int shoetypecode = 0;
        int birthdate = 0;
        int preferredposition1 = 0;
        int ballcontrol = 0;
        int shotpower = 0;
        int trait1 = 0;
        int socklengthcode = 0;
        int weight = 0;
        int hashighqualityhead = 0;
        int gkglovetypecode = 0;
        int tattoorightarm = 0;
        int balance = 0;
        int gender = 0;
        int gkkicking = 0;
        int internationalrep = 0;
        int animpenaltiesmotionstylecode = 0;
        int shortpassing = 0;
        int freekickaccuracy = 0;
        int skillmoves = 0;
        int usercaneditname = 0;
        int attackingworkrate = 0;
        int finishingcode2 = 0;
        int aggression = 0;
        int acceleration = 0;
        int headingaccuracy = 0;
        int iscustomized = 0;
        int eyebrowcode = 0;
        int runningcode2 = 0;
        int modifier = 0;
        int gkhandling = 0;
        int eyecolorcode = 0;
        int jerseysleevelengthcode = 0;
        int accessorycolourcode3 = 0;
        int accessorycode1 = 0;
        int playerjointeamdate = 0;
        int headclasscode = 0;
        int defensiveworkrate = 0;
        int nationality = 0;
        int preferredfoot = 0;
        int sideburnscode = 0;
        int weakfootabilitytypecode = 0;
        int jumping = 0;
        int skintypecode = 0;
        int tattoorightneck = 0;
        int gkkickstyle = 0;
        int stamina = 0;
        int playerid = 0;
        int marking = 0;
        int accessorycolourcode4 = 0;
        int gkpositioning = 0;
        int headvariation = 0;
        int skintonecode = 0;
        int shortstyle = 0;
        int overallrating = 0;
        int tattooleftneck = 0;
        int emotion = 0;
        int jerseyfit = 0;
        int accessorycode2 = 0;
        int shoedesigncode = 0;
        int shoecolorcode1 = 0;
        int hairstylecode = 0;
        int bodytypecode = 0;
        int animpenaltiesstartposcode = 0;
        int runningcode1 = 0;
        int preferredposition4 = 0;
        int volleys = 0;
        int accessorycolourcode2 = 0;
        int facialhaircolorcode = 0;
        // @since FIFA19
        int headassetid = 0;
        int faceposerpreset = 0;
        int skillmoveslikelihood = 0;
        // @since FIFA20
        int tattooback = 0;
        int tattoofront = 0;
        int tattoohead = 0;
        int tattooleftleg = 0;
        int tattoorightleg = 0;
        int avatarpomid = 0;
        int personality = 0;
        int smallsidedshoetypecode = 0;
        int runstylecode = 0;
        // @since FIFA21
        // @since FIFA22
        int driref = 0;
        int phypos = 0;
        int defspe = 0;
        int paskic = 0;
        int shohan = 0;
        int pacdiv = 0;
        // @since FIFA23
        int defensiveawareness = 0;
        // @since FC24
        int icontrait2 = 0;
        int icontrait1 = 0;
        int muscularitycode = 0;
        // @since FC25
        int role1 = 0;
        int role2 = 0;
        int role3 = 0;
        int lipcolor = 0;
        int skinsurfacepack = 0;
        int skinmakeup = 0;
        int eyedetail = 0;
        int skincomplexion = 0;
        int undershortstyle = 0;

        void Read(FifaDataFile::Line &line);
    } internal;

    struct LinkedTeam {
        FifaTeam *m_team = nullptr;
        Position m_position = POS_RES;
        unsigned char m_number = 0;
    };

    FifaTeam *m_loanedFrom = nullptr;
    FifaDate m_loanEndDate;
    FifaDate m_birthDate;
    bool m_inNationalTeam = false;
    bool m_linkedWithClub = false;
    std::vector<LinkedTeam> m_linkedTeams;
    std::wstring m_firstName;
    std::wstring m_lastName;
    std::wstring m_commonName;
    std::wstring m_playerJerseyName;
    std::wstring m_quickName;
    unsigned int m_gameId = 0;

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
