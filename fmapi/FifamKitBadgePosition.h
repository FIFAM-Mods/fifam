#pragma once
#include "FifamTypes.h"
#include "FifamEnum.h"

ENUM_BEGIN(FifamKitBadgePosition, UChar)
    ENUM_MEMBER(0, Left,   L"Left")
    ENUM_MEMBER(1, Center, L"Center")
    ENUM_MEMBER(2, Right,  L"Right")
    ENUM_DEFAULT_VALUE(Left)
    ENUM_DEFAULT_READ_WRITE
ENUM_END(FifamKitBadgePosition)
