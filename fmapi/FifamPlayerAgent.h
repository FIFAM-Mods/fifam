#pragma once
#include "FifamTypes.h"
#include "FifamEnum.h"

ENUM_BEGIN(FifamPlayerAgent, UChar)
    ENUM_MEMBER(0,  None,              L"None")
    ENUM_MEMBER(1,  Agency,            L"Agency")
    ENUM_MEMBER(2,  Agent,             L"Agent")
    ENUM_MEMBER(3,  Lawyer,            L"Lawyer")
    ENUM_MEMBER(4,  Wife,              L"Wife")
    ENUM_MEMBER(5,  Girlfriend,        L"Girlfriend")
    ENUM_MEMBER(6,  Father,            L"Father")
    ENUM_MEMBER(7,  Mother,            L"Mother")
    ENUM_MEMBER(8,  Brother,           L"Brother")
    ENUM_MEMBER(9,  Sister,            L"Sister")
    ENUM_MEMBER(10, Uncle,             L"Uncle")
    ENUM_MEMBER(11, Aunt,              L"Aunt")
    ENUM_MEMBER(12, OtherFamilyMember, L"Other Family Member")
    ENUM_MEMBER(13, Friend,            L"Friend")
    ENUM_DEFAULT_VALUE(None)
    ENUM_DEFAULT_READ_WRITE
ENUM_END(FifamPlayerAgent)
