#pragma once
#include "FifamTypes.h"
#include "FifamCompType.h"
#include "FifamCompRegion.h"

/*
   higher 8 bits | middle 8 bits | lower 16 bits
   ---------------------------------------------
   region id       comp. type      comp. level
*/

class FifamCompID {
public:
    FifamCompRegion mRegion;
    FifamCompType mType;
    UShort mIndex = 0;

    FifamCompID();
    FifamCompID(UInt id);
    FifamCompID(FifamCompRegion const &region, FifamCompType const &type, UShort index);
    void Set(FifamCompRegion const &region, FifamCompType const &type, UShort index);
    FifamCompID(FifamCompRegion const &region, String const &type, UShort index);
    void Set(FifamCompRegion const &region, String const &type, UShort index);
    FifamCompID(UChar regionId, FifamCompType const &type, UShort index);
    FifamCompID(UInt regionId, FifamCompType const &type, UShort index);
    void Set(UChar regionId, FifamCompType const &type, UShort index);
    void Set(UInt regionId, FifamCompType const &type, UShort index);
    void SetFromInt(UInt value);
    UInt ToInt() const;
    void SetFromHexStr(String const &str);
    String ToHexStr() const;
    void SetFromStr(String const &str, FifamCompRegion const &region = FifamCompRegion::None);
    String ToStr(bool countryName = false) const;
    Bool IsNull() const;
};

Bool operator==(FifamCompID const &lhs, FifamCompID const &rhs);
Bool operator!=(FifamCompID const &lhs, FifamCompID const &rhs);
Bool operator<(FifamCompID const &lhs, FifamCompID const &rhs);
Bool operator>(FifamCompID const &lhs, FifamCompID const &rhs);
