#pragma once
#include "FifamTypes.h"
#include "FifamEnum.h"

ENUM_BEGIN(FifamRelRule, UChar)
    ENUM_MEMBER(  0, _0,         L"REL_RULE_0")
    ENUM_MEMBER(  1, _1,         L"REL_RULE_1")
    ENUM_MEMBER(  2, _2,         L"REL_RULE_2")
    ENUM_MEMBER(  3, _3,         L"REL_RULE_3")
    ENUM_MEMBER(  4, _4,         L"REL_RULE_4")
    ENUM_MEMBER(  5, _5,         L"REL_RULE_5")
    ENUM_MEMBER( 32, Indoor,     L"REL_RULE_INDOOR")
    ENUM_MEMBER(128, TakePoints, L"REL_RULE_TAKE_POINTS")
    ENUM_DEFAULT_VALUE(_0)
    ENUM_DEFAULT_READ_WRITE
ENUM_END(FifamRelRule)
