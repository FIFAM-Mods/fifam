#include "FifaPlayer.h"
#include "FifaTeam.h"
#include "FifaLeague.h"
#include <sstream>

unsigned int FifaPlayer::GetId() {
    return internal.playerid;
}

void FifaPlayer::Internal::Read(FifaDataFile::Line &line) {
    line >> firstnameid >> lastnameid >> playerjerseynameid >> commonnameid >> trait2 >> haircolorcode >> facialhairtypecode >> curve >> jerseystylecode >> agility >> accessorycode4 >> gksavetype >> positioning >> tattooleftarm >> hairtypecode >> standingtackle >> tattoobackneck >> preferredposition3 >> longpassing >> penalties >> animfreekickstartposcode >> animpenaltieskickstylecode >> isretiring >> longshots >> gkdiving >> interceptions >> shoecolorcode2 >> crossing >> potential >> gkreflexes >> finishingcode1 >> reactions >> composure >> vision >> contractvaliduntil >> animpenaltiesapproachcode >> finishing >> dribbling >> slidingtackle >> accessorycode3 >> accessorycolourcode1 >> headtypecode >> sprintspeed >> height >> hasseasonaljersey >> preferredposition2 >> strength >> shoetypecode >> birthdate >> preferredposition1 >> ballcontrol >> shotpower >> trait1 >> socklengthcode >> weight >> hashighqualityhead >> gkglovetypecode >> tattoorightarm >> balance >> gender >> gkkicking >> internationalrep >> animpenaltiesmotionstylecode >> shortpassing >> freekickaccuracy >> skillmoves >> usercaneditname >> attackingworkrate >> finishingcode2 >> aggression >> acceleration >> headingaccuracy >> iscustomized >> eyebrowcode >> runningcode2 >> modifier >> gkhandling >> eyecolorcode >> jerseysleevelengthcode >> accessorycolourcode3 >> accessorycode1 >> playerjointeamdate >> headclasscode >> defensiveworkrate >> nationality >> preferredfoot >> sideburnscode >> weakfootabilitytypecode >> jumping >> skintypecode >> tattoorightneck >> gkkickstyle >> stamina >> playerid >> marking >> accessorycolourcode4 >> gkpositioning >> headvariation >> skintonecode >> shortstyle >> overallrating >> tattooleftneck >> emotion >> jerseyfit >> accessorycode2 >> shoedesigncode >> shoecolorcode1 >> hairstylecode >> bodytypecode >> animpenaltiesstartposcode >> runningcode1 >> preferredposition4 >> volleys >> accessorycolourcode2 >> facialhaircolorcode;
}

FifaPlayer::FifaPlayer(FifaDataFile::Line &line) {
    internal.Read(line);
    m_loanedFrom = nullptr;
    m_inNationalTeam = false;
    m_linkedWithClub = false;
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