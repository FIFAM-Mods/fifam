#pragma once
#include "FifamAppearanceDefs.h"

class FifaPlayer;

class AppearanceGenerator {
public:
    enum HairLength {
        HairShort = 0,
        HairMedium = 1,
        HairLong = 2
    };

    enum HairRule {
        HairWithoutBaldingWithBald = 0,
        HairWithoutBaldingAndBald = 1
    };

    struct AppearanceDefHairStyles {
        Array<Vector<Pair<Int, UChar>>, 3> mHairstylesByLength;
        Array<UInt, 3> mHairstylesByLengthSum;
        Array<Vector<Pair<Int, UChar>>, 2> mHairstylesByRule;
        Array<UInt, 2> mHairstylesByRuleSum;
    };

    Map<UInt, FifamAppearanceDefs::AppearanceDef> mAppearanceDefs;
    Map<UInt, AppearanceDefHairStyles> mAppearanceDefsHairstyles;

    void Read(Path const &filename);
    Int GetRandomAppearanceParam(FifamAppearanceDefs::Type type, FifamAppearanceDefs::Param param, Int defaultValue = 0);
    Int GetRandomAppearanceHairstyleByLength(FifamAppearanceDefs::Type type, HairLength hairLength, Int defaultValue = 0);
    Int GetRandomAppearanceHairstyleByRule(FifamAppearanceDefs::Type type, HairRule hairRule, Int defaultValue = 0);
    void Generate(FifamPlayer *player, FifamAppearanceDefs::Type type, UInt age);
    void SetFromFifaPlayer(FifamPlayer *player, FifaPlayer *fifaPlayer);
};
