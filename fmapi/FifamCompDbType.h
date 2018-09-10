#pragma once
#include "FifamTypes.h"
#include "FifamEnum.h"

ENUM_BEGIN(FifamCompDbType, UChar)
    ENUM_MEMBER(0, Root,    L"DB_ROOT")
    ENUM_MEMBER(1, League,  L"DB_LEAGUE")
    ENUM_MEMBER(2, Spare,   L"DB_SPARE")
    ENUM_MEMBER(3, Cup,     L"DB_CUP")
    ENUM_MEMBER(4, Round,   L"DB_ROUND")
    ENUM_MEMBER(5, Pool,    L"DB_POOL")
    ENUM_DEFAULT_READ_WRITE
ENUM_END(FifamCompDbType)
