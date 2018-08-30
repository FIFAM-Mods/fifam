#pragma once
#include "FifamReadWrite.h"

class FifamCountryCompetitions {
public:
    bool Read(FifamReader &reader);
    bool Write(FifamWriter &writer);
};
