#pragma once
#include "FifamTypes.h"
#include "FifamEnum.h"

ENUM_BEGIN(FifamClubBadgeShape, UChar)
    ENUM_MEMBER(0,  Ball,      L"Ball")
    ENUM_MEMBER(1,  Crest,     L"Crest")
    ENUM_MEMBER(2,  Diamond,   L"Diamond")
    ENUM_MEMBER(3,  Flag,      L"Flag")
    ENUM_MEMBER(4,  Hexagon,   L"Hexagon")
    ENUM_MEMBER(5,  Oval,      L"Oval")
    ENUM_MEMBER(6,  Pill,      L"Pill")
    ENUM_MEMBER(7,  Rectangle, L"Rectangle")
    ENUM_MEMBER(8,  Round,     L"Round")
    ENUM_MEMBER(9,  Square,    L"Square")
    ENUM_MEMBER(10, Triangle,  L"Triangle")
    ENUM_DEFAULT_VALUE(Crest)
    ENUM_DEFAULT_READ_WRITE
ENUM_END(FifamClubBadgeShape)
