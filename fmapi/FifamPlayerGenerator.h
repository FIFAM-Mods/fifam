#pragma once
#include "FifamNation.h"

class FifamPlayer;

class FifamPlayerGenerator {
public:
    static FifamDate GetRandomPlayerBirthday(UShort currentYear);
    FifamPlayer *Generate(FifamNation nationality, UChar averageLevel);
};
