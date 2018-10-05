#pragma once
#include "FifamDatabase.h"
#include "foom_db.h"

class Converter {
public:
    FifamDatabase *mReferenceDatabase = nullptr;
    FifamDatabase *mFifamDatabase = nullptr;
    foom::db *mFoomDatabase = nullptr;

    struct DivisionInfo {
        Int mCountry = -1;
        Short mName;
        Short mShortName;
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

    void ReadAdditionalInfo(Path const &infoPath);
    void Convert(UInt gameId, Bool writeToGameFolder);
    void Convert(UInt gameId, Path const &originalDb, Path const &referenceDb, Bool writeToGameFolder);
    ~Converter();

    void ConvertNationInfo(FifamCountry *dst, foom::nation *nation);
};
