#pragma once
#include "FifamTypes.h"
#include "FifamEnum.h"

ENUM_BEGIN(FifamSuspensionRule, UChar)
    ENUM_MEMBER(0, _5_10_15_20_25, L"5/10/15/20/25")
    ENUM_MEMBER(1, _4_8_12_16_20,  L"4/8/12/16/20")
    ENUM_MEMBER(2, _4_7_10_13_16,  L"4/7/10/13/16")
    ENUM_MEMBER(3, _4_7_9_10_11,   L"4/7/9/10/11")
    ENUM_MEMBER(4, _5_9_13_17_21,  L"5/9/13/17/21")
    ENUM_MEMBER(5, _3_6_9_12_15,   L"3/6/9/12/15")
    ENUM_DEFAULT_VALUE(_5_10_15_20_25)
    ENUM_DEFAULT_READ_WRITE
ENUM_END(FifamSuspensionRule)
