#pragma once
#include "FifamTypes.h"
#include "FifamKitBadgePosition.h"
#include "FifamShirtNumberColor.h"
#include "FifamKitColor.h"
#include "FifamReadWrite.h"

// @since FM07
// club generic kit
class FifamKit {
public:
    enum {
        Home = 0, Away = 1
    };

    // @since FM07
    struct Set {
        // @since FM07
        UChar mShirt = 0;

        // @since FM07
        UChar mShorts = 0;

        // @since FM07
        UChar mSocks = 0;

        // @since FM07
        Array<FifamKitColor, 3> mShirtColors = {};

        // @since FM07
        Array<FifamKitColor, 3> mShortsColors = {};

        // @since FM07
        Array<FifamKitColor, 3> mSocksColors = {};

        // @since FM07
        FifamShirtNumberColor mShirtNumberColor;

        // @since FM07
        FifamKitBadgePosition mBadgePosition;

        struct {
            // @since FM07
            Array<UChar, 3> _1 = {};
        } Unknown;
    };

    UInt mSpecialKitId = 0;

    // @since FM07
    Array<Set, 2> mSets;

    void Read(FifamReader &reader);
    void Write(FifamWriter &writer);
};
