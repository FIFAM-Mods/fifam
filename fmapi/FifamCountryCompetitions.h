#pragma once
#include "FifamReadWrite.h"
#include "FifamCompetition.h"

class FifamCountryCompetitions {
public:
    Bool Read(FifamReader &reader);
    Bool Write(FifamWriter &writer);
};
