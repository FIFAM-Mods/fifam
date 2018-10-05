#pragma once
#include "FifamTypes.h"

namespace foom {

struct region {
    Int mID = 0;
    String mName;

    struct converter_data {
        Int mFIFAManagerID = -1;
        Int mFIFAManagerReplacementID = -1;
    } mConverterData;
};

}
