#pragma once
#include "FifamTypes.h"

// @since FM07
class FifamPlayerAppearance {
public:
    // @since FM07
    UChar mGenericFace = 0; // generic face ID

    // @since FM07
    enum class SkinColour { White = 0, Asian = 1, Latin1 = 2, Latin2 = 3, African1 = 4, African2 = 5, African3 = 6 };

    // @since FM07
    SkinColour mSkinColour = SkinColour::White;

    // @since FM11
    enum class FaceVariation { Normal = 0, Freckles = 1, Wrinkles = 2 };

    // @since FM11
    FaceVariation mFaceVariation = FaceVariation::Normal;

    // @since FM11
    enum class EyeColour { LightBlue = 0, Brown = 1, GrayGreen = 2, Green = 3, GreenBlue = 4, Grey = 5, Blue = 6 };

    // @since FM11
    EyeColour mEyeColour = EyeColour::Brown;

    // @since FM07
    enum class HairColour {
        Blonde = 0,         // FM07: 4
        Black = 1,          // FM07: 1
        MediumBlond = 2,    // FM07: 3
        Darkbrown = 3,      // FM07: -
        Platinumblonde = 4, // FM07: -
        Lightbrown = 5,     // FM07: -
        Brown = 6,          // FM07: 2
        Red = 7             // FM07: 5
    };

    // @since FM07
    HairColour mHairColour = HairColour::Black;

    // @since FM07
    UChar mHairStyle = 0;

    // @since FM11
    Bool mSideburns = false;

    // @since FM07
    enum class BeardType {
        None = 0,       // FM07: 1
        ChinBeard = 1,  // FM07: -
        KevinBeard = 2, // FM07: -
        FullGoatee = 3, // FM07: 5
        Full = 4,       // FM07: 6
        Moustash = 5,   // FM07: 3
        Shadow = 6,     // FM07: 4
        Goatee = 7,     // FM07: 2
        Unshaven = 8    // FM07: -
    };

    // @since FM07
    BeardType mBeardType = BeardType::None;

    // @since FM11
    enum class BeardColour { Black = 0, Blonde = 1, Brown = 2, MediumBlonde = 3, Red = 4 };

    // @since FM11
    BeardColour mBeardColour = BeardColour::Black;

    // @since FM07
    enum class ShoeType {
        White = 1,
        Black = 2,
        Blue = 3,
        Red = 4,
        Yellow = 5,
        WhiteBlue = 6,
        WhiteBlack = 7,
        WhiteLightBlue = 8,
        YellowBlack = 9,
        /* since FM10 */ Green, Grey, Orange, Magenta, BrightRed, DarkBlue, LightGreen, LightYellow, WhiteRed, BlackRed
    } mShoeType = ShoeType::Black;

    struct AppearanceInfo07 {
        UChar faceId = 0;
        UChar hairId = 0;
        UChar hairColor = 0;
        UChar beardType = 0;
        UChar shoeType = 0;
    };

    void Apply07AppearanceInfo(AppearanceInfo07 const &data);
};
