#pragma once
#include "FifamTypes.h"
#include "FifamCompType.h"
#include "FifamCompRegion.h"

/*
   higher 8 bits | middle 8 bits | lower 16 bits
   ---------------------------------------------
   region id       comp. type      comp. level
*/

struct FifamCompID {
    FifamCompRegion mRegion;
    FifamCompType mType;
    UShort mIndex = 0;
    Bool mIsTemplate = false;

    FifamCompID();
    FifamCompID(UInt id);
    FifamCompID(FifamCompRegion const &region, FifamCompType const &type, UShort index, Bool isTemplate = false);
    void Set(FifamCompRegion const &region, FifamCompType const &type, UShort index, Bool isTemplate = false);
    FifamCompID(FifamCompRegion const &region, String const &type, UShort index, Bool isTemplate = false);
    void Set(FifamCompRegion const &region, String const &type, UShort index, Bool isTemplate = false);
    FifamCompID(UChar regionId, FifamCompType const &type, UShort index, Bool isTemplate = false);
    void Set(UChar regionId, FifamCompType const &type, UShort index, Bool isTemplate = false);
    void SetFromInt(UInt value);
    UInt ToInt();
    void SetFromHexStr(String const &str);
    String ToHexStr();
    void SetFromStr(String const &str);
    String ToStr();
    bool IsNull();
    static UInt Translate(UInt id, UInt gameFrom, UInt gameTo);
};
