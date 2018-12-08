#pragma once
#include "FifamTypes.h"
#include "Date.h"
#include "binary_read_write.h"

namespace foom {

struct nation;
struct language;
struct club;

struct person {
    Int mID = 0;
    String mFirstName;
    String mSecondName;
    String mCommonName;
    String mFullName;
    Bool mFemale = 0;
    Date mDateOfBirth;
    nation *mNation = 0;
    language *mLanguage = 0;
    Int mAdaptability = 0;
    Int mAmbition = 0;
    Int mControversy = 0;
    Int mLoyalty = 0;
    Int mPressure = 0;
    Int mProfessionalism = 0;
    Int mSportsmanship = 0;
    Int mTemperament = 0;

    Bool mIsPlayer = false;

    struct language {
        foom::language *mLanguage = 0;
        Int mProficiency = 0;
        Bool mCannotSpeakLanguage = 0;
    };

    Vector<language> mVecLanguages;

    Vector<nation *> mVecSecondNations;

    struct favourite_club {
        club *mClub = 0;
        Int mReason = 0;
        Int mLevel = 0;
    };

    Vector<favourite_club> mVecFavouriteClubs;

    struct disliked_club {
        club *mClub = 0;
        Int mLevel = 0;
    };

    Vector<disliked_club> mVecDislikedClubs;

    struct converter_data {
        void *mFifamPerson = nullptr;
        void *mContractedFifamClub = nullptr;
        Int mTeamType = -1; // ?
        club *mLoanedFromClub = nullptr;
        club *mFutureClub = nullptr;
        Int mFifaPlayerId = 0;
        Bool mAvailableForAdultTeams = false; // ?
        Bool mFitsIntoDbLimit = true;
        Short mEditorFace = -1;
        Char mEditorHair = -1;
        Char mEditorSkin = -1;
        Char mEditorEye = -1;
        Char mEditorBeard = -1;
    } mConverterData;
};

}
