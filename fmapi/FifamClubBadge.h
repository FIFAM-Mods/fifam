#pragma once
#include "FifamTypes.h"
#include "FifamReadWrite.h"
#include "FifamClubBadgeShape.h"
#include "FifamClubBadgeType.h"
#include "Color.h"

class FifamClubBadge {
    FifamClubBadgeType mType;
    FifamClubBadgeShape mShape;
    UChar mShapeVariation = 1;
    UChar mPattern = 1;
    Bool mInverted = true;
    ColorPair mColor;
    String mClubFileName;
public:
    void SetColor(FifamClubBadgeShape shape, UChar shapeVariation, UChar pattern, Bool inverted, ColorPair const &color);
    void SetColor(FifamClubBadgeShape shape, UChar shapeVariation, UChar pattern, Bool inverted);
    void SetColorDefault(ColorPair const &color);
    void SetClub(String const &clubFileName);
    void SetNoBadge();
    void SetEALogo();
    void Validate();
    String GetBadgePath(UInt gameId);
    String ToStr();
    void SetBadgePath(String const &str);
    void Read(FifamReader &reader);
    void Write(FifamWriter &writer);
};
