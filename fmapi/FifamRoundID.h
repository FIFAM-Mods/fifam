#pragma once
#include "FifamEnum.h"

ENUM_BEGIN(FifamRoundID, UChar)
    ENUM_MEMBER(0,  None,         L"")
    ENUM_MEMBER(1,  Quali,        L"ROUND_QUALI")
    ENUM_MEMBER(2,  Quali2,       L"ROUND_QUALI2")
    ENUM_MEMBER(3,  Quali3,       L"ROUND_QUALI3")
    ENUM_MEMBER(4,  PreRound1,    L"ROUND_PREROUND1")
    ENUM_MEMBER(5,  _1,           L"ROUND_1")
    ENUM_MEMBER(6,  _2,           L"ROUND_2")
    ENUM_MEMBER(7,  _3,           L"ROUND_3")
    ENUM_MEMBER(8,  _4,           L"ROUND_4")
    ENUM_MEMBER(9,  _5,           L"ROUND_5")
    ENUM_MEMBER(10, Group1,       L"ROUND_GROUP1")
    ENUM_MEMBER(11, Group2,       L"ROUND_GROUP2")
    ENUM_MEMBER(12, Last16,       L"ROUND_LAST_16")
    ENUM_MEMBER(13, Quarterfinal, L"ROUND_QUARTERFINAL")
    ENUM_MEMBER(14, Semifinal,    L"ROUND_SEMIFINAL")
    ENUM_MEMBER(15, Final,        L"ROUND_FINAL")
    ENUM_MEMBER(16, Final3rdPlace, L"ROUND_FINAL_3RD_PLACE")
    ENUM_DEFAULT_VALUE(None)
    ENUM_DEFAULT_READ_WRITE
ENUM_END(FifamRoundID)
