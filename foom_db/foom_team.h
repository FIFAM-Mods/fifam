#pragma once
#include "FifamTypes.h"
#include "foom_kit.h"

namespace foom {

struct player;
struct non_player;

struct team {
    Int mID = 0;
    String mName;
    String mShortName;
    String mNickname;
    Int mReputation = 0;
    Int mAttendance = 0;
    Int mMinimumAttendance = 0;
    Int mMaximumAttendance = 0;
    Float mEuroCoeff1 = 0;
    Float mEuroCoeff2 = 0;
    Float mEuroCoeff3 = 0;
    Float mEuroCoeff4 = 0;
    Float mEuroCoeff5 = 0;
    Float mEuroCoeff6 = 0;
    Float mEuroCoeff7 = 0;
    Float mEuroCoeff8 = 0;
    Float mEuroCoeff9 = 0;
    Float mEuroCoeff10 = 0;
    Int mPreferredFormation = 0;
    Int mSecondPreferredFormation = 0;
    player *mCaptain = 0;
    player *mViceCaptain = 0;
    Color mForegroundColor;
    Color mBackgroundColor;

    Bool mIsNation = false;

    Vector<kit> mVecKits;

    Vector<non_player *> mVecContractedNonPlayers;
};

}
