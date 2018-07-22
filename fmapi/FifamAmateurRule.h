#pragma once
#include "FifamTypes.h"
#include "FifamEnum.h"

ENUM_BEGIN(FifamAmateurRule, UChar)
    ENUM_MEMBER(0, NoLimit,        L"No Limit")
    ENUM_MEMBER(1, OncePerWeek,    L"Once Per Week")
    ENUM_MEMBER(2, Age,            L"Age")
    ENUM_MEMBER(3, TransferPeriod, L"Transfer Period")
    ENUM_DEFAULT_VALUE(NoLimit)
    ENUM_DEFAULT_READ_WRITE
ENUM_END(FifamAmateurRule)
