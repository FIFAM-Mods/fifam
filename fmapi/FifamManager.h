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

    FifamNation mNationality;

    // @since FM07
    FifamDate mBirthDay;

    // @since FM07
    // @range 0-15
    Char mMotivationSkills = 0;

    // @since FM07
    // @range 0-15
    Char mCoachingSkills = 0;

    // @since FM07
    // @range 0-15
    Char mGoalkeepersTraining = 0;

    // @since FM07
    // @range 0-15
    Char mNegotiationSkills = 0;

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
