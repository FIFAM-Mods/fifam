#pragma once
#include "FifamTypes.h"

namespace foom {

struct nation;

struct official {
    Int mID = 0;
    String mFirstName;
    String mSecondName;
    String mCommonName;
    String mFullName;
    Bool mFemale = 0;
    nation *mNation = nullptr;
    Int mCurrentAbility = 0;
    Int mPotentialAbility = 0;
    Int mReputation = 0;
    Int mUEFACategory = 0;
    Bool mFIFACategory = 0;
    Bool mContinentalOfficial = 0;
    Int mAllowingFlow = 0;
    Int mDiscipline = 0;
    Int mImportantMatches = 0;
    Int mPressure = 0;
    Int mRefereeing = 0;
    Int mRunningLine = 0;
    Int mTimeKeeping = 0;

};

}
