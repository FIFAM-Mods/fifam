#pragma once
#include "FifamDbWriteable.h"
#include "FifamPersonType.h"

class FifamPerson : public FifamDbWriteable {
public:
    UInt mID;
    FifamPersonType mPersonType;
};
