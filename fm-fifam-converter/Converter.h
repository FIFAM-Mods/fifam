#pragma once
#include "FifamDatabase.h"
#include "foom_db.h"

const UShort CURRENT_YEAR = 2018;

class Converter {
public:
    FifamDatabase *mReferenceDatabase = nullptr;
    FifamDatabase *mFifamDatabase = nullptr;
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

    void ReadAdditionalInfo(Path const &infoPath);
    void Convert(UInt gameId, Bool writeToGameFolder);
    void Convert(UInt gameId, Path const &originalDb, Path const &referenceDb, Bool writeToGameFolder);
    ~Converter();

    Int ConvertPlayerAttribute(Int attr);
    void ConvertNationInfo(FifamCountry *dst, foom::nation *nation);
    void ConvertClub(FifamClub *dst, foom::club *team, foom::club *mainTeam, FifamCountry *country, DivisionInfo *div);
    FifamClub *CreateAndConvertClub(foom::club *team, foom::club *mainTeam, FifamCountry *country, DivisionInfo *div);
    void ConvertReferee(FifamReferee *dst, foom::official *official);
    void ConvertKitsAndColors(FifamClub *dst, Vector<foom::kit> const &kits);
};
