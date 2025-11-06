#pragma once
#include "FifamTypes.h"

namespace foom {

struct injury {
    Int mID = 0;
    String mName;
    String mSentenceName;
    Int mClassification = 0;
    Int mSubClassification = 0;
    Int mMinimumDays = 0;
    Int mExtraDays = 0;
    Int mMinimumDaysWomen = 0;
    Int mExtraDaysWomen = 0;
    Int mInactiveRatio = 0;
    Float mOccuranceRatio = 0;
    Int mMatchInjuryPercentage = 0;
    Int mGoalkeeperPercentage = 0;
    Int mCentralDefenderPercentage = 0;
    Bool mRecurring = 0;
    Bool mContagious = 0;
    Int mCause = 0;
    Int mSeriousness = 0;
    Int mAgeRule = 0;
    Int mSeason = 0;
    Bool mTreatmentByPhysion = 0;
    Bool mTreatmentBySpecialist = 0;
    Bool mTreatmentByInjection = 0;
    Bool mTreatmentByIsolation = 0;
    Bool mAllowToPlayWithProtectiveEquipment = 0;

    struct converter_data {
        UInt mFIFAManagerID = 0;
    } mConverterData;
};

}
