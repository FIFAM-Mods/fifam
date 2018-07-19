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

using Face07to11Info = std::tuple<Face07, FifamPlayerAppearance::SkinColour, UChar, FifamPlayerAppearance::FaceVariation, FifamPlayerAppearance::EyeColour>;
static Vector<Face07to11Info> gFace07to11Mapping = {
{ Face07::Caucasian1,   FifamPlayerAppearance::SkinColour::White,     25, FifamPlayerAppearance::FaceVariation::Normal, FifamPlayerAppearance::EyeColour::Brown },
{ Face07::Caucasian2,   FifamPlayerAppearance::SkinColour::White,     20, FifamPlayerAppearance::FaceVariation::Normal, FifamPlayerAppearance::EyeColour::Brown },
{ Face07::Caucasian3,   FifamPlayerAppearance::SkinColour::White,      3, FifamPlayerAppearance::FaceVariation::Normal, FifamPlayerAppearance::EyeColour::Brown },
{ Face07::Caucasian4,   FifamPlayerAppearance::SkinColour::White,      5, FifamPlayerAppearance::FaceVariation::Normal, FifamPlayerAppearance::EyeColour::Brown },
{ Face07::Caucasian5,   FifamPlayerAppearance::SkinColour::White,     14, FifamPlayerAppearance::FaceVariation::Normal, FifamPlayerAppearance::EyeColour::Brown },
{ Face07::Caucasian6,   FifamPlayerAppearance::SkinColour::White,     24, FifamPlayerAppearance::FaceVariation::Normal, FifamPlayerAppearance::EyeColour::Brown },
{ Face07::Caucasian7,   FifamPlayerAppearance::SkinColour::White,      8, FifamPlayerAppearance::FaceVariation::Normal, FifamPlayerAppearance::EyeColour::Brown },
{ Face07::Caucasian8,   FifamPlayerAppearance::SkinColour::White,     23, FifamPlayerAppearance::FaceVariation::Normal, FifamPlayerAppearance::EyeColour::Brown },
{ Face07::Caucasian9,   FifamPlayerAppearance::SkinColour::White,     19, FifamPlayerAppearance::FaceVariation::Normal, FifamPlayerAppearance::EyeColour::Brown },
{ Face07::Caucasian10,  FifamPlayerAppearance::SkinColour::White,     26, FifamPlayerAppearance::FaceVariation::Normal, FifamPlayerAppearance::EyeColour::Brown },
{ Face07::Caucasian11,  FifamPlayerAppearance::SkinColour::White,     21, FifamPlayerAppearance::FaceVariation::Normal, FifamPlayerAppearance::EyeColour::Brown },
{ Face07::Caucasian12,  FifamPlayerAppearance::SkinColour::White,     10, FifamPlayerAppearance::FaceVariation::Normal, FifamPlayerAppearance::EyeColour::Brown },
{ Face07::Caucasian13,  FifamPlayerAppearance::SkinColour::White,     13, FifamPlayerAppearance::FaceVariation::Normal, FifamPlayerAppearance::EyeColour::Brown },
{ Face07::Caucasian14,  FifamPlayerAppearance::SkinColour::White,    125, FifamPlayerAppearance::FaceVariation::Normal, FifamPlayerAppearance::EyeColour::Brown },
{ Face07::Caucasian15,  FifamPlayerAppearance::SkinColour::White,     22, FifamPlayerAppearance::FaceVariation::Normal, FifamPlayerAppearance::EyeColour::Brown },
{ Face07::Caucasian16,  FifamPlayerAppearance::SkinColour::White,     12, FifamPlayerAppearance::FaceVariation::Normal, FifamPlayerAppearance::EyeColour::Brown },
{ Face07::Caucasian17,  FifamPlayerAppearance::SkinColour::White,     14, FifamPlayerAppearance::FaceVariation::Normal, FifamPlayerAppearance::EyeColour::Brown },
{ Face07::Caucasian18,  FifamPlayerAppearance::SkinColour::White,    126, FifamPlayerAppearance::FaceVariation::Normal, FifamPlayerAppearance::EyeColour::Brown },
{ Face07::Caucasian19,  FifamPlayerAppearance::SkinColour::White,      2, FifamPlayerAppearance::FaceVariation::Normal, FifamPlayerAppearance::EyeColour::Brown },
{ Face07::Caucasian20,  FifamPlayerAppearance::SkinColour::White,      1, FifamPlayerAppearance::FaceVariation::Normal, FifamPlayerAppearance::EyeColour::Brown },
{ Face07::Caucasian21,  FifamPlayerAppearance::SkinColour::White,     94, FifamPlayerAppearance::FaceVariation::Normal, FifamPlayerAppearance::EyeColour::Brown },
{ Face07::EasternEur1,  FifamPlayerAppearance::SkinColour::White,      9, FifamPlayerAppearance::FaceVariation::Normal, FifamPlayerAppearance::EyeColour::Brown },
{ Face07::EasternEur2,  FifamPlayerAppearance::SkinColour::White,     16, FifamPlayerAppearance::FaceVariation::Normal, FifamPlayerAppearance::EyeColour::Brown },
{ Face07::EasternEur3,  FifamPlayerAppearance::SkinColour::White,      7, FifamPlayerAppearance::FaceVariation::Normal, FifamPlayerAppearance::EyeColour::Brown },
{ Face07::EasternEur4,  FifamPlayerAppearance::SkinColour::White,      4, FifamPlayerAppearance::FaceVariation::Normal, FifamPlayerAppearance::EyeColour::Brown },
{ Face07::EasternEur5,  FifamPlayerAppearance::SkinColour::White,     17, FifamPlayerAppearance::FaceVariation::Normal, FifamPlayerAppearance::EyeColour::Brown },
{ Face07::EasternEur6,  FifamPlayerAppearance::SkinColour::White,    102, FifamPlayerAppearance::FaceVariation::Normal, FifamPlayerAppearance::EyeColour::Brown },
{ Face07::EasternEur7,  FifamPlayerAppearance::SkinColour::White,      6, FifamPlayerAppearance::FaceVariation::Normal, FifamPlayerAppearance::EyeColour::Brown },
{ Face07::EasternEur8,  FifamPlayerAppearance::SkinColour::White,    126, FifamPlayerAppearance::FaceVariation::Normal, FifamPlayerAppearance::EyeColour::Brown },
{ Face07::EasternEur9,  FifamPlayerAppearance::SkinColour::White,    121, FifamPlayerAppearance::FaceVariation::Normal, FifamPlayerAppearance::EyeColour::Brown },
{ Face07::EasternEur10, FifamPlayerAppearance::SkinColour::White,    119, FifamPlayerAppearance::FaceVariation::Normal, FifamPlayerAppearance::EyeColour::Brown },
{ Face07::EasternEur11, FifamPlayerAppearance::SkinColour::White,    124, FifamPlayerAppearance::FaceVariation::Normal, FifamPlayerAppearance::EyeColour::Brown },
{ Face07::EasternEur12, FifamPlayerAppearance::SkinColour::White,    122, FifamPlayerAppearance::FaceVariation::Normal, FifamPlayerAppearance::EyeColour::Brown },
{ Face07::Latin1,       FifamPlayerAppearance::SkinColour::Latin1,   103, FifamPlayerAppearance::FaceVariation::Normal, FifamPlayerAppearance::EyeColour::Brown },
{ Face07::Latin2,       FifamPlayerAppearance::SkinColour::Latin1,    86, FifamPlayerAppearance::FaceVariation::Normal, FifamPlayerAppearance::EyeColour::Brown },
{ Face07::Latin3,       FifamPlayerAppearance::SkinColour::Latin1,    88, FifamPlayerAppearance::FaceVariation::Normal, FifamPlayerAppearance::EyeColour::Brown },
{ Face07::Latin4,       FifamPlayerAppearance::SkinColour::Latin1,    90, FifamPlayerAppearance::FaceVariation::Normal, FifamPlayerAppearance::EyeColour::Brown },
{ Face07::Latin5,       FifamPlayerAppearance::SkinColour::Latin1,    74, FifamPlayerAppearance::FaceVariation::Normal, FifamPlayerAppearance::EyeColour::Brown },
{ Face07::Latin6,       FifamPlayerAppearance::SkinColour::Latin1,    89, FifamPlayerAppearance::FaceVariation::Normal, FifamPlayerAppearance::EyeColour::Brown },
{ Face07::Latin7,       FifamPlayerAppearance::SkinColour::White,     71, FifamPlayerAppearance::FaceVariation::Normal, FifamPlayerAppearance::EyeColour::Brown },
{ Face07::Latin8,       FifamPlayerAppearance::SkinColour::Latin1,   110, FifamPlayerAppearance::FaceVariation::Normal, FifamPlayerAppearance::EyeColour::Brown },
{ Face07::Latin9,       FifamPlayerAppearance::SkinColour::Latin1,    87, FifamPlayerAppearance::FaceVariation::Normal, FifamPlayerAppearance::EyeColour::Brown },
{ Face07::Latin10,      FifamPlayerAppearance::SkinColour::Latin1,   111, FifamPlayerAppearance::FaceVariation::Normal, FifamPlayerAppearance::EyeColour::Brown },
{ Face07::Latin11,      FifamPlayerAppearance::SkinColour::Latin1,    96, FifamPlayerAppearance::FaceVariation::Normal, FifamPlayerAppearance::EyeColour::Brown },
{ Face07::Latin12,      FifamPlayerAppearance::SkinColour::Latin1,    73, FifamPlayerAppearance::FaceVariation::Normal, FifamPlayerAppearance::EyeColour::Brown },
{ Face07::Latin13,      FifamPlayerAppearance::SkinColour::Latin1,    75, FifamPlayerAppearance::FaceVariation::Normal, FifamPlayerAppearance::EyeColour::Brown },
{ Face07::Latin14,      FifamPlayerAppearance::SkinColour::Latin1,    79, FifamPlayerAppearance::FaceVariation::Normal, FifamPlayerAppearance::EyeColour::Brown },
{ Face07::Latin15,      FifamPlayerAppearance::SkinColour::Latin1,    84, FifamPlayerAppearance::FaceVariation::Normal, FifamPlayerAppearance::EyeColour::Brown },
{ Face07::Latin16,      FifamPlayerAppearance::SkinColour::Latin1,    70, FifamPlayerAppearance::FaceVariation::Normal, FifamPlayerAppearance::EyeColour::Brown },
{ Face07::Latin17,      FifamPlayerAppearance::SkinColour::Latin1,    88, FifamPlayerAppearance::FaceVariation::Normal, FifamPlayerAppearance::EyeColour::Brown },
{ Face07::Latin18,      FifamPlayerAppearance::SkinColour::Latin1,    80, FifamPlayerAppearance::FaceVariation::Normal, FifamPlayerAppearance::EyeColour::Brown },
{ Face07::Arabic1,      FifamPlayerAppearance::SkinColour::Latin2,    82, FifamPlayerAppearance::FaceVariation::Normal, FifamPlayerAppearance::EyeColour::Brown },
{ Face07::Arabic2,      FifamPlayerAppearance::SkinColour::Latin2,    78, FifamPlayerAppearance::FaceVariation::Normal, FifamPlayerAppearance::EyeColour::Brown },
{ Face07::Arabic3,      FifamPlayerAppearance::SkinColour::Latin2,    77, FifamPlayerAppearance::FaceVariation::Normal, FifamPlayerAppearance::EyeColour::Brown },
{ Face07::Arabic4,      FifamPlayerAppearance::SkinColour::Latin2,    68, FifamPlayerAppearance::FaceVariation::Normal, FifamPlayerAppearance::EyeColour::Brown },
{ Face07::Arabic5,      FifamPlayerAppearance::SkinColour::Latin1,    81, FifamPlayerAppearance::FaceVariation::Normal, FifamPlayerAppearance::EyeColour::Brown },
{ Face07::Arabic6,      FifamPlayerAppearance::SkinColour::Latin2,    93, FifamPlayerAppearance::FaceVariation::Normal, FifamPlayerAppearance::EyeColour::Brown },
{ Face07::African1,     FifamPlayerAppearance::SkinColour::African2,   6, FifamPlayerAppearance::FaceVariation::Normal, FifamPlayerAppearance::EyeColour::Brown },
{ Face07::African2,     FifamPlayerAppearance::SkinColour::African3,  52, FifamPlayerAppearance::FaceVariation::Normal, FifamPlayerAppearance::EyeColour::Brown },
{ Face07::African3,     FifamPlayerAppearance::SkinColour::African3,  49, FifamPlayerAppearance::FaceVariation::Normal, FifamPlayerAppearance::EyeColour::Brown },
{ Face07::African4,     FifamPlayerAppearance::SkinColour::African2,  51, FifamPlayerAppearance::FaceVariation::Normal, FifamPlayerAppearance::EyeColour::Brown },
{ Face07::African5,     FifamPlayerAppearance::SkinColour::African2,  55, FifamPlayerAppearance::FaceVariation::Normal, FifamPlayerAppearance::EyeColour::Brown },
{ Face07::African6,     FifamPlayerAppearance::SkinColour::African2,  57, FifamPlayerAppearance::FaceVariation::Normal, FifamPlayerAppearance::EyeColour::Brown },
{ Face07::African7,     FifamPlayerAppearance::SkinColour::African2, 131, FifamPlayerAppearance::FaceVariation::Normal, FifamPlayerAppearance::EyeColour::Brown },
{ Face07::African8,     FifamPlayerAppearance::SkinColour::African2,  68, FifamPlayerAppearance::FaceVariation::Normal, FifamPlayerAppearance::EyeColour::Brown },
{ Face07::African9,     FifamPlayerAppearance::SkinColour::African2,  56, FifamPlayerAppearance::FaceVariation::Normal, FifamPlayerAppearance::EyeColour::Brown },
{ Face07::African10,    FifamPlayerAppearance::SkinColour::African2,  50, FifamPlayerAppearance::FaceVariation::Normal, FifamPlayerAppearance::EyeColour::Brown },
{ Face07::African11,    FifamPlayerAppearance::SkinColour::African2,  66, FifamPlayerAppearance::FaceVariation::Normal, FifamPlayerAppearance::EyeColour::Brown },
{ Face07::African12,    FifamPlayerAppearance::SkinColour::African1,  54, FifamPlayerAppearance::FaceVariation::Normal, FifamPlayerAppearance::EyeColour::Brown },
{ Face07::African13,    FifamPlayerAppearance::SkinColour::African2, 134, FifamPlayerAppearance::FaceVariation::Normal, FifamPlayerAppearance::EyeColour::Brown },
{ Face07::African14,    FifamPlayerAppearance::SkinColour::African2, 114, FifamPlayerAppearance::FaceVariation::Normal, FifamPlayerAppearance::EyeColour::Brown },
{ Face07::African15,    FifamPlayerAppearance::SkinColour::African2,  59, FifamPlayerAppearance::FaceVariation::Normal, FifamPlayerAppearance::EyeColour::Brown },
{ Face07::African16,    FifamPlayerAppearance::SkinColour::African2, 135, FifamPlayerAppearance::FaceVariation::Normal, FifamPlayerAppearance::EyeColour::Brown },
{ Face07::Asian1,       FifamPlayerAppearance::SkinColour::Asian,     39, FifamPlayerAppearance::FaceVariation::Normal, FifamPlayerAppearance::EyeColour::Brown },
{ Face07::Asian2,       FifamPlayerAppearance::SkinColour::Asian,     40, FifamPlayerAppearance::FaceVariation::Normal, FifamPlayerAppearance::EyeColour::Brown },
{ Face07::Asian3,       FifamPlayerAppearance::SkinColour::Asian,     37, FifamPlayerAppearance::FaceVariation::Normal, FifamPlayerAppearance::EyeColour::Brown },
{ Face07::Asian4,       FifamPlayerAppearance::SkinColour::Asian,     47, FifamPlayerAppearance::FaceVariation::Normal, FifamPlayerAppearance::EyeColour::Brown },
{ Face07::Asian5,       FifamPlayerAppearance::SkinColour::Asian,     45, FifamPlayerAppearance::FaceVariation::Normal, FifamPlayerAppearance::EyeColour::Brown },
{ Face07::Asian6,       FifamPlayerAppearance::SkinColour::Asian,     46, FifamPlayerAppearance::FaceVariation::Normal, FifamPlayerAppearance::EyeColour::Brown },
{ Face07::Asian7,       FifamPlayerAppearance::SkinColour::Asian,     44, FifamPlayerAppearance::FaceVariation::Normal, FifamPlayerAppearance::EyeColour::Brown },
{ Face07::Asian8,       FifamPlayerAppearance::SkinColour::Asian,     42, FifamPlayerAppearance::FaceVariation::Normal, FifamPlayerAppearance::EyeColour::Brown },
{ Face07::Asian9,       FifamPlayerAppearance::SkinColour::Asian,     43, FifamPlayerAppearance::FaceVariation::Normal, FifamPlayerAppearance::EyeColour::Brown },
{ Face07::Asian10,      FifamPlayerAppearance::SkinColour::Asian,     41, FifamPlayerAppearance::FaceVariation::Normal, FifamPlayerAppearance::EyeColour::Brown },
{ Face07::Asian11,      FifamPlayerAppearance::SkinColour::Asian,     35, FifamPlayerAppearance::FaceVariation::Normal, FifamPlayerAppearance::EyeColour::Brown },
{ Face07::Asian12,      FifamPlayerAppearance::SkinColour::Asian,     32, FifamPlayerAppearance::FaceVariation::Normal, FifamPlayerAppearance::EyeColour::Brown },
{ Face07::Asian13,      FifamPlayerAppearance::SkinColour::Asian,     29, FifamPlayerAppearance::FaceVariation::Normal, FifamPlayerAppearance::EyeColour::Brown },
{ Face07::Asian14,      FifamPlayerAppearance::SkinColour::Asian,     28, FifamPlayerAppearance::FaceVariation::Normal, FifamPlayerAppearance::EyeColour::Brown },
{ Face07::Asian15,      FifamPlayerAppearance::SkinColour::Asian,     48, FifamPlayerAppearance::FaceVariation::Normal, FifamPlayerAppearance::EyeColour::Brown },
{ Face07::Asian16,      FifamPlayerAppearance::SkinColour::Asian,     30, FifamPlayerAppearance::FaceVariation::Normal, FifamPlayerAppearance::EyeColour::Brown },
{ Face07::Asian17,      FifamPlayerAppearance::SkinColour::Asian,     38, FifamPlayerAppearance::FaceVariation::Normal, FifamPlayerAppearance::EyeColour::Brown }
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

void FifamPlayerAppearance::Apply07AppearanceInfo(AppearanceInfo07 const &data) {
    mSkinColour = SkinColour::White;
    mGenericFace = 0;
    mFaceVariation = FaceVariation::Normal;
    mEyeColour = EyeColour::Brown;
    mHairColour = HairColour::Black;
    mHairStyle = 0;
    mSideburns = false;
    mBeardType = BeardType::None;
    mBeardColour = BeardColour::Black;

    // Select face type
    if (data.faceId >= Utils::ToInt(Face07::Caucasian1) && data.faceId <= Utils::ToInt(Face07::Caucasian21)) {
        for (auto &faceInfo : gFace07to11Mapping) {
            if (data.faceId == Utils::ToInt(std::get<0>(faceInfo))) {
                mSkinColour = std::get<1>(faceInfo);
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
            mHairColour = HairColour::Brown;
        else if (data.hairColor == Utils::ToInt(HairColour07::MediumBlond))
            mHairColour = HairColour::MediumBlond;
        else if (data.hairColor == Utils::ToInt(HairColour07::Blonde))
            mHairColour = HairColour::Blonde;
        else if (data.hairColor == Utils::ToInt(HairColour07::Red))
            mHairColour = HairColour::Red;
        else
            mHairColour = HairColour::Black;
    }

    // Select beard type
    if (data.beardType >= 1 && data.beardType <= 6) {
        if (data.beardType == Utils::ToInt(BeardType07::Goatee))
            mBeardType = BeardType::Goatee;
        if (data.beardType == Utils::ToInt(BeardType07::Moustash))
            mBeardType = BeardType::Moustash;
        if (data.beardType == Utils::ToInt(BeardType07::Shadow))
            mBeardType = BeardType::Shadow;
        if (data.beardType == Utils::ToInt(BeardType07::FullGoatee))
            mBeardType = BeardType::FullGoatee;
        if (data.beardType == Utils::ToInt(BeardType07::Full))
            mBeardType = BeardType::Full;
        else
            mBeardType = BeardType::None;
    }

    // Select beard colour
    if (mBeardType != BeardType::None) {
        if (mHairColour == HairColour::Blonde)
            mBeardColour = BeardColour::Blonde;
        else if (mHairColour == HairColour::Brown)
            mBeardColour = BeardColour::Brown;
        else if (mHairColour == HairColour::MediumBlond)
            mBeardColour = BeardColour::MediumBlonde;
        else if (mHairColour == HairColour::Red)
            mBeardColour = BeardColour::Red;
        else
            mBeardColour = BeardColour::Black;
    }

    // Select shoe type
    //if (data.shoeType >= 1 && data.shoeType <= 9)
    //    mShoeType = Utils::FromInt<ShoeType>(data.shoeType);
}

FifamPlayerAppearance::AppearanceInfo07 FifamPlayerAppearance::Get07AppearanceInfo() {
    AppearanceInfo07 result;
    result.faceId = 1;
    result.hairId = 1;
    result.hairColor = Utils::ToInt(HairColour07::Black);
    result.beardType = Utils::ToInt(BeardType::None);

    // Get face id


    // Get hair id


    // Get hair colour


    // Get beard type


    return result;
}
