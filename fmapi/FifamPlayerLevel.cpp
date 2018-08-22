#include "FifamPlayerLevel.h"
#include "FifamPlayer.h"

Array<UChar, 18> FifamPlayerLevel::GetDefaultBiasValues(FifamPlayerPosition const &position) {
    Array<UChar, 18> ary;
    switch (position.ToInt()) {
        // None  GK   RB   LB   CB   SW  RWB  LWB  ANC   DM   RM   LM   CM   RW   LW   AM   CF   ST
    default:
        ary = { 0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0 }; break;
    case FifamPlayerPosition::GK:
        ary = { 0, 100,  40,  40,  40,  40,  40,  40,  40,  40,  40,  40,  40,  40,  40,  40,  40,  40 }; break;
    case FifamPlayerPosition::RB:
        ary = { 0,  40, 100,  90,  85,  70,  95,  85,  70,  85,  90,  85,  75,  85,  80,  75,  70,  70 }; break;
    case FifamPlayerPosition::LB:
        ary = { 0,  40,  90, 100,  85,  70,  85,  95,  70,  85,  85,  90,  75,  80,  85,  75,  70,  70 }; break;
    case FifamPlayerPosition::CB:
        ary = { 0,  40,  85,  85, 100,  85,  70,  70,  80,  85,  75,  75,  80,  70,  70,  75,  70,  70 }; break;
    case FifamPlayerPosition::SW:
        ary = { 0,  40,  85,  85,  95, 100,  75,  75,  90,  90,  70,  70,  80,  40,  40,  40,  40,  40 }; break;
    case FifamPlayerPosition::RWB:
        ary = { 0,  40,  95,  85,  80,  70, 100,  95,  70,  85,  90,  85,  75,  85,  80,  75,  70,  70 }; break;
    case FifamPlayerPosition::LWB:
        ary = { 0,  40,  85,  95,  80,  70,  95, 100,  70,  85,  85,  90,  75,  80,  85,  75,  70,  70 }; break;
    case FifamPlayerPosition::ANC:
        ary = { 0,  40,  85,  85,  90,  85,  70,  70, 100,  90,  75,  75,  85,  70,  70,  80,  70,  70 }; break;
    case FifamPlayerPosition::DM:
        ary = { 0,  40,  85,  85,  85,  85,  70,  70,  90, 100,  80,  80,  90,  75,  75,  85,  75,  75 }; break;
    case FifamPlayerPosition::RM:
        ary = { 0,  40,  90,  85,  75,  70,  90,  85,  70,  80, 100,  90,  85,  95,  85,  85,  80,  75 }; break;
    case FifamPlayerPosition::LM:
        ary = { 0,  40,  85,  90,  75,  70,  85,  90,  70,  80,  90, 100,  85,  85,  95,  85,  80,  75 }; break;
    case FifamPlayerPosition::CM:
        ary = { 0,  40,  75,  75,  80,  75,  75,  75,  85,  90,  85,  85, 100,  80,  80,  90,  85,  80 }; break;
    case FifamPlayerPosition::RW:
        ary = { 0,  40,  85,  80,  70,  70,  85,  80,  70,  75,  95,  85,  80, 100,  95,  85,  90,  85 }; break;
    case FifamPlayerPosition::LW:
        ary = { 0,  40,  80,  85,  70,  70,  80,  85,  70,  75,  85,  95,  80,  95, 100,  85,  90,  85 }; break;
    case FifamPlayerPosition::AM:
        ary = { 0,  40,  75,  75,  75,  70,  75,  75,  75,  85,  85,  85,  90,  85,  85, 100,  90,  85 }; break;
    case FifamPlayerPosition::CF:
        ary = { 0,  40,  70,  70,  70,  70,  75,  75,  70,  75,  80,  80,  85,  90,  90,  90, 100,  95 }; break;
    case FifamPlayerPosition::ST:
        ary = { 0,  40,  70,  70,  70,  70,  75,  75,  70,  75,  75,  75,  80,  85,  85,  85,  95, 100 }; break;
        // None  GK   RB   LB   CB   SW  RWB  LWB  ANC   DM   RM   LM   CM   RW   LW   AM   CF   ST
    }
    return ary;
}

FifamPlayerPlayingStyle FifamPlayerLevel::GetBestStyleForPlayer(FifamPlayer *player, bool includeNewStyles) {
    if (player->mMainPosition == FifamPlayerPosition::None || player->mMainPosition == FifamPlayerPosition::GK)
        return FifamPlayerPlayingStyle::None;
    FifamPlayerPlayingStyle result = FifamPlayerPlayingStyle::None;
    Vector<FifamPlayerPlayingStyle> possibleStyles = {
        FifamPlayerPlayingStyle::None,
        FifamPlayerPlayingStyle::AttackingFB,
        FifamPlayerPlayingStyle::DefenceFB,
        FifamPlayerPlayingStyle::Libero,
        FifamPlayerPlayingStyle::SimplePasser,
        FifamPlayerPlayingStyle::BallWinner,
        FifamPlayerPlayingStyle::HardMan,
        FifamPlayerPlayingStyle::Holding,
        FifamPlayerPlayingStyle::BallWinnerMidfield,
        FifamPlayerPlayingStyle::BoxToBox,
        FifamPlayerPlayingStyle::Busy,
        FifamPlayerPlayingStyle::PlayMaker,
        FifamPlayerPlayingStyle::Dribbler,
        FifamPlayerPlayingStyle::Winger,
        FifamPlayerPlayingStyle::TargetMan,
        FifamPlayerPlayingStyle::PenaltyBox,
        FifamPlayerPlayingStyle::RunsChannels,
        FifamPlayerPlayingStyle::PullsWideLeft,
        FifamPlayerPlayingStyle::PullsWideRight,
        FifamPlayerPlayingStyle::DribblerAttack,
        FifamPlayerPlayingStyle::HoldsUp
    };
    if (includeNewStyles) {
        possibleStyles.push_back(FifamPlayerPlayingStyle::BusyAttacker);
        possibleStyles.push_back(FifamPlayerPlayingStyle::TowerStrength);
        possibleStyles.push_back(FifamPlayerPlayingStyle::DistanceShooter);
    }
    float bestLevel = 0.0f;
    for (auto const &style : possibleStyles) {
        float level = player->GetPreciseLevel(player->mMainPosition, style);
        if (level > bestLevel) {
            result = style;
            bestLevel = level;
        }
    }
    return result;
}
