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

    struct {
        // @since FM07
        UChar _1 = 0;

        // @since FM07
        struct {
            // @since FM07
            UInt _1 = 0;

            // @since FM07
            UInt _2 = 0;

            // @since FM07
            UInt _3 = 0;

        } _data1[8];

        // @since FM07
        UChar _2 = 0;

        // @since FM07
        UChar _3 = 0;
    } Unknown;

    void Read(FifamReader &reader);
    void Write(FifamWriter &writer);
};
