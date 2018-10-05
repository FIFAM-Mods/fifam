#pragma once
#include "FifamTypes.h"

namespace foom {

struct nation;

struct language {
    Int mID = 0;
    String mName;
    nation *mNation = nullptr;

    struct converter_data {
        Int mFIFAManagerID = -1;
        Int mFIFAManagerReplacementID = -1;
    } mConverterData;
};

}
