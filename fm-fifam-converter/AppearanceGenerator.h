#pragma once
#include "FifamReadWrite.h"
#include "Random.h"

class FifamPlayer;
class FifaPlayer;

class AppearanceGenerator {
public:
    enum Type {
        WesternEuropeLight = 1,
        WesternEuropeDark = 2,
        African1 = 3,
        Arabic1 = 4,
        African2 = 5,
        Latin = 6,
        Asian = 7,
        EasternEuropeLight = 8,
        EasternEuropeDark = 9,
        Arabic2 = 10,
        Indian = 11
    };

    enum Param {
        ParamFace = 0,
        ParamHair = 1,
        ParamSkinColor = 2,
        ParamVariation = 3,
        ParamBeardColor = 4,
        ParamEyeColor = 5,
        ParamBeard = 6,
        ParamSideburns = 7,
        ParamHairColor = 8
    };

    struct AppearanceDef {
        Map<UInt, String> mNames;
        Int unknown;
        Int mHeight;
        Int mWeight;
        Array<Vector<Pair<UInt, UChar>>, 9> mParameters;
        Array<UInt, 9> mParametersSum;
    };

    Map<UInt, AppearanceDef> mAppearanceDefs;

    void Read(Path const &filepath);
    UChar GetRandomAppearanceParam(AppearanceDef &def, Param param, UChar defaultValue = 0);
    void Generate(FifamPlayer *player, Type type);
    void SetFromFifaPlayer(FifamPlayer *player, FifaPlayer *fifaPlayer);
};
