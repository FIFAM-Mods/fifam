#pragma once
#include "FifamEnum.h"
#include "FifamTypes.h"

ENUM_BEGIN(FifamPersonType, UChar)
    ENUM_MEMBER(0, Player,    L"Player")
    ENUM_MEMBER(1, Manager,   L"Manager")
    ENUM_MEMBER(2, Chairman,  L"Chairman")
    ENUM_MEMBER(3, President, L"President")
    ENUM_MEMBER(4, Staff,     L"Staff")
    ENUM_DEFAULT_VALUE(Player)
    ENUM_DEFAULT_READ_WRITE
ENUM_END(FifamPersonType)
