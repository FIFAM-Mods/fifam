#pragma once
#include "FifamTypes.h"
#include "FifamEnum.h"

ENUM_BEGIN(FifamPlayerAbilityID, UChar) 
    ENUM_MEMBER( 0, ShotPower,       L"ShotPower")
    ENUM_MEMBER( 1, LongShots,       L"LongShots")
    ENUM_MEMBER( 2, Dribbling,       L"Dribbling")
    ENUM_MEMBER( 3, Technique,       L"Technique")
    ENUM_MEMBER( 4, Crossing,        L"Crossing")
    ENUM_MEMBER( 5, Creativity,      L"Creativity")
    ENUM_MEMBER( 6, Finishing,       L"Finishing")
    ENUM_MEMBER( 7, FreeKicks,       L"FreeKicks")
    ENUM_MEMBER( 8, Corners,         L"Corners")
    ENUM_MEMBER( 9, PenaltyShot,     L"PenaltyShot")
    ENUM_MEMBER(10, Passing,         L"Passing")
    ENUM_MEMBER(11, LongPassing,     L"LongPassing")
    ENUM_MEMBER(12, Flair,           L"Flair")
    ENUM_MEMBER(13, Tackling,        L"Tackling")
    ENUM_MEMBER(14, ManMarking,      L"ManMarking")
    ENUM_MEMBER(15, Header,          L"Header")
    ENUM_MEMBER(16, Touch,           L"Touch")
    ENUM_MEMBER(17, ForwardRuns,     L"ForwardRuns")
    ENUM_MEMBER(18, Acceleration,    L"Acceleration")
    ENUM_MEMBER(19, Pace,            L"Pace")
    ENUM_MEMBER(20, WorkRate,        L"WorkRate")
    ENUM_MEMBER(21, Strength,        L"Strength")
    ENUM_MEMBER(22, Agility,         L"Agility")
    ENUM_MEMBER(23, Jumping,         L"Jumping")
    ENUM_MEMBER(24, Stamina,         L"Stamina")
    ENUM_MEMBER(25, Anticipation,    L"Anticipation")
    ENUM_MEMBER(26, Aggression,      L"Aggression")
    ENUM_MEMBER(27, Composure,       L"Composure")
    ENUM_MEMBER(28, Concentration,   L"Concentration")
    ENUM_MEMBER(29, Decision,        L"Decision")
    ENUM_MEMBER(30, Determination,   L"Determination")
    ENUM_MEMBER(31, TeamWork,        L"TeamWork")
    ENUM_MEMBER(32, Consistency,     L"Consistency")
    ENUM_MEMBER(33, Intimidation,    L"Intimidation")
    ENUM_MEMBER(34, Leadership,      L"Leadership")
    ENUM_MEMBER(35, GkCrosses,       L"GkCrosses")
    ENUM_MEMBER(36, Handling,        L"Handling")
    ENUM_MEMBER(37, GoalKicks,       L"GoalKicks")
    ENUM_MEMBER(38, OneOnOne,        L"OneOnOne")
    ENUM_MEMBER(39, Positioning,     L"Positioning")
    ENUM_MEMBER(40, Punching,        L"Punching")
    ENUM_MEMBER(41, ShotStopping,    L"ShotStopping")
    ENUM_MEMBER(42, Throwing,        L"Throwing")
    ENUM_MEMBER(43, BallControl,     L"BallControl")
    ENUM_MEMBER(44, Volleys,         L"Volleys")
    ENUM_MEMBER(45, TackleStanding,  L"TackleStanding")
    ENUM_MEMBER(46, TackleSliding,   L"TackleSliding")
    ENUM_MEMBER(47, Balance,         L"Balance")
    ENUM_MEMBER(48, PosOffensive,    L"PosOffensive")
    ENUM_MEMBER(49, PosDefensive,    L"PosDefensive")
    ENUM_MEMBER(50, Vision,          L"Vision")
    ENUM_MEMBER(51, Reactions,       L"Reactions")
    ENUM_MEMBER(52, TacticAwareness, L"TacticAwareness")
    ENUM_MEMBER(53, Diving,          L"Diving")
    ENUM_MEMBER(54, Reflexes,        L"Reflexes")
    ENUM_DEFAULT_VALUE(Passing)

    ENUM_READ(reader, str) {
        if (!str.empty()) {
            UChar id = Utils::SafeConvertInt<UChar>(str);
            if (reader.IsVersionGreaterOrEqual(0x2013, 0x01)) {
                static Vector<Pair<UChar, UChar>> table = {
                    {  0, BallControl },
                    {  1, Dribbling },
                    {  2, Finishing },
                    {  3, ShotPower },
                    {  4, LongShots },
                    {  5, Volleys },
                    {  6, Crossing },
                    {  7, Passing },
                    {  8, LongPassing },
                    {  9, Header },
                    { 10, TackleStanding },
                    { 11, TackleSliding },
                    { 12, ManMarking },
                    { 13, Acceleration },
                    { 14, Pace },
                    { 15, Agility },
                    { 16, Jumping },
                    { 17, Strength },
                    { 18, Stamina },
                    { 19, Balance },
                    { 20, PosOffensive },
                    { 21, PosDefensive },
                    { 22, Vision },
                    { 23, Aggression },
                    { 24, Reactions },
                    { 25, Composure },
                    { 26, Consistency },
                    { 27, TacticAwareness },
                    { 28, FreeKicks },
                    { 29, Corners },
                    { 30, PenaltyShot },
                    { 31, Diving },
                    { 32, Handling },
                    { 33, Positioning },
                    { 34, OneOnOne },
                    { 35, Reflexes },
                    { 36, GoalKicks }
                };
                TranslateFrom(id, table);
            }
            else
                SetFromInt(id);
        }
    }

    ENUM_WRITE(writer) {
        if (writer.IsVersionGreaterOrEqual(0x2013, 0x01)) {
            static Vector<Pair<UChar, UChar>> table = {
                {  3, ShotPower },
                {  4, LongShots },
                {  1, Dribbling },
                {  1, Technique },
                {  6, Crossing },
                {  1, Creativity },
                {  2, Finishing },
                { 28, FreeKicks },
                { 29, Corners },
                { 30, PenaltyShot },
                {  7, Passing },
                {  8, LongPassing },
                {  1, Flair },
                { 10, Tackling },
                { 12, ManMarking },
                {  9, Header },
                {  1, Touch },
                { 13, ForwardRuns },
                { 13, Acceleration },
                { 14, Pace },
                { 18, WorkRate },
                { 17, Strength },
                { 15, Agility },
                { 16, Jumping },
                { 18, Stamina },
                { 22, Anticipation },
                { 23, Aggression },
                { 25, Composure },
                { 25, Concentration },
                { 25, Decision },
                { 25, Determination },
                { 18, TeamWork },
                { 26, Consistency },
                { 23, Intimidation },
                { 23, Leadership },
                { 36, GkCrosses },
                { 32, Handling },
                { 36, GoalKicks },
                { 34, OneOnOne },
                { 33, Positioning },
                { 32, Punching },
                { 32, ShotStopping },
                { 32, Throwing },
                {  0, BallControl },
                {  5, Volleys },
                { 10, TackleStanding },
                { 11, TackleSliding },
                { 19, Balance },
                { 20, PosOffensive },
                { 21, PosDefensive },
                { 22, Vision },
                { 24, Reactions },
                { 27, TacticAwareness },
                { 31, Diving },
                { 35, Reflexes }
            };
            writer.WriteOne(TranslateTo<UChar>(table, 7));
        }
        else {
            if (ToInt() >= 43) {
                static Vector<Pair<UChar, UChar>> table = {
                    { Technique, BallControl },
                    { ShotPower, Volleys },
                    { Tackling, TackleStanding },
                    { Tackling, TackleSliding },
                    { Agility, Balance },
                    { Finishing, PosOffensive },
                    { ManMarking, PosDefensive },
                    { Creativity, Vision },
                    { Concentration, Reactions },
                    { Creativity, TacticAwareness },
                    { Jumping, Diving },
                    { ShotStopping, Reflexes }
                };
                writer.WriteOne(TranslateTo<UChar>(table, 7));
            }
            else
                writer.WriteOne(ToInt());
        }
    }
ENUM_END(FifamPlayerAbilityID)
