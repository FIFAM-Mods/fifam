#include "FifaPlayer.h"
#include "FifaTeam.h"
#include "FifaLeague.h"
#include "FifaDatabase.h"
#include <sstream>

unsigned int FifaPlayer::GetId() {
    return internal.playerid;
}

void FifaPlayer::Internal::Read(FifaDataFile::Line &line) {
    switch (FifaDatabase::m_currentGameVersion) {
    case 11:
        line >> birthdate >> playerjointeamdate >> shoetypecode >> haircolorcode >> facialhairtypecode >> curve >> tacticalawareness >> jerseystylecode >> agility >> accessorycode4 >> positioning >> hairtypecode >> standingtackle >> perfconsistencytype >> preferredposition3 >> longpassing >> penalties >> animfreekickstartposcode >> animpenaltieskickstylecode >> longshots >> gkdiving >> lowmedhairtypeid >> crossing >> potential >> gkreflexes >> reactions >> vision >> contractvaliduntil >> finishing >> dribspeed >> dribbling >> slidingtackle >> accessorycode3 >> accessorycolourcode1 >> headtypecode >> firstnameid >> sprintspeed >> height >> hasseasonaljersey >> preferredposition2 >> strength >> retirementage >> preferredposition1 >> ballcontrol >> shotpower >> trait1 >> weight >> gkglovetypecode >> balance >> gkkicking >> lastnameid >> internationalrep >> animpenaltiesmotionstylecode >> shortpassing >> freekickaccuracy >> skillmoves >> trait3 >> usercaneditname >> aggression >> acceleration >> headingaccuracy >> eyebrowcode >> gkhandling >> eyecolorcode >> jerseysleevelengthcode >> accessorycolourcode3 >> accessorycode1 >> headclasscode >> developmentstylecode >> nationality >> preferredfoot >> sideburnscode >> weakfootabilitytypecode >> jumping >> skintypecode >> gkkickstyle >> stamina >> playerid >> marking >> accessorycolourcode4 >> gkpositioning >> trait2 >> skintonecode >> haswintergloves >> overallrating >> gkoneonone >> accessorycode2 >> playerjerseynameid >> shoecolorcode1 >> commonnameid >> developmenttypecode >> bodytypecode >> animpenaltiesstartposcode >> preferredposition4 >> volleys >> accessorycolourcode2 >> facialhaircolorcode;
        break;
    case 12:
        line >> birthdate >> playerjointeamdate >> shoetypecode >> haircolorcode >> facialhairtypecode >> curve >> tacticalawareness >> jerseystylecode >> agility >> accessorycode4 >> gksavetype >> positioning >> hairtypecode >> standingtackle >> faceposercode >> perfconsistencytype >> preferredposition3 >> longpassing >> penalties >> animfreekickstartposcode >> animpenaltieskickstylecode >> isretiring >> longshots >> gkdiving >> crossing >> potential >> gkreflexes >> finishingcode1 >> reactions >> vision >> contractvaliduntil >> finishing >> dribbling >> slidingtackle >> accessorycode3 >> accessorycolourcode1 >> headtypecode >> firstnameid >> sprintspeed >> height >> hasseasonaljersey >> preferredposition2 >> strength >> preferredposition1 >> ballcontrol >> shotpower >> trait1 >> socklengthcode >> weight >> hashighqualityhead >> gkglovetypecode >> balance >> gkkicking >> lastnameid >> internationalrep >> animpenaltiesmotionstylecode >> shortpassing >> freekickaccuracy >> skillmoves >> usercaneditname >> attackingworkrate >> finishingcode2 >> aggression >> acceleration >> headingaccuracy >> eyebrowcode >> runningcode2 >> gkhandling >> eyecolorcode >> jerseysleevelengthcode >> accessorycolourcode3 >> accessorycode1 >> headclasscode >> defensiveworkrate >> nationality >> preferredfoot >> sideburnscode >> weakfootabilitytypecode >> jumping >> skintypecode >> gkkickstyle >> stamina >> playerid >> marking >> accessorycolourcode4 >> gkpositioning >> trait2 >> skintonecode >> shortstyle >> overallrating >> accessorycode2 >> playerjerseynameid >> shoecolorcode1 >> commonnameid >> bodytypecode >> animpenaltiesstartposcode >> runningcode1 >> preferredposition4 >> volleys >> accessorycolourcode2 >> facialhaircolorcode;
        break;
    case 13:
        line >> birthdate >> playerjointeamdate >> shoetypecode >> haircolorcode >> facialhairtypecode >> curve >> jerseystylecode >> agility >> accessorycode4 >> gksavetype >> positioning >> hairtypecode >> standingtackle >> faceposercode >> preferredposition3 >> longpassing >> penalties >> animfreekickstartposcode >> animpenaltieskickstylecode >> isretiring >> longshots >> gkdiving >> interceptions >> shoecolorcode2 >> crossing >> potential >> gkreflexes >> finishingcode1 >> reactions >> vision >> contractvaliduntil >> finishing >> dribbling >> slidingtackle >> accessorycode3 >> accessorycolourcode1 >> headtypecode >> firstnameid >> sprintspeed >> height >> hasseasonaljersey >> preferredposition2 >> strength >> preferredposition1 >> ballcontrol >> shotpower >> trait1 >> socklengthcode >> weight >> hashighqualityhead >> gkglovetypecode >> balance >> gkkicking >> lastnameid >> internationalrep >> animpenaltiesmotionstylecode >> shortpassing >> freekickaccuracy >> skillmoves >> usercaneditname >> attackingworkrate >> finishingcode2 >> aggression >> acceleration >> headingaccuracy >> eyebrowcode >> runningcode2 >> gkhandling >> eyecolorcode >> jerseysleevelengthcode >> accessorycolourcode3 >> accessorycode1 >> headclasscode >> defensiveworkrate >> nationality >> preferredfoot >> sideburnscode >> weakfootabilitytypecode >> jumping >> skintypecode >> gkkickstyle >> stamina >> playerid >> marking >> accessorycolourcode4 >> gkpositioning >> trait2 >> skintonecode >> shortstyle >> overallrating >> jerseyfit >> accessorycode2 >> shoedesigncode >> playerjerseynameid >> shoecolorcode1 >> commonnameid >> bodytypecode >> animpenaltiesstartposcode >> runningcode1 >> preferredposition4 >> volleys >> accessorycolourcode2 >> facialhaircolorcode;
        break;
    case 14:
        line >> birthdate >> playerjointeamdate >> shoetypecode >> haircolorcode >> facialhairtypecode >> curve >> jerseystylecode >> agility >> accessorycode4 >> gksavetype >> positioning >> hairtypecode >> standingtackle >> faceposercode >> preferredposition3 >> longpassing >> penalties >> animfreekickstartposcode >> animpenaltieskickstylecode >> isretiring >> longshots >> gkdiving >> interceptions >> shoecolorcode2 >> crossing >> potential >> gkreflexes >> finishingcode1 >> reactions >> vision >> contractvaliduntil >> finishing >> dribbling >> slidingtackle >> accessorycode3 >> accessorycolourcode1 >> headtypecode >> firstnameid >> sprintspeed >> height >> hasseasonaljersey >> preferredposition2 >> strength >> preferredposition1 >> ballcontrol >> shotpower >> trait1 >> socklengthcode >> weight >> hashighqualityhead >> gkglovetypecode >> balance >> gkkicking >> lastnameid >> internationalrep >> animpenaltiesmotionstylecode >> shortpassing >> freekickaccuracy >> skillmoves >> usercaneditname >> attackingworkrate >> finishingcode2 >> aggression >> acceleration >> headingaccuracy >> eyebrowcode >> runningcode2 >> gkhandling >> eyecolorcode >> jerseysleevelengthcode >> accessorycolourcode3 >> accessorycode1 >> headclasscode >> defensiveworkrate >> nationality >> preferredfoot >> sideburnscode >> weakfootabilitytypecode >> jumping >> skintypecode >> gkkickstyle >> stamina >> playerid >> marking >> accessorycolourcode4 >> gkpositioning >> trait2 >> skintonecode >> shortstyle >> overallrating >> jerseyfit >> accessorycode2 >> shoedesigncode >> playerjerseynameid >> shoecolorcode1 >> commonnameid >> bodytypecode >> animpenaltiesstartposcode >> runningcode1 >> preferredposition4 >> volleys >> accessorycolourcode2 >> facialhaircolorcode;
        break;
    case 15:
        line >> shoetypecode >> haircolorcode >> facialhairtypecode >> curve >> jerseystylecode >> agility >> accessorycode4 >> gksavetype >> positioning >> hairtypecode >> standingtackle >> faceposercode >> preferredposition3 >> longpassing >> penalties >> animfreekickstartposcode >> animpenaltieskickstylecode >> isretiring >> longshots >> gkdiving >> interceptions >> shoecolorcode2 >> crossing >> potential >> gkreflexes >> finishingcode1 >> reactions >> vision >> contractvaliduntil >> finishing >> dribbling >> slidingtackle >> accessorycode3 >> accessorycolourcode1 >> headtypecode >> firstnameid >> sprintspeed >> height >> hasseasonaljersey >> preferredposition2 >> strength >> birthdate >> preferredposition1 >> ballcontrol >> shotpower >> trait1 >> socklengthcode >> weight >> hashighqualityhead >> gkglovetypecode >> balance >> gkkicking >> lastnameid >> internationalrep >> animpenaltiesmotionstylecode >> shortpassing >> freekickaccuracy >> skillmoves >> usercaneditname >> attackingworkrate >> finishingcode2 >> aggression >> acceleration >> headingaccuracy >> eyebrowcode >> runningcode2 >> gkhandling >> eyecolorcode >> jerseysleevelengthcode >> accessorycolourcode3 >> accessorycode1 >> playerjointeamdate >> headclasscode >> defensiveworkrate >> nationality >> preferredfoot >> sideburnscode >> weakfootabilitytypecode >> jumping >> skintypecode >> gkkickstyle >> stamina >> playerid >> marking >> accessorycolourcode4 >> gkpositioning >> trait2 >> skintonecode >> shortstyle >> overallrating >> jerseyfit >> accessorycode2 >> shoedesigncode >> playerjerseynameid >> shoecolorcode1 >> commonnameid >> bodytypecode >> animpenaltiesstartposcode >> runningcode1 >> preferredposition4 >> volleys >> accessorycolourcode2 >> facialhaircolorcode;
        break;
    case 16:
        line >> shoetypecode >> haircolorcode >> facialhairtypecode >> curve >> jerseystylecode >> agility >> accessorycode4 >> gksavetype >> positioning >> hairtypecode >> standingtackle >> faceposercode >> preferredposition3 >> longpassing >> penalties >> animfreekickstartposcode >> animpenaltieskickstylecode >> isretiring >> longshots >> gkdiving >> interceptions >> shoecolorcode2 >> crossing >> potential >> gkreflexes >> finishingcode1 >> reactions >> vision >> contractvaliduntil >> finishing >> dribbling >> slidingtackle >> accessorycode3 >> accessorycolourcode1 >> headtypecode >> firstnameid >> sprintspeed >> height >> hasseasonaljersey >> preferredposition2 >> strength >> birthdate >> preferredposition1 >> ballcontrol >> shotpower >> trait1 >> socklengthcode >> weight >> hashighqualityhead >> gkglovetypecode >> balance >> gender >> gkkicking >> lastnameid >> internationalrep >> animpenaltiesmotionstylecode >> shortpassing >> freekickaccuracy >> skillmoves >> usercaneditname >> attackingworkrate >> finishingcode2 >> aggression >> acceleration >> headingaccuracy >> eyebrowcode >> runningcode2 >> gkhandling >> eyecolorcode >> jerseysleevelengthcode >> accessorycolourcode3 >> accessorycode1 >> playerjointeamdate >> headclasscode >> defensiveworkrate >> nationality >> preferredfoot >> sideburnscode >> weakfootabilitytypecode >> jumping >> skintypecode >> gkkickstyle >> stamina >> playerid >> marking >> accessorycolourcode4 >> gkpositioning >> trait2 >> skintonecode >> shortstyle >> overallrating >> emotion >> jerseyfit >> accessorycode2 >> shoedesigncode >> playerjerseynameid >> shoecolorcode1 >> commonnameid >> bodytypecode >> animpenaltiesstartposcode >> runningcode1 >> preferredposition4 >> volleys >> accessorycolourcode2 >> facialhaircolorcode;
        break;
    case 17:
        line >> shoetypecode >> haircolorcode >> facialhairtypecode >> curve >> jerseystylecode >> agility >> accessorycode4 >> gksavetype >> positioning >> hairtypecode >> standingtackle >> preferredposition3 >> longpassing >> penalties >> animfreekickstartposcode >> animpenaltieskickstylecode >> isretiring >> longshots >> gkdiving >> interceptions >> shoecolorcode2 >> crossing >> potential >> gkreflexes >> finishingcode1 >> reactions >> composure >> vision >> contractvaliduntil >> animpenaltiesapproachcode >> finishing >> dribbling >> slidingtackle >> accessorycode3 >> accessorycolourcode1 >> headtypecode >> firstnameid >> sprintspeed >> height >> hasseasonaljersey >> preferredposition2 >> strength >> birthdate >> preferredposition1 >> ballcontrol >> shotpower >> trait1 >> socklengthcode >> weight >> hashighqualityhead >> gkglovetypecode >> balance >> gender >> gkkicking >> lastnameid >> internationalrep >> animpenaltiesmotionstylecode >> shortpassing >> freekickaccuracy >> skillmoves >> usercaneditname >> attackingworkrate >> finishingcode2 >> aggression >> acceleration >> headingaccuracy >> eyebrowcode >> runningcode2 >> modifier >> gkhandling >> eyecolorcode >> jerseysleevelengthcode >> accessorycolourcode3 >> accessorycode1 >> playerjointeamdate >> headclasscode >> defensiveworkrate >> nationality >> preferredfoot >> sideburnscode >> weakfootabilitytypecode >> jumping >> skintypecode >> gkkickstyle >> stamina >> playerid >> marking >> accessorycolourcode4 >> gkpositioning >> trait2 >> skintonecode >> shortstyle >> overallrating >> emotion >> jerseyfit >> accessorycode2 >> shoedesigncode >> playerjerseynameid >> shoecolorcode1 >> commonnameid >> bodytypecode >> animpenaltiesstartposcode >> runningcode1 >> preferredposition4 >> volleys >> accessorycolourcode2 >> facialhaircolorcode;
        break;
    case 18:
        line >> firstnameid >> lastnameid >> playerjerseynameid >> commonnameid >> trait2 >> haircolorcode >> facialhairtypecode >> curve >> jerseystylecode >> agility >> accessorycode4 >> gksavetype >> positioning >> tattooleftarm >> hairtypecode >> standingtackle >> tattoobackneck >> preferredposition3 >> longpassing >> penalties >> animfreekickstartposcode >> animpenaltieskickstylecode >> isretiring >> longshots >> gkdiving >> interceptions >> shoecolorcode2 >> crossing >> potential >> gkreflexes >> finishingcode1 >> reactions >> composure >> vision >> contractvaliduntil >> animpenaltiesapproachcode >> finishing >> dribbling >> slidingtackle >> accessorycode3 >> accessorycolourcode1 >> headtypecode >> sprintspeed >> height >> hasseasonaljersey >> preferredposition2 >> strength >> shoetypecode >> birthdate >> preferredposition1 >> ballcontrol >> shotpower >> trait1 >> socklengthcode >> weight >> hashighqualityhead >> gkglovetypecode >> tattoorightarm >> balance >> gender >> gkkicking >> internationalrep >> animpenaltiesmotionstylecode >> shortpassing >> freekickaccuracy >> skillmoves >> usercaneditname >> attackingworkrate >> finishingcode2 >> aggression >> acceleration >> headingaccuracy >> iscustomized >> eyebrowcode >> runningcode2 >> modifier >> gkhandling >> eyecolorcode >> jerseysleevelengthcode >> accessorycolourcode3 >> accessorycode1 >> playerjointeamdate >> headclasscode >> defensiveworkrate >> nationality >> preferredfoot >> sideburnscode >> weakfootabilitytypecode >> jumping >> skintypecode >> tattoorightneck >> gkkickstyle >> stamina >> playerid >> marking >> accessorycolourcode4 >> gkpositioning >> headvariation >> skintonecode >> shortstyle >> overallrating >> tattooleftneck >> emotion >> jerseyfit >> accessorycode2 >> shoedesigncode >> shoecolorcode1 >> hairstylecode >> bodytypecode >> animpenaltiesstartposcode >> runningcode1 >> preferredposition4 >> volleys >> accessorycolourcode2 >> facialhaircolorcode;
        break;
    case 19:
        line >> firstnameid >> lastnameid >> playerjerseynameid >> commonnameid >> trait2 >> haircolorcode >> facialhairtypecode >> curve >> jerseystylecode >> agility >> accessorycode4 >> gksavetype >> positioning >> tattooleftarm >> hairtypecode >> standingtackle >> tattoobackneck >> preferredposition3 >> longpassing >> penalties >> animfreekickstartposcode >> animpenaltieskickstylecode >> isretiring >> longshots >> gkdiving >> interceptions >> shoecolorcode2 >> crossing >> potential >> gkreflexes >> finishingcode1 >> reactions >> composure >> vision >> contractvaliduntil >> animpenaltiesapproachcode >> finishing >> dribbling >> slidingtackle >> accessorycode3 >> accessorycolourcode1 >> headtypecode >> sprintspeed >> height >> hasseasonaljersey >> preferredposition2 >> strength >> shoetypecode >> birthdate >> preferredposition1 >> ballcontrol >> shotpower >> trait1 >> socklengthcode >> weight >> hashighqualityhead >> gkglovetypecode >> tattoorightarm >> balance >> gender >> headassetid >> gkkicking >> internationalrep >> animpenaltiesmotionstylecode >> shortpassing >> freekickaccuracy >> skillmoves >> faceposerpreset >> usercaneditname >> attackingworkrate >> finishingcode2 >> aggression >> acceleration >> headingaccuracy >> iscustomized >> eyebrowcode >> runningcode2 >> modifier >> gkhandling >> eyecolorcode >> jerseysleevelengthcode >> accessorycolourcode3 >> accessorycode1 >> playerjointeamdate >> headclasscode >> defensiveworkrate >> nationality >> preferredfoot >> sideburnscode >> weakfootabilitytypecode >> jumping >> skintypecode >> tattoorightneck >> gkkickstyle >> stamina >> playerid >> marking >> accessorycolourcode4 >> gkpositioning >> headvariation >> skillmoveslikelihood >> skintonecode >> shortstyle >> overallrating >> tattooleftneck >> emotion >> jerseyfit >> accessorycode2 >> shoedesigncode >> shoecolorcode1 >> hairstylecode >> bodytypecode >> animpenaltiesstartposcode >> runningcode1 >> preferredposition4 >> volleys >> accessorycolourcode2 >> facialhaircolorcode;
        break;
    case 20:
        line >> firstnameid >> lastnameid >> playerjerseynameid >> commonnameid >> skintypecode >> trait2 >> bodytypecode >> haircolorcode >> facialhairtypecode >> curve >> jerseystylecode >> agility >> tattooback >> accessorycode4 >> gksavetype >> positioning >> tattooleftarm >> hairtypecode >> standingtackle >> preferredposition3 >> longpassing >> penalties >> animfreekickstartposcode >> animpenaltieskickstylecode >> isretiring >> longshots >> gkdiving >> interceptions >> shoecolorcode2 >> crossing >> potential >> gkreflexes >> finishingcode1 >> reactions >> composure >> vision >> contractvaliduntil >> animpenaltiesapproachcode >> finishing >> dribbling >> slidingtackle >> accessorycode3 >> accessorycolourcode1 >> headtypecode >> sprintspeed >> height >> hasseasonaljersey >> tattoohead >> preferredposition2 >> strength >> shoetypecode >> birthdate >> preferredposition1 >> tattooleftleg >> ballcontrol >> shotpower >> trait1 >> socklengthcode >> weight >> hashighqualityhead >> gkglovetypecode >> tattoorightarm >> balance >> gender >> headassetid >> gkkicking >> internationalrep >> animpenaltiesmotionstylecode >> shortpassing >> freekickaccuracy >> skillmoves >> faceposerpreset >> usercaneditname >> avatarpomid >> attackingworkrate >> finishingcode2 >> aggression >> acceleration >> headingaccuracy >> iscustomized >> eyebrowcode >> runningcode2 >> modifier >> gkhandling >> eyecolorcode >> jerseysleevelengthcode >> accessorycolourcode3 >> accessorycode1 >> playerjointeamdate >> headclasscode >> defensiveworkrate >> tattoofront >> nationality >> preferredfoot >> sideburnscode >> weakfootabilitytypecode >> jumping >> personality >> gkkickstyle >> stamina >> playerid >> marking >> accessorycolourcode4 >> gkpositioning >> headvariation >> skillmoveslikelihood >> skintonecode >> shortstyle >> overallrating >> smallsidedshoetypecode >> emotion >> runstylecode >> jerseyfit >> accessorycode2 >> shoedesigncode >> shoecolorcode1 >> hairstylecode >> animpenaltiesstartposcode >> runningcode1 >> preferredposition4 >> volleys >> accessorycolourcode2 >> tattoorightleg >> facialhaircolorcode;
        break;
    case 21:
        line >> firstnameid >> lastnameid >> playerjerseynameid >> commonnameid >> skintypecode >> trait2 >> haircolorcode >> facialhairtypecode >> curve >> jerseystylecode >> agility >> tattooback >> accessorycode4 >> gksavetype >> positioning >> tattooleftarm >> hairtypecode >> standingtackle >> preferredposition3 >> longpassing >> penalties >> animfreekickstartposcode >> animpenaltieskickstylecode >> isretiring >> longshots >> gkdiving >> interceptions >> shoecolorcode2 >> crossing >> potential >> gkreflexes >> finishingcode1 >> reactions >> composure >> vision >> contractvaliduntil >> animpenaltiesapproachcode >> finishing >> dribbling >> slidingtackle >> accessorycode3 >> accessorycolourcode1 >> headtypecode >> sprintspeed >> height >> hasseasonaljersey >> tattoohead >> preferredposition2 >> strength >> shoetypecode >> birthdate >> preferredposition1 >> tattooleftleg >> ballcontrol >> shotpower >> trait1 >> socklengthcode >> weight >> hashighqualityhead >> gkglovetypecode >> tattoorightarm >> balance >> gender >> headassetid >> gkkicking >> internationalrep >> animpenaltiesmotionstylecode >> shortpassing >> freekickaccuracy >> skillmoves >> faceposerpreset >> usercaneditname >> avatarpomid >> attackingworkrate >> finishingcode2 >> aggression >> acceleration >> headingaccuracy >> iscustomized >> eyebrowcode >> runningcode2 >> modifier >> gkhandling >> eyecolorcode >> jerseysleevelengthcode >> accessorycolourcode3 >> accessorycode1 >> playerjointeamdate >> headclasscode >> defensiveworkrate >> tattoofront >> nationality >> preferredfoot >> sideburnscode >> weakfootabilitytypecode >> jumping >> personality >> gkkickstyle >> stamina >> playerid >> marking >> accessorycolourcode4 >> gkpositioning >> headvariation >> skillmoveslikelihood >> skintonecode >> shortstyle >> overallrating >> smallsidedshoetypecode >> emotion >> runstylecode >> jerseyfit >> accessorycode2 >> shoedesigncode >> shoecolorcode1 >> hairstylecode >> bodytypecode >> animpenaltiesstartposcode >> runningcode1 >> preferredposition4 >> volleys >> accessorycolourcode2 >> tattoorightleg >> facialhaircolorcode;
        break;
    case 22:
        line >> firstnameid >> lastnameid >> playerjerseynameid >> commonnameid >> skintypecode >> trait2 >> haircolorcode >> facialhairtypecode >> curve >> jerseystylecode >> agility >> tattooback >> accessorycode4 >> gksavetype >> positioning >> tattooleftarm >> hairtypecode >> standingtackle >> preferredposition3 >> longpassing >> penalties >> animfreekickstartposcode >> isretiring >> longshots >> gkdiving >> interceptions >> shoecolorcode2 >> crossing >> potential >> gkreflexes >> finishingcode1 >> reactions >> composure >> vision >> contractvaliduntil >> finishing >> dribbling >> slidingtackle >> accessorycode3 >> accessorycolourcode1 >> headtypecode >> driref >> sprintspeed >> height >> hasseasonaljersey >> tattoohead >> preferredposition2 >> strength >> shoetypecode >> birthdate >> preferredposition1 >> tattooleftleg >> ballcontrol >> phypos >> shotpower >> trait1 >> socklengthcode >> weight >> hashighqualityhead >> gkglovetypecode >> tattoorightarm >> balance >> gender >> headassetid >> gkkicking >> defspe >> internationalrep >> shortpassing >> freekickaccuracy >> skillmoves >> faceposerpreset >> usercaneditname >> avatarpomid >> attackingworkrate >> finishingcode2 >> aggression >> acceleration >> paskic >> headingaccuracy >> iscustomized >> eyebrowcode >> runningcode2 >> modifier >> gkhandling >> eyecolorcode >> jerseysleevelengthcode >> accessorycolourcode3 >> accessorycode1 >> playerjointeamdate >> headclasscode >> defensiveworkrate >> tattoofront >> nationality >> preferredfoot >> sideburnscode >> weakfootabilitytypecode >> jumping >> personality >> gkkickstyle >> stamina >> playerid >> marking >> accessorycolourcode4 >> gkpositioning >> headvariation >> skillmoveslikelihood >> shohan >> skintonecode >> shortstyle >> overallrating >> smallsidedshoetypecode >> emotion >> runstylecode >> jerseyfit >> accessorycode2 >> shoedesigncode >> shoecolorcode1 >> hairstylecode >> bodytypecode >> animpenaltiesstartposcode >> pacdiv >> runningcode1 >> preferredposition4 >> volleys >> accessorycolourcode2 >> tattoorightleg >> facialhaircolorcode;
        break;
    case 23:
        line >> firstnameid >> lastnameid >> playerjerseynameid >> commonnameid >> skintypecode >> trait2 >> haircolorcode >> facialhairtypecode >> curve >> jerseystylecode >> agility >> tattooback >> accessorycode4 >> gksavetype >> positioning >> tattooleftarm >> hairtypecode >> standingtackle >> preferredposition3 >> longpassing >> penalties >> animfreekickstartposcode >> isretiring >> longshots >> gkdiving >> interceptions >> shoecolorcode2 >> crossing >> potential >> gkreflexes >> finishingcode1 >> reactions >> composure >> vision >> contractvaliduntil >> finishing >> dribbling >> slidingtackle >> accessorycode3 >> accessorycolourcode1 >> headtypecode >> driref >> sprintspeed >> height >> hasseasonaljersey >> tattoohead >> preferredposition2 >> strength >> shoetypecode >> birthdate >> preferredposition1 >> tattooleftleg >> ballcontrol >> phypos >> shotpower >> trait1 >> socklengthcode >> weight >> hashighqualityhead >> gkglovetypecode >> tattoorightarm >> balance >> gender >> headassetid >> gkkicking >> defspe >> internationalrep >> shortpassing >> freekickaccuracy >> skillmoves >> faceposerpreset >> usercaneditname >> avatarpomid >> attackingworkrate >> finishingcode2 >> aggression >> acceleration >> paskic >> headingaccuracy >> iscustomized >> eyebrowcode >> runningcode2 >> modifier >> gkhandling >> eyecolorcode >> jerseysleevelengthcode >> accessorycolourcode3 >> accessorycode1 >> playerjointeamdate >> headclasscode >> defensiveworkrate >> tattoofront >> nationality >> preferredfoot >> sideburnscode >> weakfootabilitytypecode >> jumping >> personality >> gkkickstyle >> stamina >> playerid >> accessorycolourcode4 >> gkpositioning >> headvariation >> skillmoveslikelihood >> shohan >> skintonecode >> shortstyle >> overallrating >> smallsidedshoetypecode >> emotion >> runstylecode >> jerseyfit >> accessorycode2 >> shoedesigncode >> shoecolorcode1 >> hairstylecode >> bodytypecode >> animpenaltiesstartposcode >> pacdiv >> defensiveawareness >> runningcode1 >> preferredposition4 >> volleys >> accessorycolourcode2 >> tattoorightleg >> facialhaircolorcode;
        break;
    case 24:
        line >> firstnameid >> lastnameid >> playerjerseynameid >> commonnameid >> gkglovetypecode >> skintypecode >> haircolorcode >> facialhairtypecode >> curve >> jerseystylecode >> agility >> tattooback >> accessorycode4 >> gksavetype >> positioning >> tattooleftarm >> hairtypecode >> standingtackle >> preferredposition3 >> longpassing >> penalties >> animfreekickstartposcode >> isretiring >> longshots >> gkdiving >> icontrait2 >> interceptions >> shoecolorcode2 >> crossing >> potential >> gkreflexes >> finishingcode1 >> reactions >> composure >> vision >> contractvaliduntil >> finishing >> dribbling >> slidingtackle >> accessorycode3 >> accessorycolourcode1 >> headtypecode >> driref >> sprintspeed >> height >> hasseasonaljersey >> tattoohead >> preferredposition2 >> strength >> shoetypecode >> birthdate >> preferredposition1 >> tattooleftleg >> ballcontrol >> phypos >> shotpower >> trait1 >> socklengthcode >> weight >> hashighqualityhead >> tattoorightarm >> icontrait1 >> balance >> gender >> headassetid >> gkkicking >> defspe >> internationalrep >> shortpassing >> freekickaccuracy >> skillmoves >> faceposerpreset >> usercaneditname >> avatarpomid >> attackingworkrate >> finishingcode2 >> aggression >> acceleration >> paskic >> headingaccuracy >> iscustomized >> eyebrowcode >> runningcode2 >> modifier >> gkhandling >> eyecolorcode >> jerseysleevelengthcode >> accessorycolourcode3 >> accessorycode1 >> playerjointeamdate >> headclasscode >> defensiveworkrate >> tattoofront >> nationality >> preferredfoot >> sideburnscode >> weakfootabilitytypecode >> jumping >> personality >> gkkickstyle >> stamina >> playerid >> accessorycolourcode4 >> gkpositioning >> headvariation >> skillmoveslikelihood >> trait2 >> shohan >> skintonecode >> shortstyle >> overallrating >> smallsidedshoetypecode >> emotion >> runstylecode >> muscularitycode >> jerseyfit >> accessorycode2 >> shoedesigncode >> shoecolorcode1 >> hairstylecode >> bodytypecode >> animpenaltiesstartposcode >> pacdiv >> defensiveawareness >> runningcode1 >> preferredposition4 >> volleys >> accessorycolourcode2 >> tattoorightleg >> facialhaircolorcode;
        break;
    }
}

FifaPlayer::FifaPlayer(FifaDataFile::Line &line) {
    internal.Read(line);
    m_loanedFrom = nullptr;
    m_inNationalTeam = false;
    m_linkedWithClub = false;
    m_gameId = 0;
}

bool FifaPlayer::PlayerInTeam(FifaTeam *team) {
    if (m_linkedTeams.size() > 0) {
        for (LinkedTeam &i : m_linkedTeams) {
            if (i.m_team == team)
                return true;
        }
    }
    return false;
}

bool FifaPlayer::IsMale() {
    return internal.gender == 0;
}

FifaTeam *FifaPlayer::GetLeagueTeam() {
    for (LinkedTeam &i : m_linkedTeams) {
        if (i.m_team->m_league && (i.m_team->m_league->IsNationalLeague() || i.m_team->m_league->IsRestOfWorld()))
            return i.m_team;
    }
    return nullptr;
}

FifaTeam *FifaPlayer::GetInternationalTeam() {
    for (LinkedTeam &i : m_linkedTeams) {
        if (i.m_team->m_league && i.m_team->m_league->IsInternational())
            return i.m_team;
    }
    return nullptr;
}
