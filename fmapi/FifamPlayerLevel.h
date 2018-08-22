#pragma once
#include "FifamTypes.h"
#include "FifamPlayerPosition.h"
#include "FifamPlayerPlayingStyle.h"

class FifamPlayer;

class FifamPlayerLevel {
public:
    static Array<Float, 18> GetDefaultBiasValues(FifamPlayerPosition const &position);
    static FifamPlayerPlayingStyle GetBestStyleForPlayer(FifamPlayer *player, bool includeNewStyles = true);
    static UChar GetPlayerLevel13(FifamPlayer *player, FifamPlayerPosition position, FifamPlayerPlayingStyle style, bool experience = true);
    static UChar GetPlayerLevel12(FifamPlayer *player, FifamPlayerPosition position, FifamPlayerPlayingStyle style, bool experience = true);
    static UChar GetPlayerLevel(FifamPlayer *player, FifamPlayerPosition position, FifamPlayerPlayingStyle style, bool experience = true);
    static UChar GetPlayerLevel(FifamPlayer *player, FifamPlayerPosition position, bool experience = true);
    static UChar GetPlayerLevel(FifamPlayer *player, FifamPlayerPlayingStyle style, bool experience = true);
    static UChar GetPlayerLevel(FifamPlayer *player, bool experience = true);
    static Int GetPlayerExperienceLevelModifier(FifamPlayer *player);
    static void ValidatePlayerLevel(Int &level);
};
