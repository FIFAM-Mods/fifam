#pragma once
#include "FifamTypes.h"

class FifamCountry;

// @since FM07
class FifamSponsor {
public:

    struct Unknown {
        // @since FM07
        // @maxsize 28
        String _1;

        // @since FM07
        Int _2;

        // @since FM07
        // @maxsize 31
        String _3;

        // @since FM07
        // @maxsize 31
        String _4;
    };

    FifamCountry *mCountry = nullptr;

};
