#pragma once
#include "FifamNation.h"

class FifamPlayer;

class FifamPlayerGenerator {
    FifamPlayer *Generate(FifamNation nationality, UChar averageLevel);
};
