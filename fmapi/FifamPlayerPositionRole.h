#pragma once
#include "FifamTypes.h"
#include "FifamEnum.h"

ENUM_BEGIN(FifamPlayerPositionRole, UChar)
    ENUM_MEMBER( 0, None,       L"None")
    ENUM_MEMBER( 1, Goalkeeper, L"Goalkeeper")
    ENUM_MEMBER( 2, Defender,   L"Defender")
    ENUM_MEMBER( 3, Midfielder, L"Midfielder")
    ENUM_MEMBER( 4, Attacker,   L"Attacker")
    ENUM_DEFAULT_VALUE(None)
    
    ENUM_READ(reader, str) {
        if (!str.empty()) {
            UChar id = Utils::SafeConvertInt<UChar>(str);
            if (reader.GetGameId() <= 7)
                id += 1;
            SetFromInt(id);
        }
        else
            SetDefaultValue();
    }

    ENUM_WRITE(writer) {
        UChar id = ToInt();
        if (writer.GetGameId() <= 7) {
            if (id == 0)
                id = 2;
            else
                id -= 1;
        }
        writer.WriteOne(id);
    }
ENUM_END(FifamPlayerPositionRole)
