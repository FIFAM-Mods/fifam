#include "FifamPlayerAppearance.h"
#include "Utils.h"

const UChar g14HairEditorIdToReal[98] = {
    0, 25, 1, 41, 46, 47, 92, 72, 29, 31, 43, 16, 28, 88, 90, 37, 65, 77, 82, 18, 24, 40, 2, 39, 63, 61, 64, 85, 38, 86, 21, 54, 45, 75, 78, 57, 22, 89, 70, 69, 93, 19, 23, 30, 14, 20, 62, 17, 66, 67, 94, 74, 58, 87, 83, 73, 95, 36, 32, 44, 13, 15, 11, 33, 8, 9, 10, 34, 35, 53, 7, 68, 12, 56, 55, 59, 76, 80, 84, 91, 26, 4, 71, 6, 5, 42, 27, 48, 49, 81, 52, 79, 60, 97, 96, 3, 51, 50
};

enum class Face07 {
    Caucasian1 = 1,
    Caucasian2,
    Caucasian3,
    Caucasian4,
    Caucasian5,
    Caucasian6,
    Caucasian7,
    Asian1,
    Asian2,
    Asian3,
    Asian4,
    Asian5,
    Asian6,
    African1,
    African2,
    African3,
    African4,
    African5,
    African6,
    Latin1,
    Latin2,
    Latin3,
    Latin4,
    Latin5,
    Latin6,
    Caucasian8,
    Caucasian9,
    Caucasian10,
    Caucasian11,
    Caucasian12,
    Caucasian13,
    Caucasian14,
    Caucasian15,
    Caucasian16,
    Latin7,
    Latin8,
    Latin9,
    Latin10,
    Latin11,
    Latin12,
    Latin13,
    Latin14,
    Latin15,
    Latin16,
    Latin17,
    Latin18,
    EasternEur1,
    EasternEur2,
    EasternEur3,
    EasternEur4,
    EasternEur5,
    EasternEur6,
    EasternEur7,
    EasternEur8,
    EasternEur9,
    EasternEur10,
    EasternEur11,
    EasternEur12,
    African7,
    African8,
    African9,
    African10,
    African11,
    African12,
    African13,
    African14,
    African15,
    African16,
    Asian7,
    Asian8,
    Asian9,
    Asian10,
    Asian11,
    Asian12,
    Asian13,
    Asian14,
    Asian15,
    Asian16,
    Asian17,
    Arabic1,
    Arabic2,
    Arabic3,
    Arabic4,
    Arabic5,
    Arabic6,
    Caucasian17,
    Caucasian18,
    Caucasian19,
    Caucasian20,
    Caucasian21
};

enum class HairColour07{
    Black = 1, Brown = 2, MediumBlond = 3, Blonde = 4, Red = 5
};

enum class BeardType07 {
    None = 1, Goatee = 2, Moustash = 3, Shadow = 4, FullGoatee = 5, Full = 6
};

using Face07to11Info = std::tuple<Face07, FifamSkinColor, UChar, FifamFaceVariation, FifamEyeColor>;
static Vector<Face07to11Info> gFace07to11Mapping = {
{ Face07::Caucasian1,   FifamSkinColor::White,     25, FifamFaceVariation::Normal, FifamEyeColor::Brown },
{ Face07::Caucasian2,   FifamSkinColor::White,     20, FifamFaceVariation::Normal, FifamEyeColor::Brown },
{ Face07::Caucasian3,   FifamSkinColor::White,      3, FifamFaceVariation::Normal, FifamEyeColor::Brown },
{ Face07::Caucasian4,   FifamSkinColor::White,      5, FifamFaceVariation::Normal, FifamEyeColor::Brown },
{ Face07::Caucasian5,   FifamSkinColor::White,     14, FifamFaceVariation::Normal, FifamEyeColor::Brown },
{ Face07::Caucasian6,   FifamSkinColor::White,     24, FifamFaceVariation::Normal, FifamEyeColor::Brown },
{ Face07::Caucasian7,   FifamSkinColor::White,      8, FifamFaceVariation::Normal, FifamEyeColor::Brown },
{ Face07::Caucasian8,   FifamSkinColor::White,     23, FifamFaceVariation::Normal, FifamEyeColor::Brown },
{ Face07::Caucasian9,   FifamSkinColor::White,     19, FifamFaceVariation::Normal, FifamEyeColor::Brown },
{ Face07::Caucasian10,  FifamSkinColor::White,     26, FifamFaceVariation::Normal, FifamEyeColor::Brown },
{ Face07::Caucasian11,  FifamSkinColor::White,     21, FifamFaceVariation::Normal, FifamEyeColor::Brown },
{ Face07::Caucasian12,  FifamSkinColor::White,     10, FifamFaceVariation::Normal, FifamEyeColor::Brown },
{ Face07::Caucasian13,  FifamSkinColor::White,     13, FifamFaceVariation::Normal, FifamEyeColor::Brown },
{ Face07::Caucasian14,  FifamSkinColor::White,    125, FifamFaceVariation::Normal, FifamEyeColor::Brown },
{ Face07::Caucasian15,  FifamSkinColor::White,     22, FifamFaceVariation::Normal, FifamEyeColor::Brown },
{ Face07::Caucasian16,  FifamSkinColor::White,     12, FifamFaceVariation::Normal, FifamEyeColor::Brown },
{ Face07::Caucasian17,  FifamSkinColor::White,     14, FifamFaceVariation::Normal, FifamEyeColor::Brown },
{ Face07::Caucasian18,  FifamSkinColor::White,    126, FifamFaceVariation::Normal, FifamEyeColor::Brown },
{ Face07::Caucasian19,  FifamSkinColor::White,      2, FifamFaceVariation::Normal, FifamEyeColor::Brown },
{ Face07::Caucasian20,  FifamSkinColor::White,      1, FifamFaceVariation::Normal, FifamEyeColor::Brown },
{ Face07::Caucasian21,  FifamSkinColor::White,     94, FifamFaceVariation::Normal, FifamEyeColor::Brown },
{ Face07::EasternEur1,  FifamSkinColor::White,      9, FifamFaceVariation::Normal, FifamEyeColor::Brown },
{ Face07::EasternEur2,  FifamSkinColor::White,     16, FifamFaceVariation::Normal, FifamEyeColor::Brown },
{ Face07::EasternEur3,  FifamSkinColor::White,      7, FifamFaceVariation::Normal, FifamEyeColor::Brown },
{ Face07::EasternEur4,  FifamSkinColor::White,      4, FifamFaceVariation::Normal, FifamEyeColor::Brown },
{ Face07::EasternEur5,  FifamSkinColor::White,     17, FifamFaceVariation::Normal, FifamEyeColor::Brown },
{ Face07::EasternEur6,  FifamSkinColor::White,    102, FifamFaceVariation::Normal, FifamEyeColor::Brown },
{ Face07::EasternEur7,  FifamSkinColor::White,      6, FifamFaceVariation::Normal, FifamEyeColor::Brown },
{ Face07::EasternEur8,  FifamSkinColor::White,    126, FifamFaceVariation::Normal, FifamEyeColor::Brown },
{ Face07::EasternEur9,  FifamSkinColor::White,    121, FifamFaceVariation::Normal, FifamEyeColor::Brown },
{ Face07::EasternEur10, FifamSkinColor::White,    119, FifamFaceVariation::Normal, FifamEyeColor::Brown },
{ Face07::EasternEur11, FifamSkinColor::White,    124, FifamFaceVariation::Normal, FifamEyeColor::Brown },
{ Face07::EasternEur12, FifamSkinColor::White,    122, FifamFaceVariation::Normal, FifamEyeColor::Brown },
{ Face07::Latin1,       FifamSkinColor::Latin1,   103, FifamFaceVariation::Normal, FifamEyeColor::Brown },
{ Face07::Latin2,       FifamSkinColor::Latin1,    86, FifamFaceVariation::Normal, FifamEyeColor::Brown },
{ Face07::Latin3,       FifamSkinColor::Latin1,    88, FifamFaceVariation::Normal, FifamEyeColor::Brown },
{ Face07::Latin4,       FifamSkinColor::Latin1,    90, FifamFaceVariation::Normal, FifamEyeColor::Brown },
{ Face07::Latin5,       FifamSkinColor::Latin1,    74, FifamFaceVariation::Normal, FifamEyeColor::Brown },
{ Face07::Latin6,       FifamSkinColor::Latin1,    89, FifamFaceVariation::Normal, FifamEyeColor::Brown },
{ Face07::Latin7,       FifamSkinColor::White,     71, FifamFaceVariation::Normal, FifamEyeColor::Brown },
{ Face07::Latin8,       FifamSkinColor::Latin1,   110, FifamFaceVariation::Normal, FifamEyeColor::Brown },
{ Face07::Latin9,       FifamSkinColor::Latin1,    87, FifamFaceVariation::Normal, FifamEyeColor::Brown },
{ Face07::Latin10,      FifamSkinColor::Latin1,   111, FifamFaceVariation::Normal, FifamEyeColor::Brown },
{ Face07::Latin11,      FifamSkinColor::Latin1,    96, FifamFaceVariation::Normal, FifamEyeColor::Brown },
{ Face07::Latin12,      FifamSkinColor::Latin1,    73, FifamFaceVariation::Normal, FifamEyeColor::Brown },
{ Face07::Latin13,      FifamSkinColor::Latin1,    75, FifamFaceVariation::Normal, FifamEyeColor::Brown },
{ Face07::Latin14,      FifamSkinColor::Latin1,    79, FifamFaceVariation::Normal, FifamEyeColor::Brown },
{ Face07::Latin15,      FifamSkinColor::Latin1,    84, FifamFaceVariation::Normal, FifamEyeColor::Brown },
{ Face07::Latin16,      FifamSkinColor::Latin1,    70, FifamFaceVariation::Normal, FifamEyeColor::Brown },
{ Face07::Latin17,      FifamSkinColor::Latin1,    88, FifamFaceVariation::Normal, FifamEyeColor::Brown },
{ Face07::Latin18,      FifamSkinColor::Latin1,    80, FifamFaceVariation::Normal, FifamEyeColor::Brown },
{ Face07::Arabic1,      FifamSkinColor::Latin2,    82, FifamFaceVariation::Normal, FifamEyeColor::Brown },
{ Face07::Arabic2,      FifamSkinColor::Latin2,    78, FifamFaceVariation::Normal, FifamEyeColor::Brown },
{ Face07::Arabic3,      FifamSkinColor::Latin2,    77, FifamFaceVariation::Normal, FifamEyeColor::Brown },
{ Face07::Arabic4,      FifamSkinColor::Latin2,    68, FifamFaceVariation::Normal, FifamEyeColor::Brown },
{ Face07::Arabic5,      FifamSkinColor::Latin1,    81, FifamFaceVariation::Normal, FifamEyeColor::Brown },
{ Face07::Arabic6,      FifamSkinColor::Latin2,    93, FifamFaceVariation::Normal, FifamEyeColor::Brown },
{ Face07::African1,     FifamSkinColor::African2,   6, FifamFaceVariation::Normal, FifamEyeColor::Brown },
{ Face07::African2,     FifamSkinColor::African3,  52, FifamFaceVariation::Normal, FifamEyeColor::Brown },
{ Face07::African3,     FifamSkinColor::African3,  49, FifamFaceVariation::Normal, FifamEyeColor::Brown },
{ Face07::African4,     FifamSkinColor::African2,  51, FifamFaceVariation::Normal, FifamEyeColor::Brown },
{ Face07::African5,     FifamSkinColor::African2,  55, FifamFaceVariation::Normal, FifamEyeColor::Brown },
{ Face07::African6,     FifamSkinColor::African2,  57, FifamFaceVariation::Normal, FifamEyeColor::Brown },
{ Face07::African7,     FifamSkinColor::African2, 131, FifamFaceVariation::Normal, FifamEyeColor::Brown },
{ Face07::African8,     FifamSkinColor::African2,  68, FifamFaceVariation::Normal, FifamEyeColor::Brown },
{ Face07::African9,     FifamSkinColor::African2,  56, FifamFaceVariation::Normal, FifamEyeColor::Brown },
{ Face07::African10,    FifamSkinColor::African2,  50, FifamFaceVariation::Normal, FifamEyeColor::Brown },
{ Face07::African11,    FifamSkinColor::African2,  66, FifamFaceVariation::Normal, FifamEyeColor::Brown },
{ Face07::African12,    FifamSkinColor::African1,  54, FifamFaceVariation::Normal, FifamEyeColor::Brown },
{ Face07::African13,    FifamSkinColor::African2, 134, FifamFaceVariation::Normal, FifamEyeColor::Brown },
{ Face07::African14,    FifamSkinColor::African2, 114, FifamFaceVariation::Normal, FifamEyeColor::Brown },
{ Face07::African15,    FifamSkinColor::African2,  59, FifamFaceVariation::Normal, FifamEyeColor::Brown },
{ Face07::African16,    FifamSkinColor::African2, 135, FifamFaceVariation::Normal, FifamEyeColor::Brown },
{ Face07::Asian1,       FifamSkinColor::Asian,     39, FifamFaceVariation::Normal, FifamEyeColor::Brown },
{ Face07::Asian2,       FifamSkinColor::Asian,     40, FifamFaceVariation::Normal, FifamEyeColor::Brown },
{ Face07::Asian3,       FifamSkinColor::Asian,     37, FifamFaceVariation::Normal, FifamEyeColor::Brown },
{ Face07::Asian4,       FifamSkinColor::Asian,     47, FifamFaceVariation::Normal, FifamEyeColor::Brown },
{ Face07::Asian5,       FifamSkinColor::Asian,     45, FifamFaceVariation::Normal, FifamEyeColor::Brown },
{ Face07::Asian6,       FifamSkinColor::Asian,     46, FifamFaceVariation::Normal, FifamEyeColor::Brown },
{ Face07::Asian7,       FifamSkinColor::Asian,     44, FifamFaceVariation::Normal, FifamEyeColor::Brown },
{ Face07::Asian8,       FifamSkinColor::Asian,     42, FifamFaceVariation::Normal, FifamEyeColor::Brown },
{ Face07::Asian9,       FifamSkinColor::Asian,     43, FifamFaceVariation::Normal, FifamEyeColor::Brown },
{ Face07::Asian10,      FifamSkinColor::Asian,     41, FifamFaceVariation::Normal, FifamEyeColor::Brown },
{ Face07::Asian11,      FifamSkinColor::Asian,     35, FifamFaceVariation::Normal, FifamEyeColor::Brown },
{ Face07::Asian12,      FifamSkinColor::Asian,     32, FifamFaceVariation::Normal, FifamEyeColor::Brown },
{ Face07::Asian13,      FifamSkinColor::Asian,     29, FifamFaceVariation::Normal, FifamEyeColor::Brown },
{ Face07::Asian14,      FifamSkinColor::Asian,     28, FifamFaceVariation::Normal, FifamEyeColor::Brown },
{ Face07::Asian15,      FifamSkinColor::Asian,     48, FifamFaceVariation::Normal, FifamEyeColor::Brown },
{ Face07::Asian16,      FifamSkinColor::Asian,     30, FifamFaceVariation::Normal, FifamEyeColor::Brown },
{ Face07::Asian17,      FifamSkinColor::Asian,     38, FifamFaceVariation::Normal, FifamEyeColor::Brown }
};

using Hair07to11Info = std::tuple<UChar, UChar>;
static Vector<Hair07to11Info> gHair07to11Mapping = {
{ 1,  3 },
{ 2, 66 },
{ 3, 59 },
{ 4, 67 },
{ 5, 35 },
{ 6, 13 },
{ 7, 82 },
{ 8, 49 },
{ 9, 78 },
{ 10, 48 },
{ 11,  1 },
{ 12, 18 },
{ 13,  4 },
{ 14, 30 },
{ 15, 0x00000000 },
{ 16, 0x00000000 },
{ 17, 0x00000000 },
{ 18, 0x00000000 },
{ 19, 0x00000000 },
{ 20, 0x00000000 },
{ 21, 0x00000000 },
{ 22, 14 },
{ 23, 0x00000000 },
{ 24, 29 },
{ 25, 22 },
{ 26, 0x00000000 },
{ 27, 0x00000000 },
{ 28, 0x00000000 },
{ 29, 0x00000000 },
{ 30, 0x00000000 },
{ 31, 0x00000000 },
{ 32, 65 },
{ 33, 64 },
{ 34, 70 },
{ 35, 0x00000000 },
{ 36, 0x00000000 },
{ 37, 0x00000000 },
{ 38, 0x00000000 }
};

void FifamPlayerAppearance::SetFrom07AppearanceInfo(AppearanceInfo07 const &data) {
    mSkinColor = FifamSkinColor::White;
    mGenericFace = 0;
    mFaceVariation = FifamFaceVariation::Normal;
    mEyeColour = FifamEyeColor::Brown;
    mHairColor = FifamHairColor::Black;
    mHairStyle = 0;
    mSideburns = false;
    mBeardType = FifamBeardType::None;
    mBeardColor = FifamBeardColor::Black;

    // Select face type
    if (data.faceId >= Utils::ToInt(Face07::Caucasian1) && data.faceId <= Utils::ToInt(Face07::Caucasian21)) {
        for (auto &faceInfo : gFace07to11Mapping) {
            if (data.faceId == Utils::ToInt(std::get<0>(faceInfo))) {
                mSkinColor = std::get<1>(faceInfo);
                mGenericFace = std::get<2>(faceInfo) - 1;
                mFaceVariation = std::get<3>(faceInfo);
                mEyeColour = std::get<4>(faceInfo);
                break;
            }
        }
    }

    // Select hair type
    if (data.hairId >= 1 && data.hairId <= 38) {
        for (auto &hairInfo : gHair07to11Mapping) {
            if (std::get<0>(hairInfo) == data.hairId) {
                mHairStyle = std::get<1>(hairInfo);
                break;
            }
        }
    }

    // Select hair colour
    if (data.hairColor >= 1 && data.hairColor <= 5) {
        if (data.hairColor == Utils::ToInt(HairColour07::Brown))
            mHairColor = FifamHairColor::Brown;
        else if (data.hairColor == Utils::ToInt(HairColour07::MediumBlond))
            mHairColor = FifamHairColor::MediumBlond;
        else if (data.hairColor == Utils::ToInt(HairColour07::Blonde))
            mHairColor = FifamHairColor::Blonde;
        else if (data.hairColor == Utils::ToInt(HairColour07::Red))
            mHairColor = FifamHairColor::Red;
        else
            mHairColor = FifamHairColor::Black;
    }

    // Select beard type
    if (data.beardType >= 1 && data.beardType <= 6) {
        if (data.beardType == Utils::ToInt(BeardType07::Goatee))
            mBeardType = FifamBeardType::Goatee;
        if (data.beardType == Utils::ToInt(BeardType07::Moustash))
            mBeardType = FifamBeardType::Moustash;
        if (data.beardType == Utils::ToInt(BeardType07::Shadow))
            mBeardType = FifamBeardType::Shadow;
        if (data.beardType == Utils::ToInt(BeardType07::FullGoatee))
            mBeardType = FifamBeardType::FullGoatee;
        if (data.beardType == Utils::ToInt(BeardType07::Full))
            mBeardType = FifamBeardType::Full;
        else
            mBeardType = FifamBeardType::None;
    }

    // Select beard colour
    if (mBeardType != FifamBeardType::None) {
        if (mHairColor == FifamHairColor::Blonde)
            mBeardColor = FifamBeardColor::Blonde;
        else if (mHairColor == FifamHairColor::Brown)
            mBeardColor = FifamBeardColor::Brown;
        else if (mHairColor == FifamHairColor::MediumBlond)
            mBeardColor = FifamBeardColor::MediumBlonde;
        else if (mHairColor == FifamHairColor::Red)
            mBeardColor = FifamBeardColor::Red;
        else
            mBeardColor = FifamBeardColor::Black;
    }
}

FifamPlayerAppearance::AppearanceInfo07 FifamPlayerAppearance::Get07AppearanceInfo() {
    AppearanceInfo07 result;
    result.faceId = 1;
    result.hairId = 1;
    result.hairColor = Utils::ToInt(HairColour07::Black);
    result.beardType = Utils::ToInt(FifamBeardType::None);

    // Get face id


    // Get hair id


    // Get hair colour


    // Get beard type


    return result;
}
