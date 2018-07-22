#pragma once
#include "FifamTypes.h"
#include "FifamReadWrite.h"

// @since FM07
class FifamClubRecord {
public:
    // @since FM07
    UChar mScore1 = 0;
    // @since FM07
    UChar mScore2 = 0;
    // @since FM07
    UInt mAttendance = 0;
    // @since FM07
    // @maxsize 21
    // @maxsize infinite in FM07
    String mOpponentName;
    // @since FM07
    UShort mYear = 0;

    void Read(FifamReader &reader);
    void Write(FifamWriter &writer);
};
