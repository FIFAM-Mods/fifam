#pragma once
#include "FifamTypes.h"
#include "FifamEnum.h"

ENUM_BEGIN(FifamClubSettlement, UChar)
    ENUM_MEMBER(0, Village, L"Village")
    ENUM_MEMBER(1, Town,    L"Town")
    ENUM_MEMBER(2, City,    L"City")
    ENUM_DEFAULT_VALUE(Town)
    ENUM_DEFAULT_READ_WRITE
ENUM_END(FifamClubSettlement)
