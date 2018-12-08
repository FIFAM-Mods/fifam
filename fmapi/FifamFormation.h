#pragma once
#include "FifamTypes.h"
#include "FifamEnum.h"

ENUM_BEGIN(FifamFormation, UChar)
    ENUM_MEMBER( 0, None,                     L"None")
    ENUM_MEMBER( 1, _5_4_1,                   L"5-4-1")                   // LB-CD-CD-CD-LB | LM-DM-DM-RM | ST
    ENUM_MEMBER( 2, _5_3_2,                   L"5-3-2")                   // LWB-CD-SW-CD-RWB | CM-DM-CM | ST-ST
    ENUM_MEMBER( 3, _4_5_1_Defensive,         L"4-5-1 Defensive")         // LB-CD-CD-RB | LM-DM-CM-DM-RM | ST
    ENUM_MEMBER( 4, _4_5_1_Normal,            L"4-5-1 Normal")            // LB-CD-CD-RB | LM-CM-DM-CM-RM | ST
    ENUM_MEMBER( 5, _4_5_1_Classic,           L"4-5-1 Classic")           // LB-CD-CD-RB | LM-AM-DM-AM-RM | ST
    ENUM_MEMBER( 6, _4_5_1_Attacking,         L"4-5-1 Attacking")         // LWB-CD-CD-RWB | LM-CM-AM-CM-RM | ST
    ENUM_MEMBER( 7, _4_1_4_1,                 L"4-1-4-1")                 // LB-CD-CD-RB | DM | LM-CM-CM-RM | ST
    ENUM_MEMBER( 8, _4_4_1_1_Defensive,       L"4-4-1-1 Defensive")       // LB-CD-CD-RB | LM-DM-DM-RM | AM | ST
    ENUM_MEMBER( 9, _4_4_2_Defensive,         L"4-4-2 Defensive")         // LB-CD-CD-RB | LM-DM-DM-RM | ST-ST
    ENUM_MEMBER(10, _4_4_2_Normal,            L"4-4-2 Normal")            // LB-CD-CD-RB | LM-CM-CM-RM | ST-ST
    ENUM_MEMBER(11, _4_4_2_Attacking,         L"4-4-2 Attacking")         // LWB-CD-CD-RWB | LM-CM-CM-RM | ST-ST
    ENUM_MEMBER(12, _4_4_2_Diamond_Low,       L"4-4-2 Diamond_Low")       // LB-CD-CD-RB | LM-DM-CM-RM | ST-ST
    ENUM_MEMBER(13, _4_4_2_Diamond,           L"4-4-2 Diamond")           // LB-CD-CD-RB | LM-DM-AM-RM | ST-ST
    ENUM_MEMBER(14, _4_4_2_Diamond_Attacking, L"4-4-2 Diamond_Attacking") // LB-CD-CD-RB | LM-CM-AM-RM | ST-ST
    ENUM_MEMBER(15, _4_3_3_Defensive,         L"4-3-3 Defensive")         // LB-CD-CD-RB | DM-CM-DM | CF-ST-CF
    ENUM_MEMBER(16, _4_3_3_Normal,            L"4-3-3 Normal")            // LB-CD-CD-RB | LM-DM-RM | CF-ST-CF
    ENUM_MEMBER(17, _4_3_3_Attacking,         L"4-3-3 Attacking")         // LWB-CD-CD-RWB | CM-AM-CM | LW-ST-RW
    ENUM_MEMBER(18, _4_3_3_Wings,             L"4-3-3 Wings")             // LB-CD-CD-RB | CM-DM-CM | LW-ST-RW
    ENUM_MEMBER(19, _4_2_4,                   L"4-2-4")                   // LB-CD-CD-RB | DM-DM | LW-CF-ST-RW
    ENUM_MEMBER(20, _3_5_2_Sweeper,           L"3-5-2 Sweeper")           // CD-SW-CD | LM-DM-CM-AM-RM | ST-ST
    ENUM_MEMBER(21, _3_5_2,                   L"3-5-2")                   // CD-CD-CD | LM-DM-AM-DM-RM | ST-ST
    ENUM_MEMBER(22, _3_4_3_Defensive,         L"3-4-3 Defensive")         // CD-CD-CD | LM-DM-DM-RM | CF-ST-CF
    ENUM_MEMBER(23, _3_4_3_Diamond,           L"3-4-3 Diamond")           // CD-CD-CD | LM-DM-AM-RM | CF-ST-CF
    ENUM_MEMBER(24, _3_4_3_Wings,             L"3-4-3 Wings")             // CD-CD-CD | LM-DM-AM-RM | LW-ST-RW
    ENUM_MEMBER(25, _4_2_2_1_1_V2,            L"4-2-2-1-1 V2")            // LB-CD-CD-RB | DM-DM | LM-RM | CF | CF
    ENUM_MEMBER(26, _4_2_2_1_1,               L"4-2-2-1-1")               // LB-CD-CD-RB | DM-DM | LM-RM | CF | ST
    ENUM_MEMBER(27, _4_2_2_1_1_V3,            L"4-2-2-1-1 V3")            // LB-CD-CD-RB | DM-CM | LM-RM | CF | ST
    ENUM_MEMBER(28, _4_5_1_Attacking_V2,      L"4-5-1 Attacking V2")      // LB-CD-CD-RB | LM-DM-AM-DM-RM | ST
    ENUM_MEMBER(29, _4_3_3_Defensive_V2,      L"4-3-3 Defensive V2")      // LB-CD-CD-RB | DM-CM-DM | LW-ST-RW
    ENUM_MEMBER(30, _4_3_3_Normal_V2,         L"4-3-3 Normal V2")         // LB-CD-CD-RB | LM-DM-RM | LW-ST-RW
    ENUM_MEMBER(31, _4_3_3_Attacking_V2,      L"4-3-3 Attacking V2")      // LWB-CD-CD-RWB | DM-AM-DM | LW-ST-RW
    ENUM_MEMBER(32, _3_4_3_Wing_Defenders,    L"3-4-3 Wing Defenders")    // LB-CD-RB | LM-DM-AM-RM | LW-ST-RW ; FM10 only
    ENUM_MEMBER(33, _3_5_2_Defenders,         L"3-5-2 Defenders")         // LB-CD-RB | LM-DM-AM-DM-RM | ST-ST ; FM10 only
    ENUM_MEMBER(34, _5_2_1_2,                 L"5-2-1-2")                 // LWB-CD-CD-CD-RWB | CM-CM | CF | ST-ST ; FM07-08 only
    ENUM_MEMBER(35, _5_2_2_1,                 L"5-2-2-1")                 // LWB-CD-CD-CD-RWB | CM-CM | CF-CF | ST ; FM07-08 only
    ENUM_MEMBER(36, _5_3_2_Attacking,         L"5-3-2 Attacking")         // LWB-CD-CD-CD-RWB | CM-AM-CM | ST-ST ; FM07-08 only

    ENUM_DEFAULT_VALUE(None)

    ENUM_READ(reader, str) {
        if (!str.empty()) {
            UChar id = Utils::SafeConvertInt<UChar>(str);
            if (reader.GetGameId() <= 8) {
                static Vector<Pair<UChar, UChar>> table = {
                    { 0,  _4_3_3_Wings },
                    { 1,  _5_3_2 },
                    { 2,  _3_4_3_Wings },
                    { 3,  _3_4_3_Diamond },
                    { 4,  _3_4_3_Defensive },
                    { 5,  _3_5_2 },
                    { 6,  _4_4_2_Diamond },
                    { 7,  _4_2_2_1_1_V3 },
                    { 8,  _4_2_4 },
                    { 9,  _4_4_2_Diamond_Attacking },
                    { 10, _4_3_3_Normal },
                    { 11, _4_3_3_Defensive },
                    { 12, _4_4_1_1_Defensive },
                    { 13, _4_4_2_Normal },
                    { 14, _4_5_1_Classic },
                    { 15, _5_2_1_2 },
                    { 16, _5_2_2_1 },
                    { 17, _5_3_2_Attacking },
                    { 18, _5_4_1 }
                };
                TranslateFrom(id, table);
            }
            else if (reader.GetGameId() >= 12) {
                static Vector<Pair<UChar, UChar>> table = {
                    { 0,  None },
                    { 1,  _4_4_2_Diamond },
                    { 2,  _4_3_3_Attacking_V2 },
                    { 3,  _4_3_3_Wings },
                    { 4,  _4_5_1_Defensive },
                    { 5,  _4_4_2_Normal },
                    { 6,  _4_4_1_1_Defensive },
                    { 7,  _4_4_2_Defensive },
                    { 8,  _4_1_4_1 },
                    { 9,  _4_3_3_Normal },
                    { 10, _4_2_4 },
                    { 18, _4_5_1_Attacking_V2 }
                };
                TranslateFrom(id, table);
            }
            else if (reader.GetGameId() == 9) {
                if (id <= 24)
                    SetFromInt(id);
                else
                    SetDefaultValue();
            }
            else if (reader.GetGameId() == 10) {
                if (id <= 24)
                    SetFromInt(id);
                else if (id == 25)
                    SetFromInt(_3_4_3_Wing_Defenders);
                else if (id == 26)
                    SetFromInt(_3_5_2_Defenders);
                else
                    SetDefaultValue();
            }
            else if (reader.GetGameId() == 11) {
                if (id <= 31)
                    SetFromInt(id);
                else
                    SetDefaultValue();
            }
        }
    }

    UChar GetIdForGame(UInt gameId) {
        if (gameId <= 8) {
            static Vector<Pair<UChar, UChar>> table = {
                { 11, None },
                { 18, _5_4_1 },
                { 1, _5_3_2 },
                { 14, _4_5_1_Defensive },
                { 14, _4_5_1_Normal },
                { 14, _4_5_1_Classic },
                { 14, _4_5_1_Attacking },
                { 14, _4_1_4_1 },
                { 12, _4_4_1_1_Defensive },
                { 13, _4_4_2_Defensive },
                { 13, _4_4_2_Normal },
                { 13, _4_4_2_Attacking },
                { 13, _4_4_2_Diamond_Low },
                { 6, _4_4_2_Diamond },
                { 9, _4_4_2_Diamond_Attacking },
                { 11, _4_3_3_Defensive },
                { 10, _4_3_3_Normal },
                { 0, _4_3_3_Attacking },
                { 0, _4_3_3_Wings },
                { 8, _4_2_4 },
                { 2, _3_5_2_Sweeper },
                { 5, _3_5_2 },
                { 4, _3_4_3_Defensive },
                { 3, _3_4_3_Diamond },
                { 2, _3_4_3_Wings },
                { 8, _4_2_2_1_1_V2 },
                { 12, _4_2_2_1_1 },
                { 7, _4_2_2_1_1_V3 },
                { 12, _4_5_1_Attacking_V2 },
                { 0, _4_3_3_Defensive_V2 },
                { 0, _4_3_3_Normal_V2 },
                { 0, _4_3_3_Attacking_V2 },
                { 3, _3_4_3_Wing_Defenders },
                { 5, _3_5_2_Defenders },
                { 15, _5_2_1_2 },
                { 16, _5_2_2_1 },
                { 17, _5_3_2_Attacking }
            };
            return TranslateTo<UChar>(table, 11);
        }
        else if (gameId == 9) {
            if (ToInt() <= 24)
                return ToInt();
            else {
                static Vector<Pair<UChar, UChar>> table = {
                    { _4_4_2_Defensive,   _4_2_2_1_1_V2 },
                    { _4_4_1_1_Defensive, _4_2_2_1_1 },
                    { _4_4_1_1_Defensive, _4_2_2_1_1_V3 },
                    { _4_5_1_Attacking,   _4_5_1_Attacking_V2 },
                    { _4_3_3_Defensive,   _4_3_3_Defensive_V2 },
                    { _4_3_3_Normal,      _4_3_3_Normal_V2 },
                    { _4_3_3_Attacking,   _4_3_3_Attacking_V2 },
                    { _3_4_3_Wings,       _3_4_3_Wing_Defenders },
                    { _3_5_2,             _3_5_2_Defenders },
                    { _5_3_2,             _5_2_1_2 },
                    { _5_4_1,             _5_2_2_1 },
                    { _5_3_2,             _5_3_2_Attacking }
                };
                return TranslateTo<UChar>(table, _3_4_3_Wings);
            }
        }
        else if (gameId == 10) {
            if (ToInt() <= 24)
                return ToInt();
            else {
                static Vector<Pair<UChar, UChar>> table = {
                    { _4_4_2_Defensive,   _4_2_2_1_1_V2 },
                    { _4_4_1_1_Defensive, _4_2_2_1_1 },
                    { _4_4_1_1_Defensive, _4_2_2_1_1_V3 },
                    { _4_5_1_Attacking,   _4_5_1_Attacking_V2 },
                    { _4_3_3_Defensive,   _4_3_3_Defensive_V2 },
                    { _4_3_3_Normal,      _4_3_3_Normal_V2 },
                    { _4_3_3_Attacking,   _4_3_3_Attacking_V2 },
                    { 25,                 _3_4_3_Wing_Defenders },
                    { 26,                 _3_5_2_Defenders },
                    { _5_3_2,             _5_2_1_2 },
                    { _5_4_1,             _5_2_2_1 },
                    { _5_3_2,             _5_3_2_Attacking }
                };
                return TranslateTo<UChar>(table, _3_4_3_Wings);
            }
        }
        else if (gameId == 11) {
            if (ToInt() <= 31)
                return ToInt();
            else {
                static Vector<Pair<UChar, UChar>> table = {
                    { _3_4_3_Wings, _3_4_3_Wing_Defenders },
                    { _3_5_2,       _3_5_2_Defenders },
                    { _5_3_2,       _5_2_1_2 },
                    { _5_4_1,       _5_2_2_1 },
                    { _5_3_2,       _5_3_2_Attacking }
                };
               return TranslateTo<UChar>(table, _3_4_3_Wings);
            }
        }
        else { // gameId >= 12
            static Vector<Pair<UChar, UChar>> table = {
                { 0, None },
                { 0, _5_4_1 },
                { 0, _5_3_2 },
                { 4, _4_5_1_Defensive },
                { 8, _4_5_1_Normal },
                { 8, _4_5_1_Classic },
                { 8, _4_5_1_Attacking },
                { 8, _4_1_4_1 },
                { 6, _4_4_1_1_Defensive },
                { 7, _4_4_2_Defensive },
                { 5, _4_4_2_Normal },
                { 7, _4_4_2_Attacking },
                { 1, _4_4_2_Diamond_Low },
                { 1, _4_4_2_Diamond },
                { 1, _4_4_2_Diamond_Attacking },
                { 9, _4_3_3_Defensive },
                { 9, _4_3_3_Normal },
                { 3, _4_3_3_Attacking },
                { 3, _4_3_3_Wings },
                { 10, _4_2_4 },
                { 0, _3_5_2_Sweeper },
                { 0, _3_5_2 },
                { 0, _3_4_3_Defensive },
                { 0, _3_4_3_Diamond },
                { 0, _3_4_3_Wings },
                { 7, _4_2_2_1_1_V2 },
                { 18, _4_2_2_1_1 },
                { 18, _4_2_2_1_1_V3 },
                { 18, _4_5_1_Attacking_V2 },
                { 3, _4_3_3_Defensive_V2 },
                { 9, _4_3_3_Normal_V2 },
                { 2, _4_3_3_Attacking_V2 },
                { 0, _3_4_3_Wing_Defenders },
                { 0, _3_5_2_Defenders },
                { 0, _5_2_1_2 },
                { 0, _5_2_2_1 },
                { 0, _5_3_2_Attacking }
            };
            return TranslateTo<UChar>(table, 0);
        }
    }

    ENUM_WRITE(writer) {
        writer.WriteOne(GetIdForGame(writer.GetGameId()));
    }
ENUM_END(FifamFormation)

/* 
FM 07, 08
_4_1_2_2_1 = 0,
_1_4_3_2 = 1,
_3_4_1_2 = 2,
_3_4_2_1 = 3,
_3_4_3 = 4,
_3_5_2 = 5,
_4_1_2_1_2 = 6,
_4_2_3_1 = 7,
_4_2_4 = 8,
_4_3_1_2 = 9,
_4_3_2_1 = 10,
_4_3_3 = 11,
_4_4_1_1 = 12,
_4_4_2 = 13,
_4_5_1 = 14,
_5_2_1_2 = 15,
_5_2_2_1 = 16,
_5_3_2 = 17,
_5_4_1 = 18
*/

/*
FM09

*/
