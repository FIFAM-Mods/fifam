#pragma once
#include "FifamPropertied.h"
#include "FifamTypes.h"
#include "FifamReadWrite.h"

class FifamCountry;

// @since FM07
class FifamSponsor : public FifamPropertied {
public:
    struct {
        // @since FM07
        // @maxsize 28
        String _1;
        // @since FM07
        Int _2 = 0;
        // @since FM07
        // @maxsize 31
        String _3;
        // @since FM07
        // @maxsize 31
        String _4;
    } Unknown;

    FifamSponsor();
    FifamSponsor(FifamSponsor const &rhs);
    
    void Read(FifamReader &reader);
    void Write(FifamWriter &writer);
};
