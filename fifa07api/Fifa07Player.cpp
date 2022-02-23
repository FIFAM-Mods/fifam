#include "Fifa07Player.h"
#include "Fifa07Database.h"
#include <sstream>

unsigned int Fifa07Player::GetId() {
    return playerid;
}

void Fifa07Player::Parse(Fifa07DataFile::Line &line) {
    switch (Fifa07Database::m_currentGameVersion) {
    case 7:
        line >> playerid >> firstname >> surname >> birthdate >> originalname >> height >> weight >> preferredposition1 >> preferredfoot >> facetypeid >> haircolorid >> hairtypeid >> facialhairstyleid >> legtypeid >> sleevelength >> accessoryid1 >> accessorycolor1 >> accessoryid2 >> accessorycolor2 >> accessoryid3 >> accessorycolor3 >> accessoryid4 >> accessorycolor4 >> accessoryid5 >> accessorycolor5 >> shoecolor >> injuryprone >> acceleration >> aggression >> sprintspeed >> stamina >> strength >> marking >> tackling >> ballcontrol >> dribbling >> crossing >> heading >> shortpassing >> longpassing >> longshotaccuracy >> finishing >> shotpower >> reactions >> reflexes >> handling >> gkdiving >> gkpositioning >> playercomposure >> freekicks >> flexibility >> adaptability >> ambition >> fansfavourite >> highclubidentification >> lowclubidentification >> mediadarling >> scandalprone >> hightrainingworkrate >> lowtrainingworkrate >> inflexible >> assetid >> potential >> nationality >> bodysizecode >> leadership >> lowmedhairtypeid >> retirementage >> celebration >> learningspeed >> wouldsignfor >> wouldnotsignfor >> weakfootability >> playingstyle >> developmentstylecode >> developmenttypecode >> internationalreputation; 
        break;
    case 8:
        line >> playerid >> firstname >> surname >> birthdate >> originalname >> height >> weight >> preferredposition1 >> preferredfoot >> facetypeid >> headtypeid >> haircolorid >> hairtypeid >> lowmedhairtypeid >> facialhairstyleid >> bodysizecode >> legtypeid >> shoecolor >> sleevelength >> accessoryid1 >> accessorycolor1 >> accessoryid2 >> accessorycolor2 >> accessoryid3 >> accessorycolor3 >> accessoryid4 >> accessorycolor4 >> accessoryid5 >> accessorycolor5 >> injuryprone >> acceleration >> aggression >> ballcontrol >> crossing >> dribbling >> finishing >> freekicks >> heading >> longpassing >> longshotaccuracy >> marking >> playercomposure >> positioning >> potential >> reactions >> shortpassing >> shotpower >> sprintspeed >> stamina >> strength >> tackling >> tacticalawareness >> gkdiving >> gkpositioning >> handling >> reflexes >> adaptability >> ambition >> comesforcrosses >> diver >> divesintotackles >> earlycrosser >> fansfavourite >> flexibility >> highclubidentification >> hightrainingworkrate >> holdsup >> inflexible >> latecrosser >> leadership >> longshottaker >> longthrows >> lowclubidentification >> lowconcentration >> lowtrainingworkrate >> mediadarling >> onetimepasser >> playmaker >> puncher >> pushesupforcorners >> rushesoutofgoal >> scandalprone >> selfish >> staysongoalline >> technicaldribbler >> assetid >> nationality >> retirementage >> celebration >> learningspeed >> wouldsignfor >> wouldnotsignfor >> weakfootability >> playingstyle >> developmentstylecode >> developmenttypecode >> internationalreputation >> exportfromdb;
        break;
    }
}

Fifa07Player::Fifa07Player(Fifa07DataFile::Line &line) {
    Parse(line);;
}
