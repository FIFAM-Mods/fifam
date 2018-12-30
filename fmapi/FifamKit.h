#pragma once
#include "FifamTypes.h"
#include "Color.h"
#include "FifamKitBadgePosition.h"
#include "FifamKitColor.h"
#include "FifamShirtNumberColor.h"
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
        Array<Color, 3> mSleevesColors;
        // @since FM07
        Array<Color, 3> mShortsColors;
        // @since FM07
        Array<Color, 2> mSocksColors;
        // @since FM07
        Color mCaptainArmbandColor;
        // @since FM07
        Color mShirtNumberColor;
        // @since FM07
        FifamKitBadgePosition mBadgePosition;
    };
    // @since FM07
    Array<Set, 2> mSets;
    // @since FM07
    UInt mSpecialKitId = 0;

    inline static Vector<Pair<UInt, Color>> mKitColorTable = {
        { 0,  { 255, 85, 85 }},
        { 1,  { 14, 177, 2 }},
        { 2,  { 1, 186, 220 }},
        { 3,  { 255, 240, 0 }},
        { 4,  { 172, 121, 40 }},
        { 5,  { 152, 206, 216 }},
        { 6,  { 220, 1, 145 }},
        { 7,  { 255, 255, 255 }},
        { 8,  { 255, 0, 0 }},
        { 9,  { 11, 146, 1 }},
        { 10, { 1, 157, 185 }},
        { 11, { 255, 210, 0 }},
        { 12, { 137, 96, 32 }},
        { 13, { 119, 157, 164 }},
        { 14, { 220, 1, 181 }},
        { 15, { 230, 227, 227 }},
        { 16, { 227, 4, 4 }},
        { 17, { 8, 116, 0 }},
        { 18, { 0, 117, 139 }},
        { 19, { 255, 192, 0 }},
        { 20, { 98, 66, 15 }},
        { 21, { 93, 123, 128 }},
        { 22, { 220, 1, 212 }},
        { 23, { 201, 201, 201 }},
        { 24, { 201, 0, 0 }},
        { 25, { 2, 177, 101 }},
        { 26, { 0, 156, 255 }},
        { 27, { 255, 162, 0 }},
        { 28, { 185, 109, 53 }},
        { 29, { 144, 202, 136 }},
        { 30, { 202, 1, 220 }},
        { 31, { 170, 167, 167 }},
        { 32, { 176, 2, 2 }},
        { 33, { 0, 96, 43 }},
        { 34, { 0, 96, 255 }},
        { 35, { 255, 138, 0 }},
        { 36, { 153, 91, 46 }},
        { 37, { 103, 142, 98 }},
        { 38, { 176, 1, 220 }},
        { 39, { 128, 128, 128 }},
        { 40, { 149, 0, 0 }},
        { 41, { 121, 148, 13 }},
        { 42, { 2, 35, 204 }},
        { 43, { 255, 114, 0 }},
        { 44, { 119, 68, 30 }},
        { 45, { 72, 100, 68 }},
        { 46, { 150, 1, 220 }},
        { 47, { 95, 95, 95 }},
        { 48, { 121, 0, 0 }},
        { 49, { 81, 101, 0 }},
        { 50, { 66, 0, 255 }},
        { 51, { 255, 84, 0 }},
        { 52, { 172, 139, 40 }},
        { 53, { 255, 236, 148 }},
        { 54, { 130, 1, 220 }},
        { 55, { 56, 56, 56 }},
        { 56, { 92, 0, 0 }},
        { 57, { 46, 61, 0 }},
        { 58, { 50, 5, 180 }},
        { 59, { 255, 36, 0 }},
        { 60, { 123, 99, 27 }},
        { 61, { 201, 186, 115 }},
        { 62, { 92, 0, 156 }},
        { 63, { 0, 0, 0 }},
        // added for better color detection
        { FifamKitColor::Blue8, { 0, 48, 104 }}, // Barcelona
        { FifamKitColor::Blue8, { 0, 16, 64 }}, // PSG
        { FifamKitColor::Violet8, { 96, 64, 144 }}, // Fiorentina
        { FifamKitColor::Red2, { 232, 48, 56 }}, // CSKA
        { FifamKitColor::Blue6, { 32, 64, 152 }}, // Everton
        { FifamKitColor::Red7, { 120, 32, 56 }}, // West Ham
        { FifamKitColor::Field2, { 72, 112, 96 }}, // Saint-Etienne
        { FifamKitColor::Violet1, { 232, 160, 184 }}, // Palermo
        { FifamKitColor::Green1, { 0, 120, 96 }}, // Norwich
        { FifamKitColor::Blue7, { 72, 96, 184 }}, // Chayka
        { FifamKitColor::Green4, { 0, 128, 96 }}, // Akhmat
        { FifamKitColor::Green4, { 0, 128, 88 }} // Konyaspor
    };
    inline static Vector<Pair<UInt, Color>> mShirtNumberColorTable = {
        { 1, { 255, 255, 255 }},
        { 2, { 0, 0, 0 }},
        { 3, { 255, 255, 0 }},
        { 4, { 255, 0, 0 }},
        { 5, { 0, 255, 0 }},
        { 6, { 0, 0, 255 }},
        // added for better color detection
        { FifamShirtNumberColor::Blue, { 0, 144, 144 }}
    };

    static Color GetShirtBackColor(UInt shirtId, Color const &firstColor, Color const &secondColor, Color const &thirdColor);

    void Read(FifamReader &reader);
    void Write(FifamWriter &writer);
};
