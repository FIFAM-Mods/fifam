#pragma once
#include "FifamTypes.h"
#include "FifamDate.h"
#include "FifamReadWrite.h"
#include "FifamNation.h"
#include "FifamLanguage.h"

// @since FM07
// @until FM11
class FifamManager {
public:
    // @since FM07
    String mFirstName;
    // @since FM07
    String mLastName;
    // @since FM07
    String mPseudonym;
    // @since FM07
    FifamNation mNationality;
    // @since FM07
    FifamDate mBirthDay;
    // @since FM07
    // @range 0-15
    UChar mMotivationSkills = 0;
    // @since FM07
    // @range 0-15
    UChar mCoachingSkills = 0;
    // @since FM07
    // @range 0-15
    UChar mGoalkeepersTraining = 0;
    // @since FM07
    // @range 0-15
    UChar mNegotiationSkills = 0;
    // @since FM07
    Array<FifamLanguage, 4> mLanguage;

    // @since FM07
    struct {
        // @since FM07
        UChar _1 = 0;

        // @since FM07
        UChar _2 = 0;
    } Unknown;

    void Read(FifamReader &reader);
    void Write(FifamWriter &writer);
};
