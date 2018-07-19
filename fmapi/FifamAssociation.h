#pragma once
#include "FifamEnum.h"
#include "FifamTypes.h"

// @since FM07
ENUM_BEGIN(FifamAssociation, UChar)
    ENUM_MEMBER(0, UEFA,     L"UEFA")
    ENUM_MEMBER(1, CONMEBOL, L"CONMEBOL")
    ENUM_MEMBER(2, CONCACAF, L"CONCACAF")
    ENUM_MEMBER(3, CAF,      L"CAF")
    ENUM_MEMBER(4, AFC,      L"AFC")
    ENUM_MEMBER(5, OFC,      L"OFC")
    ENUM_MEMBER(6, None,     L"None")
    ENUM_DEFAULT_VALUE(None)
    ENUM_DEFAULT_READ_WRITE
ENUM_END(FifamAssociation)
