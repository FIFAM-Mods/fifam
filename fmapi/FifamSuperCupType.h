#pragma once
#include "FifamTypes.h"
#include "FifamEnum.h"

ENUM_BEGIN(FifamSuperCupType, UInt)
    ENUM_MEMBER( 0, None,    L"None")
    ENUM_MEMBER(15, OneLeg,  L"One Leg")
    ENUM_MEMBER(16, TwoLegs, L"Two Legs")
    ENUM_DEFAULT_VALUE(None)
    ENUM_DEFAULT_READ_WRITE
ENUM_END(FifamSuperCupType)
