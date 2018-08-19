#pragma once
#include "FifamTypes.h"
#include "FifamEnum.h"

ENUM_BEGIN(FifamPlayerContractSquadRole, UChar)
    ENUM_MEMBER(0, SquadPlayer,        L"Squad Player")
    ENUM_MEMBER(1, BuildTeamAroundYou, L"Build Team Around You")
    ENUM_MEMBER(2, FirstTeamRegular,   L"First Team Regular")
    ENUM_MEMBER(3, OneForTheFuture,    L"One for the Future")
    ENUM_MEMBER(4, ReservePlayer,      L"Reserve Player")
    ENUM_DEFAULT_VALUE(SquadPlayer)
    ENUM_DEFAULT_READ_WRITE
ENUM_END(FifamPlayerContractSquadRole)
