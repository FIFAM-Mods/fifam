#pragma once
#include "FifamTypes.h"

class FifamVersion {
    union {
        UInt mIntValue;
        struct {
            UInt mNumber : 16;
            UInt mYear : 16;
        };
    };
public:
    FifamVersion();
    FifamVersion(UShort year, UShort number);
    void Set(UShort year, UShort number);
    void Set(UInt intValue);
    UShort GetYear() const;
    UShort GetNumber() const;
    UInt GetIntValue() const;
    Bool IsGreaterOrEqual(UShort year, UShort number) const;
    String GetString() const;
};
