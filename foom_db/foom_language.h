#pragma once
#include "FifamTypes.h"

namespace foom {

struct nation;

struct language {
    Int mID = 0;
    String mName;
    nation *mNation = nullptr;

    struct converter_data {
        UInt mFIFAManagerID = 0;
        UInt mFIFAManagerReplacementID = 0;
    } mConverterData;
};

}
