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
    ENUM_DEFAULT_READ_WRITE
ENUM_END(FifamPlayerPositionRole)
