#pragma once
#include "FifamTypes.h"
#include "FifamEnum.h"

ENUM_BEGIN(FifamCompDbType, UChar)
    ENUM_MEMBER(0,  Root,           L"Root")
    ENUM_MEMBER(1,  Round,          L"Round")
    ENUM_MEMBER(2,  Pool,           L"Pool")
    ENUM_MEMBER(3,  League,         L"League")
    ENUM_MEMBER(4,  Cup,            L"Cup")
    ENUM_MEMBER(5,  Rel1,           L"Rel1")
    ENUM_MEMBER(6,  Rel2,           L"Rel2")
    ENUM_MEMBER(7,  Rel3,           L"Rel3")
    ENUM_MEMBER(8,  RelSwitherland, L"RelSwitherland")
    ENUM_MEMBER(9,  RelScotland1,   L"RelScotland1")
    ENUM_MEMBER(10, RelScotland2,   L"RelScotland2")
    ENUM_DEFAULT_READ_WRITE
ENUM_END(FifamCompDbType)
