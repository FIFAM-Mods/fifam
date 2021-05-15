#pragma once
#include "FifamPropertied.h"
#include "FifamTypes.h"
#include "FifamReadWrite.h"
#include "FifamDbWriteable.h"

class FifamCountry;

// @since FM07
class FifamStadium : public FifamDbWriteable, public FifamPropertied {
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
        // @since FM03
        // @until FM04
        String _2;
    } Unknown;

    FifamCountry *mCountry = nullptr;

    void Read(FifamReader &reader);
    void Write(FifamWriter &writer);
};
