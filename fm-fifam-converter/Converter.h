#pragma once
#include "FifamDatabase.h"
#include "foom_db.h"

const UShort CURRENT_YEAR = 2017;

class Converter {
public:
    FifamDatabase *mReferenceDatabase = nullptr;
    FifamDatabase *mFifamDatabase = nullptr;
    UInt mCurrentGameId = FifamDatabase::LATEST_GAME_VERSION;
    foom::db *mFoomDatabase = nullptr;

    struct DivisionInfo {
        String mNationName;
        Int mNationID = -1;
        String mName;
        String mShortName;
        Int mID = -1;
        Bool mIsLvl = 0;
        Int mLevel = 0;
        Bool mIsTop = 0;
        Int mParent = -1;
        Int mTeams = 0;
        Int mRep = 0;
        Int mPriority = 0;
    };

    Vector<DivisionInfo> mDivisions;
    Array<UInt, FifamDatabase::NUM_COUNTRIES> mNextFreeUID;
    Array<UInt, FifamDatabase::NUM_COUNTRIES> mNumTeamsInLeagueSystem;
    Array<Vector<foom::player *>, FifamDatabase::NUM_COUNTRIES> mNationalTeamPlayers;
    UInt mPersonIdCounter = 1;
    
    Date GetCurrentDate() { return Date(1, 7, CURRENT_YEAR); }
    Date FmEmptyDate() { return Date(1, 1, 1900); }

    void ReadAdditionalInfo(Path const &infoPath);
    void Convert(UInt gameId, Bool writeToGameFolder);
    void Convert(UInt gameId, UInt originalGameId, Path const &originalDb, UInt referenceGameId, Path const &referenceDb, Bool writeToGameFolder);
    ~Converter();

    Int ConvertPlayerAttribute(Int attr);
    void ConvertNationInfo(FifamCountry *dst, foom::nation *nation);
    void ConvertClub(FifamClub *dst, foom::club *team, foom::club *mainTeam, FifamCountry *country, DivisionInfo *div);
    FifamClub *CreateAndConvertClub(foom::club *team, foom::club *mainTeam, FifamCountry *country, DivisionInfo *div);
    void ConvertReferee(FifamReferee *dst, foom::official *official);
    void ConvertKitsAndColors(FifamClub *dst, Vector<foom::kit> const &kits, Int badgeType);
    FifamPlayer *CreateAndConvertPlayer(foom::player *p, FifamClub *club);
    FifamStaff *CreateAndConvertStaff(foom::non_player *p, FifamClub *club, FifamClubStaffPosition position);
    void ConvertPersonAttributes(FifamPerson *person, foom::person *p);
    
    bool IsIconicPlayer(Int playerId);
    bool IsIntrovertPlayer(Int playerId);
    bool IsExtrovertPlayer(Int playerId);
    bool IsInsecurePlayer(Int playerId);
    bool IsFansFavouritePlayer(Int playerId);
    bool IsSensitivePlayer(Int playerId);
};
