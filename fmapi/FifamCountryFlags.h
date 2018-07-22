#pragma once
#include "FifamTypes.h"
#include "FifamEnum.h"

ENUM_BEGIN(FifamCountryFlags, UChar)
    ENUM_MEMBER(1,   Unknown1,          L"Unknown1")
    ENUM_MEMBER(2,   GeneratePlayers,   L"GeneratePlayers")
    ENUM_MEMBER(4,   NoTerracesAllowed, L"NoTerracesAllowed")
    ENUM_MEMBER(8,   Unknown8,          L"Unknow8")
    ENUM_MEMBER(16,  Unknown16,         L"Unknow16")
    ENUM_MEMBER(32,  Unknown32,         L"Unknow32")
    ENUM_MEMBER(64,  Unknown64,         L"Unknow64")
    ENUM_MEMBER(128, Unknown128,        L"Unknow128")
    ENUM_DEFAULT_READ_WRITE
ENUM_END(FifamCountryFlags)
