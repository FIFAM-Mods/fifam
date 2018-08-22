#pragma once
#include "FifamTypes.h"
#include "FifamEnum.h"

ENUM_BEGIN(FifamChairmanStability, UChar)
    ENUM_MEMBER(0, Insane,          L"Insane")
    ENUM_MEMBER(1, Impatient,       L"Impatient")
    ENUM_MEMBER(2, Average,         L"Average")
    ENUM_MEMBER(3, Patient,         L"Patient")
    ENUM_MEMBER(4, LongTermPlaning, L"Long-Term Planing")
    ENUM_DEFAULT_VALUE(Average)
    ENUM_DEFAULT_READ_WRITE
ENUM_END(FifamChairmanStability)
