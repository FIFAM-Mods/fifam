#pragma once
#include "FifamTypes.h"
#include "FifamReadWrite.h"

class FifamCountry;

// @since FM07
class FifamStadium {
public:
    // @since FM07
    // @maxsize 29
    String mName;
    // @since FM07
    // @maxsize 29
    String mCity;
    // @since FM07
    UChar mStadiumType = 0;

    struct Part {
        // @since FM07
        UInt mSeats = 0;
        // @since FM07
        UInt mTerraces = 0;
        // @since FM07
        UInt mVIPSeats = 0;
        // @since FM07
        Bool mSeasonTicketBlock = false;
        // @since FM07
        Bool mAwaySupportersBlock = false;
    };

    // @since FM07
    Array<Part, 8> mParts;

    struct {
        // @since FM07
        UChar _1 = 0;
    } Unknown;

    void Read(FifamReader &reader);
    void Write(FifamWriter &writer);
};
