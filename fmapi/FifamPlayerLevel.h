#pragma once
#include "FifamTypes.h"
#include "FifamPlayerPosition.h"
#include "FifamPlayerPlayingStyle.h"

class FifamPlayer;

class FifamPlayerLevel {
public:
    static Array<UChar, 18> GetDefaultBiasValues(FifamPlayerPosition const &position);
    static FifamPlayerPlayingStyle GetBestStyleForPlayer(FifamPlayer *player, bool includeNewStyles = true);
};
