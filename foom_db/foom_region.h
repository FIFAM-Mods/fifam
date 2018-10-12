#pragma once
#include "FifamTypes.h"

namespace foom {

struct region {
    Int mID = 0;
    String mName;

    struct converter_data {
        UInt mFIFAManagerID = 0;
        UInt mFIFAManagerReplacementID = 0;
    } mConverterData;
};

}
