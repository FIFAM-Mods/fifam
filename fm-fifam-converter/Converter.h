#pragma once
#include "FifamDatabase.h"
#include "foom_db.h"
#include "FifaDatabase.h"
#include "AppearanceGenerator.h"
#include "FifamCompLeague.h"
#include "FifamCompCup.h"
#include "FifamCompRound.h"

const UShort CURRENT_YEAR = 2018;

class Converter {
public:
    FifamDatabase *mReferenceDatabase = nullptr;
    FifamDatabase *mFifamDatabase = nullptr;
    FifaDatabase *mFifaDatabase = nullptr;
    UInt mCurrentGameId = FifamDatabase::LATEST_GAME_VERSION;
    foom::db *mFoomDatabase = nullptr;

    struct DivisionInfo {
        enum Type {
            League,
            Level,
        };

        Int mNationID = -1;
        String mName;
        String mShortName;
        Int mID = -1;
        Type mType = League;
        Int mLevel = 0;
        Int mTeams = 0;
        Int mRep = 0;
        Int mOrder = 0;
        Int mPriority = 0;
        Int mRounds = -1;
        Int mPromoted = 0;
        Int mRelegated = 0;
        Int mStartDate = 0;
        Int mEndDate = 0;
        Int mWinterBreakStart = 0;
        Int mWinterBreakEnd = 0;
        Int mNumSubs = 3;
        Int mForeignersLimit = 0;
        Int mNonEuSigns = 0;
        Int mDomesticPlayers = 0;
        Int mU21Players = 0;
        Int mReserveTeamsAllowed = -1;
        FifamEqualPointsSorting mSorting;
        Int mTvBonus = -1;
        Int mWinBouns = -1;
        Int mPlaceBonus = -1;
        Int mAttendanceMp = 0;
        Int mTransfersMp = 0;
        FifamCompID mCompID;
    };

    struct CupInfo {
        enum Type {
            FA = 0,
            League = 1,
            Supercup = 2
        };
        Int mNationID = -1;
        String mName;
        String mShortName;
        Int mID = -1;
        Int mLevel = 0;
        Type mType = League;
        Int mReputation = 0;
        Int mPriority = 0;
        FifamCupSystemType mTemplateType;
        Array<String, 8> mRoundDesc;
        String mStructure;
        Int mMaxLevel = 0;
        Int mNumSubs = 0;
        Int mBonus = 0;
        Int mTvBonus = 0;
        Int mStartDate = 0;
        Int mEndDate = 0;
    };

    struct FixtureInfo {
        Date mDate;
        foom::club *mTeam1 = 0;
        foom::club *mTeam2 = 0;
    };

    struct ClubColor {
        enum ClubColorType {
            DEFAULT,
            TEAM,
            KIT,
            RGB
        };
        ClubColorType type = ClubColorType::DEFAULT;
        Array<Color, 2> rgb;
    };

    Vector<DivisionInfo> mDivisions;
    Vector<CupInfo> mCups;
    Map<UInt, Vector<FixtureInfo>> mFixturesPerLeague;
    Array<UInt, FifamDatabase::NUM_COUNTRIES> mNextFreeUID = {};
    Array<Map<UInt, Vector<FifamCompLeague *>>, FifamDatabase::NUM_COUNTRIES> mLeaguesSystem;
    Array<UInt, FifamDatabase::NUM_COUNTRIES> mNumTeamsInLeagueSystem = {};
    Array<Vector<foom::player *>, FifamDatabase::NUM_COUNTRIES> mNationalTeamPlayers;
    UInt mPersonIdCounter = 1;
    Map<Int, Path> mAvailableBadges;
    AppearanceGenerator appearanceGenerator;
    Map<Int, ClubColor> mClubColorsMap;
    
    Date GetCurrentDate() { return Date(1, 7, CURRENT_YEAR); }
    Date FmEmptyDate() { return Date(1, 1, 1900); }

    void ReadAdditionalInfo(Path const &infoPath);
    void Convert(UInt gameId, Bool writeToGameFolder);
    void Convert(UInt gameId, UInt originalGameId, Path const &originalDb, UInt referenceGameId, Path const &referenceDb, Bool writeToGameFolder);
    ~Converter();

    Int ConvertPlayerAttribute(Int attr);
    void ConvertNationInfo(FifamCountry *dst, foom::nation *nation, UInt gameId);
    void ConvertClub(UInt gameId, FifamClub *dst, foom::club *team, foom::club *mainTeam, FifamCountry *country, DivisionInfo *div);
    void ConvertClubStadium(FifamClub *dst, UInt gameId);
    FifamClub *CreateAndConvertClub(UInt gameId, foom::club *team, foom::club *mainTeam, FifamCountry *country, DivisionInfo *div, bool convertSquad);
    void ConvertReferee(FifamReferee *dst, foom::official *official);
    void ConvertKitsAndColors(FifamClub *dst, Int foomId, Vector<foom::kit> const &kits, Int badgeType, Color const &teamBackgroundColor,
        Color const &teamForegroundColor, UInt gameId);
    FifamPlayer *CreateAndConvertPlayer(UInt gameId, foom::player *p, FifamClub *club);
    FifamStaff *CreateAndConvertStaff(foom::non_player *p, FifamClub *club, FifamClubStaffPosition position);
    void ConvertPersonAttributes(FifamPerson *person, foom::person *p);
    void CreateStaffMembersForClub(UInt gameId, foom::team *team, FifamClub *dst, Bool isNationalTeam);
    UChar GetPlayerLevelFromCA(Int ca);
    
    Bool IsIconicPlayer(Int playerId);
    Bool IsIntrovertPlayer(Int playerId);
    Bool IsExtrovertPlayer(Int playerId);
    Bool IsInsecurePlayer(Int playerId);
    Bool IsFansFavouritePlayer(Int playerId);
    Bool IsSensitivePlayer(Int playerId);

    Bool IsPlayerRetiredFromNationalTeam(Int playerId);

    FifamClubLink GetCompWinner(foom::comp *c, Bool checkPreviousSeason = false);
    void GetCompWinnerAndRunnerUp(foom::comp *c, FifamClubLink &outWinner, FifamClubLink &outRunnerUp, Bool checkPreviousSeason = false);
    FifamClubLink GetCompHost(foom::comp *c, Bool checkPreviousSeason = false);

    FifamClubLink GetTeamClubLink(foom::team *t, Bool allowReserveTeam = true);

    Map<UShort, foom::team *> GetWinnersList(Vector<foom::comp *> const &inputComps, bool isSupercup = false);

    FifamFormation ConvertFormationId(Int id);

    bool GenerateCalendar(FifamNation const &countryId, FifamDatabase *database, Vector<FifamCompLeague *> const &leagues,
        Vector<FifamCompCup *> const &cups, Int startDate, Int endDate, Int winterBreakStart, Int winterBreakEnd);
};
