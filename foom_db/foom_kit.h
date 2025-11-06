#pragma once
#include "FifamTypes.h"
#include "Color.h"

namespace foom {

struct comp;

struct kit {
    Int mKit = 0; // home (0), away (1), gk (2), third (3), gk2 (4), gk3 (5)
    Int mType = 0; // kit part
    Int mKitStyle = 0;
    Int mBackOfShirtStyle = 0;
    Color mForeground;
    Color mBackground;
    Color mOutline;
    Color mNumberColour;
    Color mNumberOutlineColour;
    comp *mCompetition = 0;
    Int mYear = 0;
    Int mAlternativeKitNumber = 0;
};

}
