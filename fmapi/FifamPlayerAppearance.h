#pragma once
#include "FifamTypes.h"
#include "FifamEnum.h"

ENUM_BEGIN(FifamSkinColor, UChar)
    ENUM_MEMBER(0, White,    L"White")
    ENUM_MEMBER(1, Asian,    L"Asian")
    ENUM_MEMBER(2, Latin1,   L"Latin1")
    ENUM_MEMBER(3, Latin2,   L"Latin2")
    ENUM_MEMBER(4, African1, L"African1")
    ENUM_MEMBER(5, African2, L"African2")
    ENUM_MEMBER(6, African3, L"African3")
    ENUM_DEFAULT_VALUE(White)
    ENUM_DEFAULT_READ_WRITE
ENUM_END(FifamSkinColor)

ENUM_BEGIN(FifamFaceVariation, UChar)
    ENUM_MEMBER(0, Normal,   L"Normal")
    ENUM_MEMBER(1, Freckles, L"Freckles")
    ENUM_MEMBER(2, Wrinkles, L"Wrinkles")
    ENUM_DEFAULT_VALUE(Normal)
    ENUM_DEFAULT_READ_WRITE
ENUM_END(FifamFaceVariation)

ENUM_BEGIN(FifamEyeColor, UChar)
    ENUM_MEMBER(0, LightBlue, L"LightBlue")
    ENUM_MEMBER(1, Brown,     L"Brown")
    ENUM_MEMBER(2, GrayGreen, L"GrayGreen")
    ENUM_MEMBER(3, Green,     L"Green")
    ENUM_MEMBER(4, GreenBlue, L"GreenBlue")
    ENUM_MEMBER(5, Grey,      L"Grey")
    ENUM_MEMBER(6, Blue,      L"Blue")
    ENUM_DEFAULT_VALUE(Brown)
    ENUM_DEFAULT_READ_WRITE
ENUM_END(FifamEyeColor)

ENUM_BEGIN(FifamHairColor, UChar)
    ENUM_MEMBER(0, Blonde,         L"Blonde")
    ENUM_MEMBER(1, Black,          L"Black")
    ENUM_MEMBER(2, MediumBlond,    L"MediumBlond")
    ENUM_MEMBER(3, Darkbrown,      L"Darkbrown")
    ENUM_MEMBER(4, Platinumblonde, L"Platinumblonde")
    ENUM_MEMBER(5, Lightbrown,     L"Lightbrown")
    ENUM_MEMBER(6, Brown,          L"Brown")
    ENUM_MEMBER(7, Red,            L"Red")
    ENUM_DEFAULT_VALUE(Black)
    ENUM_DEFAULT_READ_WRITE
ENUM_END(FifamHairColor)

ENUM_BEGIN(FifamBeardType, UChar)
    ENUM_MEMBER(0, None,       L"None")
    ENUM_MEMBER(1, ChinBeard,  L"ChinBeard")
    ENUM_MEMBER(2, KevinBeard, L"KevinBeard")
    ENUM_MEMBER(3, FullGoatee, L"FullGoatee")
    ENUM_MEMBER(4, Full,       L"Full")
    ENUM_MEMBER(5, Moustash,   L"Moustash")
    ENUM_MEMBER(6, Shadow,     L"Shadow")
    ENUM_MEMBER(7, Goatee,     L"Goatee")
    ENUM_MEMBER(8, Unshaven,   L"Unshaven")
    ENUM_DEFAULT_VALUE(None)
    ENUM_DEFAULT_READ_WRITE
ENUM_END(FifamBeardType)

ENUM_BEGIN(FifamBeardColor, UChar)
    ENUM_MEMBER(0, Black,        L"Black")
    ENUM_MEMBER(1, Blonde,       L"Blonde")
    ENUM_MEMBER(2, Brown,        L"Brown")
    ENUM_MEMBER(3, MediumBlonde, L"MediumBlonde")
    ENUM_MEMBER(4, Red,          L"Red")
    ENUM_DEFAULT_VALUE(Black)
    ENUM_DEFAULT_READ_WRITE
ENUM_END(FifamBeardColor)

// @since FM07
class FifamPlayerAppearance {
public:
    // @since FM07
    UChar mGenericFace = 0; // generic face ID
    // @since FM11
    FifamSkinColor mSkinColor;
    // @since FM11
    FifamFaceVariation mFaceVariation;
    // @since FM11
    FifamEyeColor mEyeColour;
    // @since FM07
    FifamHairColor mHairColor;
    // @since FM07
    UChar mHairStyle = 0;
    // @since FM11
    Bool mSideburns = false;
    // @since FM07
    FifamBeardType mBeardType;
    // @since FM11
    FifamBeardColor mBeardColor;

    struct AppearanceInfo07 {
        UChar faceId = 0;
        UChar hairId = 0;
        UChar hairColor = 0;
        UChar beardType = 0;
    };

    static UChar GetHairIdFromEditor14Id(UChar hairId);
    void SetFrom07AppearanceInfo(AppearanceInfo07 const &data);
    AppearanceInfo07 Get07AppearanceInfo();
};
