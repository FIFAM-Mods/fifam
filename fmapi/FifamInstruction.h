#pragma once
#include "FifamInstructionID.h"
#include "FifamReadWrite.h"
#include "FifamNation.h"
#include "FifamContinent.h"
#include "FifamClubTeamType.h"
#include "FifamCompType.h"
#include "FifamCompID.h"

class FifamCompetition;
class FifamDatabase;

// @since FM07
class FifamAbstractInstruction  {
public:
    virtual FifamInstructionID GetID() const = 0;
    virtual FifamAbstractInstruction *Clone() const = 0;
    virtual ~FifamAbstractInstruction() {}
};

class FifamInstruction {
public:
    class BUILD_COUNTER : public FifamAbstractInstruction {
    public:
        UInt mNumAssessmentPositions = 0;
        BUILD_COUNTER(UInt numAssessmentPositions);
        FifamInstructionID GetID() const override;
        BUILD_COUNTER *Clone() const override;
    };

    class RESERVE_ASSESSMENT_TEAMS : public FifamAbstractInstruction {
    public:
        UInt mAssessmentPosition = 0;
        UInt mNumOfReservedSpaces = 0;
        RESERVE_ASSESSMENT_TEAMS(UInt assessmentPosition, UInt numOfReservedSpaces);
        FifamInstructionID GetID() const override;
        RESERVE_ASSESSMENT_TEAMS *Clone() const override;
    };

    class BUILD_UEFA5 : public FifamAbstractInstruction {
    public:
        UInt mAssessmentPosition = 0;
        UInt mNumOfReservedSpaces = 0;
        BUILD_UEFA5(UInt assessmentPosition, UInt numOfReservedSpaces);
        FifamInstructionID GetID() const override;
        BUILD_UEFA5 *Clone() const override;
    };

    class GET_CHAMP : public FifamAbstractInstruction {
    public:
        FifamCompID mCompetition;
        GET_CHAMP(FifamCompID competition);
        FifamInstructionID GetID() const override;
        GET_CHAMP *Clone() const override;
    };

    class GET_EUROPEAN_ASSESSMENT_TEAMS : public FifamAbstractInstruction {
    public:
        UInt mAssessmentPosition = 0;
        UInt mLeagueStartPosition = 0;
        UInt mNumTeams = 0;
        GET_EUROPEAN_ASSESSMENT_TEAMS(UInt assessmentPosition, UInt leagueStartPosition, UInt numTeams);
        FifamInstructionID GetID() const override;
        GET_EUROPEAN_ASSESSMENT_TEAMS *Clone() const override;
    };

    class GET_UEFA5_TAB : public FifamAbstractInstruction {
    public:
        UInt mAssessmentPosition = 0;
        UInt mLeagueStartPosition = 0;
        UInt mNumTeams = 0;
        GET_UEFA5_TAB(UInt assessmentPosition, UInt leagueStartPosition, UInt numTeams);
        FifamInstructionID GetID() const override;
        GET_UEFA5_TAB *Clone() const override;
    };

    class FILL_ASSESSMENT_RESERVES : public FifamAbstractInstruction {
    public:
        FILL_ASSESSMENT_RESERVES();
        FifamInstructionID GetID() const override;
        FILL_ASSESSMENT_RESERVES *Clone() const override;
    };

    class GET_UEFA5_SURE_TAB : public FifamAbstractInstruction {
    public:
        UInt mAssessmentPosition = 0;
        UInt unknown1 = 0;
        GET_UEFA5_SURE_TAB(UInt assessmentPosition, UInt unknown);
        FifamInstructionID GetID() const override;
        GET_UEFA5_SURE_TAB *Clone() const override;
    };

    class GET_CHAMP_OR_RUNNER_UP : public FifamAbstractInstruction {
    public:
        FifamCompID mCompetition;
        GET_CHAMP_OR_RUNNER_UP(FifamCompID competition);
        FifamInstructionID GetID() const override;
        GET_CHAMP_OR_RUNNER_UP *Clone() const override;
    };

    class GET_UEFA5_SURE_UIC : public FifamAbstractInstruction {
    public:
        UInt mAssessmentPosition = 0;
        UInt unknown1 = 0;
        GET_UEFA5_SURE_UIC(UInt assessmentPosition, UInt unknown);
        FifamInstructionID GetID() const override;
        GET_UEFA5_SURE_UIC *Clone() const override;
    };

    class GET_TAB_X_TO_Y : public FifamAbstractInstruction {
    public:
        FifamCompID mLeague;
        UInt mLeagueStartPosition = 0;
        UInt mNumTeams = 0;
        GET_TAB_X_TO_Y(FifamCompID league, UInt leagueStartPosition, UInt numTeams);
        FifamInstructionID GetID() const override;
        GET_TAB_X_TO_Y *Clone() const override;
    };

    class GET_TAB_SURE_X_TO_Y_Z : public FifamAbstractInstruction {
    public:
        FifamCompID mLeague;
        UInt mLeagueStartPosition = 0;
        UInt mLeagueMaxPosition = 0;
        UInt mNumTeams = 0;
        GET_TAB_SURE_X_TO_Y_Z(FifamCompID league, UInt leagueStartPosition, UInt leagueMaxPosition, UInt numTeams);
        FifamInstructionID GetID() const override;
        GET_TAB_SURE_X_TO_Y_Z *Clone() const override;
    };

    class GET_TAB_LEVEL_X_TO_Y : public FifamAbstractInstruction {
    public:
        UInt mLeagueLevel = 0;
        UInt mLeagueStartPosition = 0;
        UInt mNumTeamsFromEachLeague = 0;
        GET_TAB_LEVEL_X_TO_Y(UInt leagueLevel, UInt leagueStartPosition, UInt numTeamsFromEachLeague);
        FifamInstructionID GetID() const override;
        GET_TAB_LEVEL_X_TO_Y *Clone() const override;
    };

    class GET_TAB_SPARE : public FifamAbstractInstruction {
    public:
        GET_TAB_SPARE();
        FifamInstructionID GetID() const override;
        GET_TAB_SPARE *Clone() const override;
    };

    class GET_TAB_LEVEL_START_X_TO_Y : public FifamAbstractInstruction {
    public:
        UInt mLeagueLevel = 0;
        UInt mLeagueStartPosition = 0;
        UInt mNumTeamsFromEachLeague = 0;
        GET_TAB_LEVEL_START_X_TO_Y(UInt leagueLevel, UInt leagueStartPosition, UInt numTeamsFromEachLeague);
        FifamInstructionID GetID() const override;
        GET_TAB_LEVEL_START_X_TO_Y *Clone() const override;
    };

    class GET_EUROPEAN_ASSESSMENT_CUPWINNER : public FifamAbstractInstruction {
    public:
        UInt mAssessmentPosition = 0;
        GET_EUROPEAN_ASSESSMENT_CUPWINNER(UInt assessmentPosition);
        FifamInstructionID GetID() const override;
        GET_EUROPEAN_ASSESSMENT_CUPWINNER *Clone() const override;
    };

    class GET_UEFA5_CHAMP_OR_FINALIST : public FifamAbstractInstruction {
    public:
        UInt mAssessmentPosition = 0;
        FifamCompID mCompetition;
        GET_UEFA5_CHAMP_OR_FINALIST(UInt assessmentPosition, FifamCompID competition);
        FifamInstructionID GetID() const override;
        GET_UEFA5_CHAMP_OR_FINALIST *Clone() const override;
    };

    class GET_WINNER : public FifamAbstractInstruction {
    public:
        FifamCompID mRound;
        GET_WINNER(FifamCompID round);
        FifamInstructionID GetID() const override;
        GET_WINNER *Clone() const override;
    };

    class GET_LOSER : public FifamAbstractInstruction {
    public:
        FifamCompID mRound;
        GET_LOSER(FifamCompID round);
        FifamInstructionID GetID() const override;
        GET_LOSER *Clone() const override;
    };

    class GET_POOL : public FifamAbstractInstruction {
    public:
        FifamCompID mPool;
        UInt mPoolStartPosition = 0;
        UInt mNumTeams = 0;
        GET_POOL(FifamCompID pool, UInt poolStartPosition, UInt numTeams);
        FifamInstructionID GetID() const override;
        GET_POOL *Clone() const override;
    };

    class GET_NAT_UEFA5_WITH_HOST : public FifamAbstractInstruction {
    public:
        UInt mAssessmentPosition = 0;
        GET_NAT_UEFA5_WITH_HOST(UInt assessmentPosition);
        FifamInstructionID GetID() const override;
        GET_NAT_UEFA5_WITH_HOST *Clone() const override;
    };

    class GET_NAT_UEFA5_WITHOUT_HOST : public FifamAbstractInstruction {
    public:
        UInt mAssessmentPosition = 0;
        GET_NAT_UEFA5_WITHOUT_HOST(UInt assessmentPosition);
        FifamInstructionID GetID() const override;
        GET_NAT_UEFA5_WITHOUT_HOST *Clone() const override;
    };

    class GET_NAT_SOUTH_AMERICA : public FifamAbstractInstruction {
    public:
        FifamNation mCountryId;
        GET_NAT_SOUTH_AMERICA(FifamNation countryId);
        FifamInstructionID GetID() const override;
        GET_NAT_SOUTH_AMERICA *Clone() const override;
    };

    class GET_NAT_AMERICA : public FifamAbstractInstruction {
    public:
        GET_NAT_AMERICA();
        FifamInstructionID GetID() const override;
        GET_NAT_AMERICA *Clone() const override;
    };

    class GET_NAT_AFRICA : public FifamAbstractInstruction {
    public:
        GET_NAT_AFRICA();
        FifamInstructionID GetID() const override;
        GET_NAT_AFRICA *Clone() const override;
    };

    class GET_NAT_ASIA : public FifamAbstractInstruction {
    public:
        GET_NAT_ASIA();
        FifamInstructionID GetID() const override;
        GET_NAT_ASIA *Clone() const override;
    };

    class GET_NAT_OCEANIA : public FifamAbstractInstruction {
    public:
        GET_NAT_OCEANIA();
        FifamInstructionID GetID() const override;
        GET_NAT_OCEANIA *Clone() const override;
    };

    class GET_HOST : public FifamAbstractInstruction {
    public:
        GET_HOST();
        FifamInstructionID GetID() const override;
        GET_HOST *Clone() const override;
    };

    class GET_INTERNATIONAL_TAB_LEVEL_X_TO_Y : public FifamAbstractInstruction {
    public:
        FifamNation mCountryId;
        UInt mLeagueLevel = 0;
        UInt mLeagueStartPosition = 0;
        UInt mNumTeamsFromEachLeague = 0;
        GET_INTERNATIONAL_TAB_LEVEL_X_TO_Y(FifamNation countryId, UInt leagueLevel, UInt leagueStartPosition, UInt numTeamsFromEachLeague);
        FifamInstructionID GetID() const override;
        GET_INTERNATIONAL_TAB_LEVEL_X_TO_Y *Clone() const override;
    };

    class GET_INTERNATIONAL_SPARE : public FifamAbstractInstruction {
    public:
        FifamNation mCountryId;
        UInt mNumTeams = 0;
        GET_INTERNATIONAL_SPARE(FifamNation countryId, UInt numTeams);
        FifamInstructionID GetID() const override;
        GET_INTERNATIONAL_SPARE *Clone() const override;
    };

    class GET_RUNNER_UP : public FifamAbstractInstruction {
    public:
        FifamCompID mCompetition;
        GET_RUNNER_UP(FifamCompID competition);
        FifamInstructionID GetID() const override;
        GET_RUNNER_UP *Clone() const override;
    };

    class GET_TAB_LEVEL_INDOOR : public FifamAbstractInstruction {
    public:
        UInt mLeagueLevel = 0;
        UInt mLeagueStartPosition = 0;
        UInt mNumTeamsFromEachLeague = 0;
        GET_TAB_LEVEL_INDOOR(UInt leagueLevel, UInt leagueStartPosition, UInt numTeamsFromEachLeague);
        FifamInstructionID GetID() const override;
        GET_TAB_LEVEL_INDOOR *Clone() const override;
    };

    class GET_RELEGATED_TEAMS : public FifamAbstractInstruction {
    public:
        FifamCompID mLeague;
        GET_RELEGATED_TEAMS(FifamCompID league);
        FifamInstructionID GetID() const override;
        GET_RELEGATED_TEAMS *Clone() const override;
    };

    class GET_INTERNATIONAL_TEAMS : public FifamAbstractInstruction {
    public:
        FifamNation mCountryId;
        UInt mNumTeams = 0;
        GET_INTERNATIONAL_TEAMS(FifamNation countryId, UInt numTeams);
        FifamInstructionID GetID() const override;
        GET_INTERNATIONAL_TEAMS *Clone() const override;
    };

    class GET_CC_FA_WINNER : public FifamAbstractInstruction {
    public:
        FifamCompID mCompetition;
        GET_CC_FA_WINNER(FifamCompID competition);
        FifamInstructionID GetID() const override;
        GET_CC_FA_WINNER *Clone() const override;
    };

    class GET_CC_SPARE : public FifamAbstractInstruction {
    public:
        GET_CC_SPARE();
        FifamInstructionID GetID() const override;
        GET_CC_SPARE *Clone() const override;
    };

    class GET_CHAMP_COUNTRY_TEAM : public FifamAbstractInstruction {
    public:
        FifamCompID mCompetition;
        GET_CHAMP_COUNTRY_TEAM(FifamCompID competition);
        FifamInstructionID GetID() const override;
        GET_CHAMP_COUNTRY_TEAM *Clone() const override;
    };

    class GET_RANDOM_NATIONAL_TEAM : public FifamAbstractInstruction {
    public:
        FifamContinent mContinentId;
        UInt mNumTeams = 0;
        GET_RANDOM_NATIONAL_TEAM(FifamContinent continentId, UInt numTeams);
        FifamInstructionID GetID() const override;
        GET_RANDOM_NATIONAL_TEAM *Clone() const override;
    };

    class CHANGE_TEAM_TYPES : public FifamAbstractInstruction {
    public:
        FifamClubTeamType mNewTeamType;
        CHANGE_TEAM_TYPES(FifamClubTeamType newTeamType);
        FifamInstructionID GetID() const override;
        CHANGE_TEAM_TYPES *Clone() const override;
    };

    class GET_FAIRNESS_TEAM : public FifamAbstractInstruction {
    public:
        UInt mNumTeams = 0;
        GET_FAIRNESS_TEAM(UInt numTeams);
        FifamInstructionID GetID() const override;
        GET_FAIRNESS_TEAM *Clone() const override;
    };

    class COPY_LEAGUE_DATA : public FifamAbstractInstruction {
    public:
        FifamCompID mLeague;
        COPY_LEAGUE_DATA(FifamCompID league);
        FifamInstructionID GetID() const override;
        COPY_LEAGUE_DATA *Clone() const override;
    };

    class GET_NATIONAL_TEAM : public FifamAbstractInstruction {
    public:
        FifamNation mCountryId;
        GET_NATIONAL_TEAM(FifamNation countryId);
        FifamInstructionID GetID() const override;
        GET_NATIONAL_TEAM *Clone() const override;
    };

    class GET_NATIONAL_TEAM_WITHOUT_HOST : public FifamAbstractInstruction {
    public:
        FifamNation mCountryId;
        GET_NATIONAL_TEAM_WITHOUT_HOST(FifamNation countryId);
        FifamInstructionID GetID() const override;
        GET_NATIONAL_TEAM_WITHOUT_HOST *Clone() const override;
    };

    class SHUFFLE_TEAMS : public FifamAbstractInstruction {
    public:
        SHUFFLE_TEAMS();
        FifamInstructionID GetID() const override;
        SHUFFLE_TEAMS *Clone() const override;
    };
};
