#pragma once
#include "FifamTypes.h"
#include "FifamPlayerPosition.h"
#include "FifamPlayerPlayingStyle.h"

class FifamPlayer;

class FifamPlayerLevel {
public:
    static Array<Float, 18> GetDefaultBiasValues(FifamPlayerPosition const &position);
    static FifamPlayerPlayingStyle GetBestStyleForPlayer(FifamPlayer *player, Bool includeNewStyles = true);
    static Vector<FifamPlayerPlayingStyle> GetBestStylesForPlayer(FifamPlayer *player, Bool includeNewStyles = true);
    static UChar GetPlayerLevel13(FifamPlayer *player, FifamPlayerPosition position, FifamPlayerPlayingStyle style, Bool experience = true);
    static UChar GetPlayerLevel12(FifamPlayer *player, FifamPlayerPosition position, FifamPlayerPlayingStyle style, Bool experience = true);
    static UChar GetPlayerLevel(FifamPlayer *player, FifamPlayerPosition position, FifamPlayerPlayingStyle style, Bool experience = true);
    static UChar GetPlayerLevel(FifamPlayer *player, FifamPlayerPosition position, Bool experience = true);
    static UChar GetPlayerLevel(FifamPlayer *player, FifamPlayerPlayingStyle style, Bool experience = true);
    static UChar GetPlayerLevel(FifamPlayer *player, Bool experience = true);
    static Int GetPlayerExperienceLevelModifier(FifamPlayer *player);
    static void ValidatePlayerLevel(Int &level);
    static Vector<FifamPlayerPlayingStyle> GetPlayingStylesForPosition(FifamPlayerPosition position, Bool includeNewStyles = true);
};
