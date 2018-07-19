#pragma once

#include "FifamTypes.h"
#include "FifamNation.h"

class FifamCompetition {
public:
    FifamNation mCountry = FifamNation::None;

    enum Kind {
        Round,
        Pool,
        League,
        Cup,
        Rel1,
        Rel2,
        Rel3,
        RelSwitherland,
        RelScotland1,
        RelScotland2
    } mKind = Kind::Round;

    Char mIndex = 0;

    // Competition ID:
    // { Country, Kind, Index }


};
