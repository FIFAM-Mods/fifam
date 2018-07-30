#pragma once
#include "FifamTypes.h"
#include "FifamKitBadgePosition.h"
#include "FifamKitColor.h"
#include "FifamShirtNumberColor.h"
#include "Color.h"
#include "FifamReadWrite.h"

// @since FM07
// club generic kit
class FifamKit {
public:
    enum { Home = 0, Away = 1 };

    // @since FM07
    struct Set {
        // @since FM07
        UChar mShirt = 0;
        // @since FM07
        UChar mShorts = 0;
        // @since FM07
        UChar mSocks = 0;
        // @since FM07
        Array<Color, 3> mShirtColors;
        // @since FM07
        Array<Color, 3> mShortsColors;
        // @since FM07
        Array<Color, 3> mSocksColors;
        // @since FM07
        Color mShirtNumberColor;
        // @since FM07
        FifamKitBadgePosition mBadgePosition;

        struct {
            // @since FM07
            Array<Color, 3> _1 = {};
        } Unknown;
    };
    // @since FM07
    Array<Set, 2> mSets;
    // @since FM07
    UInt mSpecialKitId = 0;

    inline static Vector<Color> mKitColorTable = {
        { 255, 85, 85 },
        { 14, 177, 2 },
        { 1, 186, 220 },
        { 255, 240, 0 },
        { 172, 121, 40 },
        { 152, 206, 216 },
        { 220, 1, 145 },
        { 255, 255, 255 },
        { 255, 0, 0 },
        { 11, 146, 1 },
        { 1, 157, 185 },
        { 255, 210, 0 },
        { 137, 96, 32 },
        { 119, 157, 164 },
        { 220, 1, 181 },
        { 230, 227, 227 },
        { 227, 4, 4 },
        { 8, 116, 0 },
        { 0, 117, 139 },
        { 255, 192, 0 },
        { 98, 66, 15 },
        { 93, 123, 128 },
        { 220, 1, 212 },
        { 201, 201, 201 },
        { 201, 0, 0 },
        { 2, 177, 101 },
        { 0, 156, 255 },
        { 255, 162, 0 },
        { 185, 109, 53 },
        { 144, 202, 136 },
        { 202, 1, 220 },
        { 170, 167, 167 },
        { 176, 2, 2 },
        { 0, 96, 43 },
        { 0, 96, 255 },
        { 255, 138, 0 },
        { 153, 91, 46 },
        { 103, 142, 98 },
        { 176, 1, 220 },
        { 128, 128, 128 },
        { 149, 0, 0 },
        { 121, 148, 13 },
        { 2, 35, 204 },
        { 255, 114, 0 },
        { 119, 68, 30 },
        { 72, 100, 68 },
        { 150, 1, 220 },
        { 95, 95, 95 },
        { 121, 0, 0 },
        { 81, 101, 0 },
        { 66, 0, 255 },
        { 255, 84, 0 },
        { 172, 139, 40 },
        { 255, 236, 148 },
        { 130, 1, 220 },
        { 56, 56, 56 },
        { 92, 0, 0 },
        { 46, 61, 0 },
        { 50, 5, 180 },
        { 255, 36, 0 },
        { 123, 99, 27 },
        { 201, 186, 115 },
        { 92, 0, 156 },
        { 0, 0, 0 }
    };
    inline static Vector<Color> mShirtNumberColorTable = {
        { 0, 0, 0 },
        { 255, 255, 255 },
        { 0, 0, 0 },
        { 255, 255, 0 },
        { 255, 0, 0 },
        { 0, 255, 0 },
        { 0, 0, 255 }
    };

    void Read(FifamReader &reader);
    void Write(FifamWriter &writer);
};
