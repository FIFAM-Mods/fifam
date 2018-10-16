#pragma once
#include "FifamTypes.h"
#include "Date.h"

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
};

}
