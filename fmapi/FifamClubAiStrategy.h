#pragma once
#include "FifamTypes.h"
#include "FifamEnum.h"

ENUM_BEGIN(FifamClubAiStrategy, UChar)
    ENUM_MEMBER(0, General,              L"General")
    ENUM_MEMBER(1, Representation,       L"Representation")
    ENUM_MEMBER(2, TrainingAndYouth,     L"Training and youth")
    ENUM_MEMBER(3, Medical,              L"Medical")
    ENUM_MEMBER(4, Income,               L"Income")
    ENUM_MEMBER(5, ScoutingAndTransfers, L"Scouting and transfers")
    ENUM_MEMBER(6, Stadium,              L"Stadium")
    ENUM_DEFAULT_VALUE(General)
    ENUM_DEFAULT_READ_WRITE
ENUM_END(FifamClubAiStrategy)
