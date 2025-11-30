#pragma once
#include "FifamDatabase.h"
#include "foom_db.h"
#include "FifaDatabase.h"
#include "AppearanceGenerator.h"
#include "FifamCompLeague.h"
#include "FifamCompCup.h"
#include "FifamCompRound.h"

const UShort CURRENT_YEAR = 2025;

class Converter {
public:
    FifamDatabase *mReferenceDatabase = nullptr;
    FifamDatabase *mFifamDatabase = nullptr;
    FifaDatabase *mFifaDatabase = nullptr;
    UInt mCurrentGameId = FifamDatabase::LATEST_GAME_VERSION;
    Bool mFromFifaDatabase = false;
    foom::db *mFoomDatabase = nullptr;
    Bool mWarnings = false;
    Bool mErrors = true;
    Bool mLogErrors = false;
    Path mLogPath = "errorlog.txt";
    Path mOutputGameFolder;
    Bool mWriteToGameFolder = false;
    Path mTestsOutputFolder;
    Path mContentFolder;
    Path mContentArtsFolder;
    Path mFifaAssetsPath;
    Bool mGenerateLeaguesFiles = false;
    Bool mGenerateLeagueConfigFiles = true;
    Bool mGenerateSpecialScripts = true;
    Bool mQuickTest = false;
    Bool mWomen = false;
    Bool mToFifa07Database = false;
    Bool mLogAppearance = false;
    Bool mLogWeights = false;

    FifamDatabase *mPreviousDb = nullptr;
    Map<UInt, FifamPlayer *> mPreviousPlayers;
    Map<UInt, ColorPair> mRefDbColors;
    Set<UInt> mFreeAgentsToAdd;
    Array<UInt, 207> mIPCountryStrength = {};
    Set<UInt> mFaceIDs;
    Map<UInt, Int> mFifamCompIdToFifa07LeagueId;
    Map<UInt, Int> mFifamCompIdToFifa07TournamentId;

    FifamClub *GetPreviousClub(UInt id);
    FifamPlayer *GetPreviousPlayer(UInt id);
    void UpdateDataFromPreviousDb();

    struct PlayOffInfo {
        struct TeamEntry {
            UChar mLevel = 0;
            UChar mPositionIndex = 0;
            UChar mTablePosition = 0;
            UShort mLeagueIndex = 0;
            FifamCompLeague *mLeague = nullptr;
        };

        struct Round {
            enum class ShuffleType { None, AllTeams, HalfTeams };
            UChar mLegs = 1;
            ShuffleType mShuffle = ShuffleType::None;
            Bool mLosersRound = false;
            UShort mNewTeams = 0;
            FifamCompRound *mFifamRound = nullptr;
            Vector<TeamEntry> mTeamEntries;
        };

        struct League {
            FifamCompLeague *mFifamLeague = nullptr;
            UShort mTotalTeams = 0;
            UShort mNumWinners = 0;
            UShort mNumLosers = 0;
            Vector<TeamEntry> mTeamEntries;
        };

        Int mNationID = -1;
        Int mID = 0;
        FifamTrArray<String> mName;
        Vector<Round> mRounds;
        League mLeague;
        Bool mIsLeague = false;
        Int mSubs = 0;
        Int mAwayGoalRule = -1;

        Int mMinLeagueLevel = -1;
        Int mMaxLeagueLevel = -1;
        Int mPromotionLevel = -1;
        Int mRelegationLevel = -1;
    };

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
        UInt mPromoted = 0;
        UInt mRelegated = 0;
        Pair<Int, Int> mSplit = { 0, 0 };
        Pair<Int, Int> mSplitRounds = { 1, 1 };
        Vector<UInt> mPromotionPlayoff;
        Vector<UInt> mRelegationPlayoff;
        UInt mTotalTeamsPromotionPlayoff = 0;
        UInt mTotalTeamsRelegationPlayoff = 0;
        Int mPromotionID = 0;
        Int mRelegationID = 0;
        Int mStartDate = 0;
        Int mEndDate = 0;
        Int mWinterBreakStart = 0;
        Int mWinterBreakEnd = 0;
        Int mNumSubs = 0;
        Bool mAwayGoalRule = false;
        Int mForeignersField = -1;
        Int mForeignersGame = -1;
        Int mSeasonSquad = -1;
        Int mSeasonSquadForeigners = -1;
        Int mNonEuSigns = 0;
        Int mReserveTeamsAllowed = -1;
        FifamEqualPointsSorting mSorting;
        Int mTvBonus = -1;
        Int mWinBouns = -1;
        Int mPlaceBonus = -1;
        Int mAttendanceMp = 0;
        Int mTransfersMp = 0;
        FifamCompID mCompID;
        Bool mOneYearCalendar = false;
        Int mCalendarMatches = 0;
        PlayOffInfo *mPromotionInfo = nullptr;
        PlayOffInfo *mRelegationInfo = nullptr;
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
        Int mAwayGoalRule = -1;
        Int mBonus = 0;
        Int mTvBonus = 0;
        Int mStartDate = 0;
        Int mEndDate = 0;
        Bool mOneYearCalendar = false;
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
    Vector<PlayOffInfo> mPlayOffs;
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
    Map<Int, Int> mPenaltyPointsMap;
    Array<Map<Int, String>, FifamTranslation::NUM_TRANSLATIONS + 1> mAbbreviationMap;
    Array<Map<Int, String>, FifamTranslation::NUM_TRANSLATIONS + 1> mShortNamesMap;
    Array<Map<Int, String>, FifamTranslation::NUM_TRANSLATIONS + 1> mNamesMap;
    Map<UInt, Vector<UShort>> mCalendarsFirstSeason;
    Map<UInt, Vector<UShort>> mCalendarsSecondSeason;
    Map<UInt, Pair<FifamTrArray<String>, FifamTrArray<String>>> mSplitNames;
    
    ~Converter();
    Date GetCurrentDate() { return Date(1, 7, CURRENT_YEAR); }
    Date FmEmptyDate() { return Date(1, 1, 1900); }

    void ReadAdditionalInfo(Path const &infoPath, UInt gameId);
    void Convert();

    Int ConvertPlayerAttribute(Int attr, UInt gameId = 14);
    Int LinearConvertPlayerAttribute(Int attr, UInt gameId = 14);
    void ConvertNationInfo(FifamCountry *dst, foom::nation *nation, UInt gameId);
    void ConvertClub(UInt gameId, FifamClub *dst, foom::club *team, foom::club *mainTeam, FifamCountry *country, DivisionInfo *div);
    void ConvertReserveClub(UInt gameId, FifamClub *dst, foom::club *team, foom::club *mainTeam, FifamCountry *country, DivisionInfo *div);
    void ConvertClubStadium(FifamClub *dst, UInt gameId);
    void ApplyClubCustomNames(UInt foomID, FifamClub *club);
    FifamClub *CreateAndConvertClub(UInt gameId, foom::club *team, foom::club *mainTeam, FifamCountry *country, DivisionInfo *div, bool convertSquad);
    void ConvertReferee(FifamReferee *dst, foom::official *official, UInt gameId);
    void ConvertKitsAndColors(FifamClub *dst, Int foomId, Vector<foom::kit> const &kits, Int badgeType, Color const &teamBackgroundColor,
        Color const &teamForegroundColor, UInt gameId);
    FifamPlayer *CreateAndConvertPlayer(UInt gameId, foom::player *p, FifamClub *club);
    FifamStaff *CreateAndConvertStaff(foom::non_player *p, FifamClub *club, FifamClubStaffPosition position, UInt gameId);
    String FixPersonName(String const &name, UInt gameId);
    void ConvertPersonAttributes(FifamPerson *person, foom::person *p, UInt gameId);
    void CreateStaffMembersForClub(UInt gameId, foom::team *team, FifamClub *dst, Bool isNationalTeam);
    UChar GetPlayerLevelFromCA(Int ca);

    Bool IsConvertable(foom::person *p, UInt gameId);
    Bool IsConvertable(foom::official *o, UInt gameId);

    UChar GetPlayerLevel(FifamPlayer *player, Bool includeExperience, UInt gameId);
    UChar GetPlayerLevel(FifamPlayer *player, FifamPlayerPosition position, FifamPlayerPlayingStyle style, Bool includeExperience, UInt gameId);

    Bool IsPlayerRetiredFromNationalTeam(Int playerId);

    FifamClubLink GetCompWinner(foom::comp *c, Bool checkPreviousSeason = false);
    void GetCompWinnerAndRunnerUp(foom::comp *c, FifamClubLink &outWinner, FifamClubLink &outRunnerUp, Bool checkPreviousSeason = false);
    FifamClubLink GetCompHost(foom::comp *c, Bool checkPreviousSeason = false);

    FifamClubLink GetTeamClubLink(foom::team *t, Bool allowReserveTeam = true);

    Map<UShort, foom::team *> GetWinnersList(Vector<foom::comp *> const &inputComps, bool isSupercup = false);

    FifamFormation ConvertFormationId(Int id);
    Int ConvertFormationIdToCustom(Int id);

    Bool GenerateCalendar(FifamNation const &countryId, FifamDatabase *database, Vector<FifamCompLeague *> const &leagues, Vector<FifamCompCup *> const &cups, Pair<FifamCompLeague *, FifamCompLeague *> const &split, Vector<PlayOffInfo *> const &playOffs);

    FifamPlayer *CreateAndConvertFifaPlayer(UInt gameId, FifaPlayer *p, FifamClub *club);

    void ConvertLeagues(UInt gameId);
    Bool ProcessScriptWithSpecialFormat(FifamCountry *country, Vector<FifamCompLeague *> &leagues, Vector<FifamCompCup *> &cups, Pair<FifamCompLeague *, FifamCompLeague *> &split, Vector<PlayOffInfo *> &playOffs);

    void GenerateNewTeamIDsFile(Path const &outputFilePath, Path const &oldTeamIDsFilePath);

    Bool ClubColorsFromBadgeFile(UInt clubId, FifamClubTeamColor &out);

    enum eAlertType { AL_WARNING, AL_ERROR };

    template<typename ...ArgTypes>
    Bool Alert(eAlertType type, const std::wstring &format, ArgTypes... args) {
        if ((type == AL_ERROR && mErrors) || (type == AL_WARNING && mWarnings)) {
            static WideChar text[4096];
            swprintf(text, 4096, format.c_str(), Utils::FormatterUtilsUnicode::Arg(args)...);
            if (!mLogErrors)
                MessageBoxW(NULL, text, (type == AL_ERROR) ? L"Error" : L"Warning", (type == AL_ERROR) ? MB_ICONERROR : MB_ICONWARNING);
            else {
                Bool fileExists = exists(mLogPath);
                FILE *file = nullptr;
                if (!fileExists) {
                    file = _wfopen(mLogPath.c_str(), L"wb");
                    if (!file)
                        return false;
                    const unsigned char bom[] = { 0xFF, 0xFE };
                    fwrite(bom, sizeof(bom), 1, file);
                    fclose(file);
                }
                file = _wfopen(mLogPath.c_str(), L"at, ccs=UNICODE");
                if (!file)
                    return false;
                fputws((type == AL_ERROR) ? L"Error\t" : L"Warning\t", file);
                auto Quoted = [](String const &str) -> String {
                    Bool addQuotes = false;
                    for (WideChar c : str) {
                        if (c == L'\r' || c == L'\n' || c == L'"' || c == L'\t') {
                            addQuotes = true;
                            break;
                        }
                    }
                    if (!addQuotes)
                        return str;
                    String result;
                    result += L'"';
                    for (WideChar c : str) {
                        result += c;
                        if (c == L'"')
                            result += c;
                    }
                    result += L'"';
                    return result;
                };
                fputws(Quoted(text).c_str(), file);
                fputws(L"\n", file);
                fclose(file);
            }
        }
        return false;
    }
};
