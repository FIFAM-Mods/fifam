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
    } mConverterData;

    void read_binary(binary_file &file) {
        file.read(mID);
        file.read(mFirstName);
        file.read(mSecondName);
        file.read(mCommonName);
        file.read(mFullName);
        file.read(mFemale);
        file.read(mDateOfBirth);
        file.read(mNation);
        file.read(mLanguage);
        file.read(mAdaptability);
        file.read(mAmbition);
        file.read(mControversy);
        file.read(mLoyalty);
        file.read(mPressure);
        file.read(mProfessionalism);
        file.read(mSportsmanship);
        file.read(mTemperament);
    }

    void write_binary(binary_file &file) {
        file.write(mID);
        file.write(mFirstName);
        file.write(mSecondName);
        file.write(mCommonName);
        file.write(mFullName);
        file.write(mFemale);
        file.write(mDateOfBirth);
        file.write(mNation);
        file.write(mLanguage);
        file.write(mAdaptability);
        file.write(mAmbition);
        file.write(mControversy);
        file.write(mLoyalty);
        file.write(mPressure);
        file.write(mProfessionalism);
        file.write(mSportsmanship);
        file.write(mTemperament);
    }
};

}
