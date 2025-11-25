#include "FifamAppearanceDefs.h"
#include "FifamPlayer.h"
#include "FifamPlayerAppearance.h"
#include "Random.h"

void FifamAppearanceDefs::Read(Path const &filepath) {
    FifamReader reader(filepath, 0);
    if (reader.Available()) {
        reader.SkipLine();
        UInt numAppDefs = reader.ReadLine<UInt>();
        for (UInt i = 0; i < numAppDefs; i++) {
            UInt id = reader.ReadLine<UInt>();
            AppearanceDef &def = mAppearanceDefs[id];
            UInt numTranslationNames = reader.ReadLine<UInt>();
            for (UInt n = 0; n < numTranslationNames; n++) {
                id = reader.ReadLine<UInt>();
                def.mNames[id] = reader.ReadFullLine();
            }
            def.unknown = reader.ReadLine<Int>();
            def.mHeight = reader.ReadLine<Int>();
            def.mWeight = reader.ReadLine<Int>();
            UInt numAppearanceParameters = reader.ReadLine<UInt>();
            for (UInt a = 0; a < numAppearanceParameters; a++) {
                UInt numValues = reader.ReadLine<UInt>();
                if (a < 9) {
                    def.mParameters[a].resize(numValues, { 0, 0 });
                    for (UInt v = 0; v < numValues; v++)
                        def.mParameters[a][v].first = reader.ReadLine<Int>();
                }
                else
                    reader.SkipLines(numValues);
                numValues = reader.ReadLine<UInt>();
                if (a < 9) {
                    for (UInt v = 0; v < numValues; v++) {
                        if (v < def.mParameters[a].size()) {
                            UChar percentage = reader.ReadLine<UChar>();
                            def.mParameters[a][v].second = percentage;
                            def.mParametersSum[a] += percentage;
                        }
                        else
                            reader.SkipLine();
                    }
                }
                else
                    reader.SkipLines(numValues);
            }
        }
    }
}

void FifamAppearanceDefs::Write(Path const &filepath) {
    FifamWriter writer(filepath);
    if (writer.Available()) {
        UInt count = mAppearanceDefs.size();
        writer.WriteLine(count + 1);
        writer.WriteLine(count);
        for (auto const &[id, def] : mAppearanceDefs) {
            writer.WriteLine(id);
            writer.WriteLine<UInt>(def.mNames.size());
            for (auto const &[translationId, name] : def.mNames) {
                writer.WriteLine(translationId);
                writer.WriteLine(name);
            }
            writer.WriteLine(def.unknown);
            writer.WriteLine(def.mHeight);
            writer.WriteLine(def.mWeight);
            UInt numParameters = def.mParameters.size();
            writer.WriteLine(numParameters);
            for (UInt a = 0; a < numParameters; a++) {
                UInt numValues = def.mParameters[a].size();
                writer.WriteLine(numValues);
                for (UInt v = 0; v < numValues; v++)
                    writer.WriteLine(def.mParameters[a][v].first);
                writer.WriteLine(numValues);
                for (UInt v = 0; v < numValues; v++)
                    writer.WriteLine(def.mParameters[a][v].second);
            }
        }
    }
}

Int FifamAppearanceDefs::GetRandomAppearanceParam(FifamAppearanceDefs::AppearanceDef &def, FifamAppearanceDefs::Param param, Int defaultValue) {
    if (def.mParametersSum[param] > 0) {
        UInt rnd = Random::Get(0, def.mParametersSum[param] - 1);
        UInt acc = 0;
        for (UInt i = 0; i < def.mParameters[param].size(); i++) {
            acc += def.mParameters[param][i].second;
            if (rnd < acc)
                return def.mParameters[param][i].first;
        }
    }
    return defaultValue;
}

void FifamAppearanceDefs::Generate(FifamPlayer *player, FifamAppearanceDefs::Type type) {
    AppearanceDef &def = mAppearanceDefs[type];
    player->mAppearance.mSkinColor = GetRandomAppearanceParam(def, ParamSkinColor, player->mAppearance.mSkinColor);
    player->mAppearance.mGenericFace = GetRandomAppearanceParam(def, ParamFace, player->mAppearance.mGenericFace);
    player->mAppearance.mHairStyle = GetRandomAppearanceParam(def, ParamHair, player->mAppearance.mHairStyle);
    player->mAppearance.mFaceVariation = GetRandomAppearanceParam(def, ParamVariation, player->mAppearance.mFaceVariation);
    player->mAppearance.mBeardColor = GetRandomAppearanceParam(def, ParamBeardColor, player->mAppearance.mBeardColor);
    player->mAppearance.mEyeColour = GetRandomAppearanceParam(def, ParamEyeColor, player->mAppearance.mEyeColour);
    player->mAppearance.mBeardType = GetRandomAppearanceParam(def, ParamBeard, player->mAppearance.mBeardType);
    player->mAppearance.mSideburns = GetRandomAppearanceParam(def, ParamSideburns, player->mAppearance.mSideburns);
    player->mAppearance.mHairColor = GetRandomAppearanceParam(def, ParamHairColor, player->mAppearance.mHairColor);
    player->mAppearance.SetBeardColorFromHairColor();
}
