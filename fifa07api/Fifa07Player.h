#pragma once
#include <string>
#include "Fifa07DataFile.h"
#include "Fifa07Propertied.h"

class Fifa07Player : public Fifa07Propertied {
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

    int playerid = 0;
    std::wstring firstname;
    std::wstring surname;
    int birthdate = 0;
    int originalname = 0;
    int height = 0;
    int weight = 0;
    int preferredposition1 = 0;
    int preferredfoot = 0;
    int facetypeid = 0;
    int haircolorid = 0;
    int hairtypeid = 0;
    int facialhairstyleid = 0;
    int legtypeid = 0;
    int sleevelength = 0;
    int accessoryid1 = 0;
    int accessorycolor1 = 0;
    int accessoryid2 = 0;
    int accessorycolor2 = 0;
    int accessoryid3 = 0;
    int accessorycolor3 = 0;
    int accessoryid4 = 0;
    int accessorycolor4 = 0;
    int accessoryid5 = 0;
    int accessorycolor5 = 0;
    int shoecolor = 0;
    int injuryprone = 0;
    int acceleration = 0;
    int aggression = 0;
    int sprintspeed = 0;
    int stamina = 0;
    int strength = 0;
    int marking = 0;
    int tackling = 0;
    int ballcontrol = 0;
    int dribbling = 0;
    int crossing = 0;
    int heading = 0;
    int shortpassing = 0;
    int longpassing = 0;
    int longshotaccuracy = 0;
    int finishing = 0;
    int shotpower = 0;
    int reactions = 0;
    int reflexes = 0;
    int handling = 0;
    int gkdiving = 0;
    int gkpositioning = 0;
    int playercomposure = 0;
    int freekicks = 0;
    int flexibility = 0;
    int adaptability = 0;
    int ambition = 0;
    int fansfavourite = 0;
    int highclubidentification = 0;
    int lowclubidentification = 0;
    int mediadarling = 0;
    int scandalprone = 0;
    int hightrainingworkrate = 0;
    int lowtrainingworkrate = 0;
    int inflexible = 0;
    int assetid = 0;
    int potential = 0;
    int nationality = 0;
    int bodysizecode = 0;
    int leadership = 0;
    int lowmedhairtypeid = 0;
    int retirementage = 0;
    int celebration = 0;
    int learningspeed = 0;
    int wouldsignfor = 0;
    int wouldnotsignfor = 0;
    int weakfootability = 0;
    int playingstyle = 0;
    int developmentstylecode = 0;
    int developmenttypecode = 0;
    int internationalreputation = 0;
    // 08
    int headtypeid = 0;
    int positioning = 0;
    int tacticalawareness = 0;
    int comesforcrosses = 0;
    int diver = 0;
    int divesintotackles = 0;
    int earlycrosser = 0;
    int holdsup = 0;
    int latecrosser = 0;
    int longshottaker = 0;
    int longthrows = 0;
    int lowconcentration = 0;
    int onetimepasser = 0;
    int playmaker = 0;
    int puncher = 0;
    int pushesupforcorners = 0;
    int rushesoutofgoal = 0;
    int selfish = 0;
    int staysongoalline = 0;
    int technicaldribbler = 0;
    int exportfromdb = 0;

    void Parse(Fifa07DataFile::Line &line);

    Fifa07Player() {}
    Fifa07Player(Fifa07DataFile::Line &line);
    unsigned int GetId();
};
