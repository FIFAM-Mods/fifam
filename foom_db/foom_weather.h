#pragma once
#include "FifamTypes.h"
#include "FifamClimate.h"

namespace foom {

struct weather {
    Int mID = 0;
    String mName;
    UShort mSpringStartDay = 0;
    UChar mSpringPrecipitationDry = 0;
    UChar mSpringPrecipitationWet = 0;
    UChar mSpringPrecipitationDrizzle = 0;
    UChar mSpringPrecipitationShower = 0;
    UChar mSpringPrecipitationDownPour = 0;
    UChar mSpringWindCalm = 0;
    UChar mSpringWindBreezy = 0;
    UChar mSpringWindGusty = 0;
    UChar mSpringWindStrong = 0;
    UChar mSpringWindGale = 0;
    UChar mSpringTemperatureExtremelyFreezing = 0;
    UChar mSpringTemperatureVeryFreezing = 0;
    UChar mSpringTemperatureFreezing = 0;
    UChar mSpringTemperatureCold = 0;
    UChar mSpringTemperatureMild = 0;
    UChar mSpringTemperatureFine = 0;
    UChar mSpringTemperatureWarm = 0;
    UChar mSpringTemperatureHot = 0;
    UChar mSpringTemperatureVeryHot = 0;
    UChar mSpringTemperatureExtremelyHot = 0;
    UChar mSpringTemperatureDayNightVariance = 0;
    UChar mSummerStartDay = 0;
    UChar mSummerPrecipitationDry = 0;
    UChar mSummerPrecipitationWet = 0;
    UChar mSummerPrecipitationDrizzle = 0;
    UChar mSummerPrecipitationShower = 0;
    UChar mSummerPrecipitationDownPour = 0;
    UChar mSummerWindCalm = 0;
    UChar mSummerWindBreezy = 0;
    UChar mSummerWindGusty = 0;
    UChar mSummerWindStrong = 0;
    UChar mSummerWindGale = 0;
    UChar mSummerTemperatureExtremelyFreezing = 0;
    UChar mSummerTemperatureVeryFreezing = 0;
    UChar mSummerTemperatureFreezing = 0;
    UChar mSummerTemperatureCold = 0;
    UChar mSummerTemperatureMild = 0;
    UChar mSummerTemperatureFine = 0;
    UChar mSummerTemperatureWarm = 0;
    UChar mSummerTemperatureHot = 0;
    UChar mSummerTemperatureVeryHot = 0;
    UChar mSummerTemperatureExtremelyHot = 0;
    UChar mSummerTemperatureDayNightVariance = 0;
    UChar mAutumnStartDay = 0;
    UChar mAutumnPrecipitationDry = 0;
    UChar mAutumnPrecipitationWet = 0;
    UChar mAutumnPrecipitationDrizzle = 0;
    UChar mAutumnPrecipitationShower = 0;
    UChar mAutumnPrecipitationDownPour = 0;
    UChar mAutumnWindCalm = 0;
    UChar mAutumnWindBreezy = 0;
    UChar mAutumnWindGusty = 0;
    UChar mAutumnWindStrong = 0;
    UChar mAutumnWindGale = 0;
    UChar mAutumnTemperatureExtremelyFreezing = 0;
    UChar mAutumnTemperatureVeryFreezing = 0;
    UChar mAutumnTemperatureFreezing = 0;
    UChar mAutumnTemperatureCold = 0;
    UChar mAutumnTemperatureMild = 0;
    UChar mAutumnTemperatureFine = 0;
    UChar mAutumnTemperatureWarm = 0;
    UChar mAutumnTemperatureHot = 0;
    UChar mAutumnTemperatureVeryHot = 0;
    UChar mAutumnTemperatureExtremelyHot = 0;
    UChar mAutumnTemperatureDayNightVariance = 0;
    UChar mWinterStartDay = 0;
    UChar mWinterPrecipitationDry = 0;
    UChar mWinterPrecipitationWet = 0;
    UChar mWinterPrecipitationDrizzle = 0;
    UChar mWinterPrecipitationShower = 0;
    UChar mWinterPrecipitationDownPour = 0;
    UChar mWinterWindCalm = 0;
    UChar mWinterWindBreezy = 0;
    UChar mWinterWindGusty = 0;
    UChar mWinterWindStrong = 0;
    UChar mWinterWindGale = 0;
    UChar mWinterTemperatureExtremelyFreezing = 0;
    UChar mWinterTemperatureVeryFreezing = 0;
    UChar mWinterTemperatureFreezing = 0;
    UChar mWinterTemperatureCold = 0;
    UChar mWinterTemperatureMild = 0;
    UChar mWinterTemperatureFine = 0;
    UChar mWinterTemperatureWarm = 0;
    UChar mWinterTemperatureHot = 0;
    UChar mWinterTemperatureVeryHot = 0;
    UChar mWinterTemperatureExtremelyHot = 0;
    UChar mWinterTemperatureDayNightVariance = 0;

    struct converter_data {
        FifamClimate mFIFAManagerClimate;
    } mConverterData;
};

}
