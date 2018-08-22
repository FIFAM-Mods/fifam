#pragma once
#include "FifamTypes.h"
#include "FifamEnum.h"
#include "FifamPlayerPositionRole.h"

ENUM_BEGIN(FifamPlayerPosition, UChar)
    ENUM_MEMBER( 0,  None, L"None")
    ENUM_MEMBER( 1,  GK,   L"GK"  )
    ENUM_MEMBER( 2,  RB,   L"RB"  )
    ENUM_MEMBER( 3,  LB,   L"LB"  )
    ENUM_MEMBER( 4,  CB,   L"CB"  )
    ENUM_MEMBER( 5,  SW,   L"SW"  )
    ENUM_MEMBER( 6,  RWB,  L"RWB" )
    ENUM_MEMBER( 7,  LWB,  L"LWB" )
    ENUM_MEMBER( 8,  ANC,  L"ANC" )
    ENUM_MEMBER( 9,  DM,   L"DM"  )
    ENUM_MEMBER( 10, RM,   L"RM"  )
    ENUM_MEMBER( 11, LM,   L"LM"  )
    ENUM_MEMBER( 12, CM,   L"CM"  )
    ENUM_MEMBER( 13, RW,   L"RW"  )
    ENUM_MEMBER( 14, LW,   L"LW"  )
    ENUM_MEMBER( 15, AM,   L"AM"  )
    ENUM_MEMBER( 16, CF,   L"CF"  )
    ENUM_MEMBER( 17, ST,   L"ST"  )
    ENUM_DEFAULT_VALUE(None)

    ENUM_READ(reader, str) {
        if (!str.empty()) {
            UChar id = Utils::SafeConvertInt<UChar>(str);
            if (reader.IsVersionGreaterOrEqual(0x2012, 0x05)) {
                static Vector<Pair<UChar, UChar>> table = {
                    { 0,  None },
                    { 1,  GK },
                    { 2,  RB },
                    { 3,  LB },
                    { 4,  CB },
                    { 5,  DM },
                    { 6,  RM },
                    { 7,  LM },
                    { 8,  CM },
                    { 9,  RW },
                    { 10, LW },
                    { 11, AM },
                    { 12, CF },
                    { 13, ST }
                };
                TranslateFrom(id, table);
            }
            else
                SetFromInt(id);
        }
        else
            SetDefaultValue();
    }

    ENUM_WRITE(writer) {
        if (writer.IsVersionGreaterOrEqual(0x2012, 0x05)) {
            static Vector<Pair<UChar, UChar>> table = {
                { 0,  None },
                { 1,  GK },
                { 2,  RB },
                { 3,  LB },
                { 4,  CB },
                { 4,  SW },
                { 2,  RWB },
                { 3,  LWB },
                { 4,  ANC },
                { 5,  DM },
                { 6,  RM },
                { 7,  LM },
                { 8,  CM },
                { 9,  RW },
                { 10, LW },
                { 11, AM },
                { 12, CF },
                { 13, ST }
            };
            writer.WriteOne(TranslateTo<UChar>(table, 0));
        }
        else
            writer.WriteOne(ToInt());
    }

    FifamPlayerPositionRole GetRole() {
        static Vector<Pair<UChar, UChar>> table = {
            { FifamPlayerPositionRole::None,       None },
            { FifamPlayerPositionRole::Goalkeeper, GK },
            { FifamPlayerPositionRole::Defender,   RB },
            { FifamPlayerPositionRole::Defender,   LB },
            { FifamPlayerPositionRole::Defender,   CB },
            { FifamPlayerPositionRole::Defender,   SW },
            { FifamPlayerPositionRole::Defender,   RWB },
            { FifamPlayerPositionRole::Defender,   LWB },
            { FifamPlayerPositionRole::Midfielder, ANC },
            { FifamPlayerPositionRole::Midfielder, DM },
            { FifamPlayerPositionRole::Midfielder, RM },
            { FifamPlayerPositionRole::Midfielder, LM },
            { FifamPlayerPositionRole::Midfielder, CM },
            { FifamPlayerPositionRole::Attacker,   RW },
            { FifamPlayerPositionRole::Attacker,   LW },
            { FifamPlayerPositionRole::Midfielder, AM },
            { FifamPlayerPositionRole::Attacker,   CF },
            { FifamPlayerPositionRole::Attacker,   ST }
        };
        FifamPlayerPositionRole result;
        result.TranslateFrom<UChar>(ToInt(), table);
        return result;
    }
ENUM_END(FifamPlayerPosition)
