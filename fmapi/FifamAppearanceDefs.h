#pragma once
#include "FifamReadWrite.h"

class FifamPlayer;

class FifamAppearanceDefs {
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
        Array<Vector<Pair<Int, UChar>>, 9> mParameters;
        Array<UInt, 9> mParametersSum;
    };

    Map<UInt, AppearanceDef> mDefs;

    void Read(Path const &filepath);
    void Write(Path const &filepath);
    Int GetRandomAppearanceParam(AppearanceDef &def, Param param, Int defaultValue = 0);
    void Generate(FifamPlayer *player, Type type);
};
