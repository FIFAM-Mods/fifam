#pragma once
#include "FifamTypes.h"
#include "FifamReadWrite.h"

// @since FM11
class FifamCountryAppearanceData {
public:
    // @since FM11
    UInt mAppearanceType = 0;
    // @since FM11
    UInt mDistributionProbability = 0;

    void Read(FifamReader &reader);
    void Write(FifamWriter &writer);
};
