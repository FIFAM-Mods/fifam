#pragma once
#include "FifamTypes.h"
#include "FifamEnum.h"

ENUM_BEGIN(FifamStadiumType, UChar)
    ENUM_MEMBER(0, Oval,            L"Oval")
    ENUM_MEMBER(1, RoundedCorners,  L"Rounded Corners")
    ENUM_MEMBER(2, FootballStadium, L"Football Stadium")
    ENUM_DEFAULT_VALUE(Oval)
    ENUM_DEFAULT_READ_WRITE
ENUM_END(FifamStadiumType)
