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

    struct AppearanceDefHairStyles {
        Array<Vector<Pair<Int, UChar>>, 3> mHairstylesByLength;
        Array<UInt, 3> mHairstylesSum;
    };

    Map<UInt, FifamAppearanceDefs::AppearanceDef> mAppearanceDefs;
    Map<UInt, AppearanceDefHairStyles> mAppearanceDefsHairstyles;

    void Read(Path const &filename);
    Int GetRandomAppearanceParam(FifamAppearanceDefs::Type type, FifamAppearanceDefs::Param param, Int defaultValue = 0);
    Int GetRandomAppearanceHairstyle(FifamAppearanceDefs::Type type, HairLength hairLength, Int defaultValue = 0);
    void Generate(FifamPlayer *player, FifamAppearanceDefs::Type type);
    void SetFromFifaPlayer(FifamPlayer *player, FifaPlayer *fifaPlayer);
};
