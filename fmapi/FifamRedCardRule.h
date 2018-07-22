#pragma once
#include "FifamTypes.h"
#include "FifamEnum.h"

ENUM_BEGIN(FifamRedCardRule, UChar)
    ENUM_MEMBER(0, Immediately, L"Immediately")
    ENUM_MEMBER(1, NextMonday,  L"Next Monday")
    ENUM_DEFAULT_VALUE(Immediately)
    ENUM_DEFAULT_READ_WRITE
ENUM_END(FifamRedCardRule)
