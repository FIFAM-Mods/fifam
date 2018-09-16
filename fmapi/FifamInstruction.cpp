#include "FifamInstruction.h"

FifamInstruction::BUILD_COUNTER::BUILD_COUNTER(UInt numAssessmentPositions) {
    mNumAssessmentPositions = numAssessmentPositions;
}

FifamInstructionID FifamInstruction::BUILD_COUNTER::GetID() const {
    return FifamInstructionID::ID_BUILD_COUNTER;
}

FifamInstruction::BUILD_COUNTER *FifamInstruction::BUILD_COUNTER::Clone() const {
    return new BUILD_COUNTER(mNumAssessmentPositions);
}

FifamInstruction::RESERVE_ASSESSMENT_TEAMS::RESERVE_ASSESSMENT_TEAMS(UInt assessmentPosition, UInt numOfReservedSpaces) {
    mAssessmentPosition = assessmentPosition;
    mNumOfReservedSpaces = numOfReservedSpaces;
}

FifamInstructionID FifamInstruction::RESERVE_ASSESSMENT_TEAMS::GetID() const {
    return FifamInstructionID::ID_RESERVE_ASSESSMENT_TEAMS;
}

FifamInstruction::RESERVE_ASSESSMENT_TEAMS *FifamInstruction::RESERVE_ASSESSMENT_TEAMS::Clone() const {
    return new RESERVE_ASSESSMENT_TEAMS(mAssessmentPosition, mNumOfReservedSpaces);
}

FifamInstruction::BUILD_UEFA5::BUILD_UEFA5(UInt assessmentPosition, UInt numOfReservedSpaces) {
    mAssessmentPosition = assessmentPosition;
    mNumOfReservedSpaces = numOfReservedSpaces;
}

FifamInstructionID FifamInstruction::BUILD_UEFA5::GetID() const {
    return FifamInstructionID::ID_BUILD_UEFA5;
}

FifamInstruction::BUILD_UEFA5 *FifamInstruction::BUILD_UEFA5::Clone() const {
    return new BUILD_UEFA5(mAssessmentPosition, mNumOfReservedSpaces);
}

FifamInstruction::GET_CHAMP::GET_CHAMP(FifamCompetition *competition) {
    mCompetition = competition;
}

FifamInstructionID FifamInstruction::GET_CHAMP::GetID() const {
    return FifamInstructionID::ID_GET_CHAMP;
}

FifamInstruction::GET_CHAMP *FifamInstruction::GET_CHAMP::Clone() const {
    return new GET_CHAMP(mCompetition);
}

FifamInstruction::GET_EUROPEAN_ASSESSMENT_TEAMS::GET_EUROPEAN_ASSESSMENT_TEAMS(UInt assessmentPosition, UInt leagueStartPosition, UInt numTeams) {
    mAssessmentPosition = assessmentPosition;
    mLeagueStartPosition = leagueStartPosition;
    mNumTeams = numTeams;
}

FifamInstructionID FifamInstruction::GET_EUROPEAN_ASSESSMENT_TEAMS::GetID() const {
    return FifamInstructionID::ID_GET_EUROPEAN_ASSESSMENT_TEAMS;
}

FifamInstruction::GET_EUROPEAN_ASSESSMENT_TEAMS *FifamInstruction::GET_EUROPEAN_ASSESSMENT_TEAMS::Clone() const {
    return new GET_EUROPEAN_ASSESSMENT_TEAMS(mAssessmentPosition, mLeagueStartPosition, mNumTeams);
}

FifamInstruction::GET_UEFA5_TAB::GET_UEFA5_TAB(UInt assessmentPosition, UInt leagueStartPosition, UInt numTeams) {
    mAssessmentPosition = assessmentPosition;
    mLeagueStartPosition = leagueStartPosition;
    mNumTeams = numTeams;
}

FifamInstructionID FifamInstruction::GET_UEFA5_TAB::GetID() const {
    return FifamInstructionID::ID_GET_UEFA5_TAB;
}

FifamInstruction::GET_UEFA5_TAB *FifamInstruction::GET_UEFA5_TAB::Clone() const {
    return new GET_UEFA5_TAB(mAssessmentPosition, mLeagueStartPosition, mNumTeams);
}

FifamInstruction::FILL_ASSESSMENT_RESERVES::FILL_ASSESSMENT_RESERVES() {

}

FifamInstructionID FifamInstruction::FILL_ASSESSMENT_RESERVES::GetID() const {
    return FifamInstructionID::ID_FILL_ASSESSMENT_RESERVES;
}

FifamInstruction::FILL_ASSESSMENT_RESERVES *FifamInstruction::FILL_ASSESSMENT_RESERVES::Clone() const {
    return new FILL_ASSESSMENT_RESERVES();
}

FifamInstruction::GET_UEFA5_SURE_TAB::GET_UEFA5_SURE_TAB(UInt assessmentPosition, UInt unknown) {
    mAssessmentPosition = assessmentPosition;
    unknown1 = unknown;
}

FifamInstructionID FifamInstruction::GET_UEFA5_SURE_TAB::GetID() const {
    return FifamInstructionID::ID_GET_UEFA5_SURE_TAB;
}

FifamInstruction::GET_UEFA5_SURE_TAB *FifamInstruction::GET_UEFA5_SURE_TAB::Clone() const {
    return new GET_UEFA5_SURE_TAB(mAssessmentPosition, unknown1);
}

FifamInstruction::GET_CHAMP_OR_RUNNER_UP::GET_CHAMP_OR_RUNNER_UP(FifamCompetition *competition) {
    mCompetition = competition;
}

FifamInstructionID FifamInstruction::GET_CHAMP_OR_RUNNER_UP::GetID() const {
    return FifamInstructionID::ID_GET_CHAMP_OR_RUNNER_UP;
}

FifamInstruction::GET_CHAMP_OR_RUNNER_UP *FifamInstruction::GET_CHAMP_OR_RUNNER_UP::Clone() const {
    return new GET_CHAMP_OR_RUNNER_UP(mCompetition);
}

FifamInstruction::GET_UEFA5_SURE_UIC::GET_UEFA5_SURE_UIC(UInt assessmentPosition, UInt unknown) {
    mAssessmentPosition = assessmentPosition;
    unknown1 = unknown;
}

FifamInstructionID FifamInstruction::GET_UEFA5_SURE_UIC::GetID() const {
    return FifamInstructionID::ID_GET_UEFA5_SURE_UIC;
}

FifamInstruction::GET_UEFA5_SURE_UIC *FifamInstruction::GET_UEFA5_SURE_UIC::Clone() const {
    return new GET_UEFA5_SURE_UIC(mAssessmentPosition, unknown1);
}

FifamInstruction::GET_TAB_X_TO_Y::GET_TAB_X_TO_Y(FifamCompetition *league, UInt leagueStartPosition, UInt numTeams) {
    mLeague = league;
    mLeagueStartPosition = leagueStartPosition;
    mNumTeams = numTeams;
}

FifamInstructionID FifamInstruction::GET_TAB_X_TO_Y::GetID() const {
    return FifamInstructionID::ID_GET_TAB_X_TO_Y;
}

FifamInstruction::GET_TAB_X_TO_Y *FifamInstruction::GET_TAB_X_TO_Y::Clone() const {
    return new GET_TAB_X_TO_Y(mLeague, mLeagueStartPosition, mNumTeams);
}

FifamInstruction::GET_TAB_SURE_X_TO_Y_Z::GET_TAB_SURE_X_TO_Y_Z(FifamCompetition *league, UInt leagueStartPosition, UInt leagueMaxPosition, UInt numTeams) {
    mLeague = league;
    mLeagueStartPosition = leagueStartPosition;
    mLeagueMaxPosition = leagueMaxPosition;
    mNumTeams = numTeams;
}

FifamInstructionID FifamInstruction::GET_TAB_SURE_X_TO_Y_Z::GetID() const {
    return FifamInstructionID::ID_GET_TAB_SURE_X_TO_Y_Z;
}

FifamInstruction::GET_TAB_SURE_X_TO_Y_Z *FifamInstruction::GET_TAB_SURE_X_TO_Y_Z::Clone() const {
    return new GET_TAB_SURE_X_TO_Y_Z(mLeague, mLeagueStartPosition, mLeagueMaxPosition, mNumTeams);
}

FifamInstruction::GET_TAB_LEVEL_X_TO_Y::GET_TAB_LEVEL_X_TO_Y(UInt leagueLevel, UInt leagueStartPosition, UInt numTeamsFromEachLeague) {
    mLeagueLevel = leagueLevel;
    mLeagueStartPosition = leagueStartPosition;
    mNumTeamsFromEachLeague = numTeamsFromEachLeague;
}

FifamInstructionID FifamInstruction::GET_TAB_LEVEL_X_TO_Y::GetID() const {
    return FifamInstructionID::ID_GET_TAB_LEVEL_X_TO_Y;
}

FifamInstruction::GET_TAB_LEVEL_X_TO_Y *FifamInstruction::GET_TAB_LEVEL_X_TO_Y::Clone() const {
    return new GET_TAB_LEVEL_X_TO_Y(mLeagueLevel, mLeagueStartPosition, mNumTeamsFromEachLeague);
}

FifamInstruction::GET_TAB_SPARE::GET_TAB_SPARE() {

}

FifamInstructionID FifamInstruction::GET_TAB_SPARE::GetID() const {
    return FifamInstructionID::ID_GET_TAB_SPARE;
}

FifamInstruction::GET_TAB_SPARE *FifamInstruction::GET_TAB_SPARE::Clone() const {
    return new GET_TAB_SPARE();
}

FifamInstruction::GET_TAB_LEVEL_START_X_TO_Y::GET_TAB_LEVEL_START_X_TO_Y(UInt leagueLevel, UInt leagueStartPosition, UInt numTeamsFromEachLeague) {
    mLeagueLevel = leagueLevel;
    mLeagueStartPosition = leagueStartPosition;
    mNumTeamsFromEachLeague = numTeamsFromEachLeague;
}

FifamInstructionID FifamInstruction::GET_TAB_LEVEL_START_X_TO_Y::GetID() const {
    return FifamInstructionID::ID_GET_TAB_LEVEL_START_X_TO_Y;
}

FifamInstruction::GET_TAB_LEVEL_START_X_TO_Y *FifamInstruction::GET_TAB_LEVEL_START_X_TO_Y::Clone() const {
    return new GET_TAB_LEVEL_START_X_TO_Y(mLeagueLevel, mLeagueStartPosition, mNumTeamsFromEachLeague);
}

FifamInstruction::GET_EUROPEAN_ASSESSMENT_CUPWINNER::GET_EUROPEAN_ASSESSMENT_CUPWINNER(UInt assessmentPosition) {
    mAssessmentPosition = assessmentPosition;
}

FifamInstructionID FifamInstruction::GET_EUROPEAN_ASSESSMENT_CUPWINNER::GetID() const {
    return FifamInstructionID::ID_GET_EUROPEAN_ASSESSMENT_CUPWINNER;
}

FifamInstruction::GET_EUROPEAN_ASSESSMENT_CUPWINNER *FifamInstruction::GET_EUROPEAN_ASSESSMENT_CUPWINNER::Clone() const {
    return new GET_EUROPEAN_ASSESSMENT_CUPWINNER(mAssessmentPosition);
}

FifamInstruction::GET_UEFA5_CHAMP_OR_FINALIST::GET_UEFA5_CHAMP_OR_FINALIST(UInt assessmentPosition, FifamCompType compType, UInt compIndex) {
    mAssessmentPosition = assessmentPosition;
    mCompType = compType;
    mCompIndex = compIndex;
}

FifamInstructionID FifamInstruction::GET_UEFA5_CHAMP_OR_FINALIST::GetID() const {
    return FifamInstructionID::ID_GET_UEFA5_CHAMP_OR_FINALIST;
}

FifamInstruction::GET_UEFA5_CHAMP_OR_FINALIST *FifamInstruction::GET_UEFA5_CHAMP_OR_FINALIST::Clone() const {
    return new GET_UEFA5_CHAMP_OR_FINALIST(mAssessmentPosition, mCompType, mCompIndex);
}

FifamInstruction::GET_WINNER::GET_WINNER(FifamCompetition *round) {
    mRound = round;
}

FifamInstructionID FifamInstruction::GET_WINNER::GetID() const {
    return FifamInstructionID::ID_GET_WINNER;
}

FifamInstruction::GET_WINNER *FifamInstruction::GET_WINNER::Clone() const {
    return new GET_WINNER(mRound);
}

FifamInstruction::GET_LOSER::GET_LOSER(FifamCompetition *round) {
    mRound = round;
}

FifamInstructionID FifamInstruction::GET_LOSER::GetID() const {
    return FifamInstructionID::ID_GET_LOSER;
}

FifamInstruction::GET_LOSER *FifamInstruction::GET_LOSER::Clone() const {
    return new GET_LOSER(mRound);
}

FifamInstruction::GET_POOL::GET_POOL(FifamCompetition *pool, UInt poolStartPosition, UInt numTeams) {
    mPool = pool;
    mPoolStartPosition = poolStartPosition;
    mNumTeams = numTeams;
}

FifamInstructionID FifamInstruction::GET_POOL::GetID() const {
    return FifamInstructionID::ID_GET_POOL;
}

FifamInstruction::GET_POOL *FifamInstruction::GET_POOL::Clone() const {
    return new GET_POOL(mPool, mPoolStartPosition, mNumTeams);
}

FifamInstruction::GET_NAT_UEFA5_WITH_HOST::GET_NAT_UEFA5_WITH_HOST(UInt assessmentPosition) {
    mAssessmentPosition = assessmentPosition;
}

FifamInstructionID FifamInstruction::GET_NAT_UEFA5_WITH_HOST::GetID() const {
    return FifamInstructionID::ID_GET_NAT_UEFA5_WITH_HOST;
}

FifamInstruction::GET_NAT_UEFA5_WITH_HOST *FifamInstruction::GET_NAT_UEFA5_WITH_HOST::Clone() const {
    return new GET_NAT_UEFA5_WITH_HOST(mAssessmentPosition);
}

FifamInstruction::GET_NAT_UEFA5_WITHOUT_HOST::GET_NAT_UEFA5_WITHOUT_HOST(UInt assessmentPosition) {
    mAssessmentPosition = assessmentPosition;
}

FifamInstructionID FifamInstruction::GET_NAT_UEFA5_WITHOUT_HOST::GetID() const {
    return FifamInstructionID::ID_GET_NAT_UEFA5_WITHOUT_HOST;
}

FifamInstruction::GET_NAT_UEFA5_WITHOUT_HOST *FifamInstruction::GET_NAT_UEFA5_WITHOUT_HOST::Clone() const {
    return new GET_NAT_UEFA5_WITHOUT_HOST(mAssessmentPosition);
}

FifamInstruction::GET_NAT_SOUTH_AMERICA::GET_NAT_SOUTH_AMERICA(FifamNation countryId) {
    mCountryId = countryId;
}

FifamInstructionID FifamInstruction::GET_NAT_SOUTH_AMERICA::GetID() const {
    return FifamInstructionID::ID_GET_NAT_SOUTH_AMERICA;
}

FifamInstruction::GET_NAT_SOUTH_AMERICA *FifamInstruction::GET_NAT_SOUTH_AMERICA::Clone() const {
    return new GET_NAT_SOUTH_AMERICA(mCountryId);
}

FifamInstruction::GET_NAT_AMERICA::GET_NAT_AMERICA() {

}

FifamInstructionID FifamInstruction::GET_NAT_AMERICA::GetID() const {
    return FifamInstructionID::ID_GET_NAT_AMERICA;
}

FifamInstruction::GET_NAT_AMERICA *FifamInstruction::GET_NAT_AMERICA::Clone() const {
    return new GET_NAT_AMERICA();
}

FifamInstruction::GET_NAT_AFRICA::GET_NAT_AFRICA() {

}

FifamInstructionID FifamInstruction::GET_NAT_AFRICA::GetID() const {
    return FifamInstructionID::ID_GET_NAT_AFRICA;
}

FifamInstruction::GET_NAT_AFRICA *FifamInstruction::GET_NAT_AFRICA::Clone() const {
    return new GET_NAT_AFRICA();
}

FifamInstruction::GET_NAT_ASIA::GET_NAT_ASIA() {

}

FifamInstructionID FifamInstruction::GET_NAT_ASIA::GetID() const {
    return FifamInstructionID::ID_GET_NAT_ASIA;
}

FifamInstruction::GET_NAT_ASIA *FifamInstruction::GET_NAT_ASIA::Clone() const {
    return new GET_NAT_ASIA();
}

FifamInstruction::GET_NAT_OCEANIA::GET_NAT_OCEANIA() {

}

FifamInstructionID FifamInstruction::GET_NAT_OCEANIA::GetID() const {
    return FifamInstructionID::ID_GET_NAT_OCEANIA;
}

FifamInstruction::GET_NAT_OCEANIA *FifamInstruction::GET_NAT_OCEANIA::Clone() const {
    return new GET_NAT_OCEANIA();
}

FifamInstruction::GET_HOST::GET_HOST() {

}

FifamInstructionID FifamInstruction::GET_HOST::GetID() const {
    return FifamInstructionID::ID_GET_HOST;
}

FifamInstruction::GET_HOST *FifamInstruction::GET_HOST::Clone() const {
    return new GET_HOST();
}

FifamInstruction::GET_INTERNATIONAL_TAB_LEVEL_X_TO_Y::GET_INTERNATIONAL_TAB_LEVEL_X_TO_Y(FifamNation countryId, UInt leagueLevel, UInt leagueStartPosition, UInt numTeamsFromEachLeague) {
    mCountryId = countryId;
    mLeagueLevel = leagueLevel;
    mLeagueStartPosition = leagueStartPosition;
    mNumTeamsFromEachLeague = numTeamsFromEachLeague;
}

FifamInstructionID FifamInstruction::GET_INTERNATIONAL_TAB_LEVEL_X_TO_Y::GetID() const {
    return FifamInstructionID::ID_GET_INTERNATIONAL_TAB_LEVEL_X_TO_Y;
}

FifamInstruction::GET_INTERNATIONAL_TAB_LEVEL_X_TO_Y *FifamInstruction::GET_INTERNATIONAL_TAB_LEVEL_X_TO_Y::Clone() const {
    return new GET_INTERNATIONAL_TAB_LEVEL_X_TO_Y(mCountryId, mLeagueLevel, mLeagueStartPosition, mNumTeamsFromEachLeague);
}

FifamInstruction::GET_INTERNATIONAL_SPARE::GET_INTERNATIONAL_SPARE(FifamNation countryId, UInt numTeams) {
    mCountryId = countryId;
    mNumTeams = numTeams;
}

FifamInstructionID FifamInstruction::GET_INTERNATIONAL_SPARE::GetID() const {
    return FifamInstructionID::ID_GET_INTERNATIONAL_SPARE;
}

FifamInstruction::GET_INTERNATIONAL_SPARE *FifamInstruction::GET_INTERNATIONAL_SPARE::Clone() const {
    return new GET_INTERNATIONAL_SPARE(mCountryId, mNumTeams);
}

FifamInstruction::GET_RUNNER_UP::GET_RUNNER_UP(FifamCompetition *competition) {
    mCompetition = competition;
}

FifamInstructionID FifamInstruction::GET_RUNNER_UP::GetID() const {
    return FifamInstructionID::ID_GET_RUNNER_UP;
}

FifamInstruction::GET_RUNNER_UP *FifamInstruction::GET_RUNNER_UP::Clone() const {
    return new GET_RUNNER_UP(mCompetition);
}

FifamInstruction::GET_TAB_LEVEL_INDOOR::GET_TAB_LEVEL_INDOOR(UInt leagueLevel, UInt leagueStartPosition, UInt numTeamsFromEachLeague) {
    mLeagueLevel = leagueLevel;
    mLeagueStartPosition = leagueStartPosition;
    mNumTeamsFromEachLeague = numTeamsFromEachLeague;
}

FifamInstructionID FifamInstruction::GET_TAB_LEVEL_INDOOR::GetID() const {
    return FifamInstructionID::ID_GET_TAB_LEVEL_INDOOR;
}

FifamInstruction::GET_TAB_LEVEL_INDOOR *FifamInstruction::GET_TAB_LEVEL_INDOOR::Clone() const {
    return new GET_TAB_LEVEL_INDOOR(mLeagueLevel, mLeagueStartPosition, mNumTeamsFromEachLeague);
}

FifamInstruction::GET_RELEGATED_TEAMS::GET_RELEGATED_TEAMS(FifamCompetition *league) {
    mLeague = league;
}

FifamInstructionID FifamInstruction::GET_RELEGATED_TEAMS::GetID() const {
    return FifamInstructionID::ID_GET_RELEGATED_TEAMS;
}

FifamInstruction::GET_RELEGATED_TEAMS *FifamInstruction::GET_RELEGATED_TEAMS::Clone() const {
    return new GET_RELEGATED_TEAMS(mLeague);
}

FifamInstruction::GET_INTERNATIONAL_TEAMS::GET_INTERNATIONAL_TEAMS(FifamNation countryId, UInt numTeams) {
    mCountryId = countryId;
    mNumTeams = numTeams;
}

FifamInstructionID FifamInstruction::GET_INTERNATIONAL_TEAMS::GetID() const {
    return FifamInstructionID::ID_GET_INTERNATIONAL_TEAMS;
}

FifamInstruction::GET_INTERNATIONAL_TEAMS *FifamInstruction::GET_INTERNATIONAL_TEAMS::Clone() const {
    return new GET_INTERNATIONAL_TEAMS(mCountryId, mNumTeams);
}

FifamInstruction::GET_CC_FA_WINNER::GET_CC_FA_WINNER(FifamCompetition *competition) {
    mCompetition = competition;
}

FifamInstructionID FifamInstruction::GET_CC_FA_WINNER::GetID() const {
    return FifamInstructionID::ID_GET_CC_FA_WINNER;
}

FifamInstruction::GET_CC_FA_WINNER *FifamInstruction::GET_CC_FA_WINNER::Clone() const {
    return new GET_CC_FA_WINNER(mCompetition);
}

FifamInstruction::GET_CC_SPARE::GET_CC_SPARE() {

}

FifamInstructionID FifamInstruction::GET_CC_SPARE::GetID() const {
    return FifamInstructionID::ID_GET_CC_SPARE;
}

FifamInstruction::GET_CC_SPARE *FifamInstruction::GET_CC_SPARE::Clone() const {
    return new GET_CC_SPARE();
}

FifamInstruction::GET_CHAMP_COUNTRY_TEAM::GET_CHAMP_COUNTRY_TEAM(FifamCompetition *competition) {
    mCompetition = competition;
}

FifamInstructionID FifamInstruction::GET_CHAMP_COUNTRY_TEAM::GetID() const {
    return FifamInstructionID::ID_GET_CHAMP_COUNTRY_TEAM;
}

FifamInstruction::GET_CHAMP_COUNTRY_TEAM *FifamInstruction::GET_CHAMP_COUNTRY_TEAM::Clone() const {
    return new GET_CHAMP_COUNTRY_TEAM(mCompetition);
}

FifamInstruction::GET_RANDOM_NATIONAL_TEAM::GET_RANDOM_NATIONAL_TEAM(FifamContinent continentId, UInt numTeams) {
    mContinentId = continentId;
    mNumTeams = numTeams;
}

FifamInstructionID FifamInstruction::GET_RANDOM_NATIONAL_TEAM::GetID() const {
    return FifamInstructionID::ID_GET_RANDOM_NATIONAL_TEAM;
}

FifamInstruction::GET_RANDOM_NATIONAL_TEAM *FifamInstruction::GET_RANDOM_NATIONAL_TEAM::Clone() const {
    return new GET_RANDOM_NATIONAL_TEAM(mContinentId, mNumTeams);
}

FifamInstruction::CHANGE_TEAM_TYPES::CHANGE_TEAM_TYPES(FifamClubTeamType newTeamType) {
    mNewTeamType = newTeamType;
}

FifamInstructionID FifamInstruction::CHANGE_TEAM_TYPES::GetID() const {
    return FifamInstructionID::ID_CHANGE_TEAM_TYPES;
}

FifamInstruction::CHANGE_TEAM_TYPES *FifamInstruction::CHANGE_TEAM_TYPES::Clone() const {
    return new CHANGE_TEAM_TYPES(mNewTeamType);
}

FifamInstruction::GET_FAIRNESS_TEAM::GET_FAIRNESS_TEAM(UInt numTeams) {
    mNumTeams = numTeams;
}

FifamInstructionID FifamInstruction::GET_FAIRNESS_TEAM::GetID() const {
    return FifamInstructionID::ID_GET_FAIRNESS_TEAM;
}

FifamInstruction::GET_FAIRNESS_TEAM *FifamInstruction::GET_FAIRNESS_TEAM::Clone() const {
    return new GET_FAIRNESS_TEAM(mNumTeams);
}

FifamInstruction::COPY_LEAGUE_DATA::COPY_LEAGUE_DATA(FifamCompetition *league) {
    mLeague = league;
}

FifamInstructionID FifamInstruction::COPY_LEAGUE_DATA::GetID() const {
    return FifamInstructionID::ID_COPY_LEAGUE_DATA;
}

FifamInstruction::COPY_LEAGUE_DATA *FifamInstruction::COPY_LEAGUE_DATA::Clone() const {
    return new COPY_LEAGUE_DATA(mLeague);
}

FifamInstruction::GET_NATIONAL_TEAM::GET_NATIONAL_TEAM(FifamNation countryId) {
    mCountryId = countryId;
}

FifamInstructionID FifamInstruction::GET_NATIONAL_TEAM::GetID() const {
    return FifamInstructionID::ID_GET_NATIONAL_TEAM;
}

FifamInstruction::GET_NATIONAL_TEAM *FifamInstruction::GET_NATIONAL_TEAM::Clone() const {
    return new GET_NATIONAL_TEAM(mCountryId);
}

FifamInstruction::GET_NATIONAL_TEAM_WITHOUT_HOST::GET_NATIONAL_TEAM_WITHOUT_HOST(FifamNation countryId) {
    mCountryId = countryId;
}

FifamInstructionID FifamInstruction::GET_NATIONAL_TEAM_WITHOUT_HOST::GetID() const {
    return FifamInstructionID::ID_GET_NATIONAL_TEAM_WITHOUT_HOST;
}

FifamInstruction::GET_NATIONAL_TEAM_WITHOUT_HOST *FifamInstruction::GET_NATIONAL_TEAM_WITHOUT_HOST::Clone() const {
    return new GET_NATIONAL_TEAM_WITHOUT_HOST(mCountryId);
}

FifamInstruction::SHUFFLE_TEAMS::SHUFFLE_TEAMS() {

}

FifamInstructionID FifamInstruction::SHUFFLE_TEAMS::GetID() const {
    return FifamInstructionID::ID_SHUFFLE_TEAMS;
}

FifamInstruction::SHUFFLE_TEAMS *FifamInstruction::SHUFFLE_TEAMS::Clone() const {
    return new SHUFFLE_TEAMS();
}
