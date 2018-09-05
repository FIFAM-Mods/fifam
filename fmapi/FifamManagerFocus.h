#pragma once
#include "FifamTypes.h"
#include "FifamEnum.h"

ENUM_BEGIN(FifamManagerFocus, Char)
    ENUM_MEMBER( 0, None,               L"None")
    ENUM_MEMBER( 1, Offensive,          L"Offensive")
    ENUM_MEMBER( 2, Defensive,          L"Defensive")
    ENUM_MEMBER( 3, Discipline,         L"Discipline")
    ENUM_MEMBER( 4, PlayerCharacter,    L"PlayerCharacter")
    ENUM_MEMBER( 5, Fitness,            L"Fitness")
    ENUM_MEMBER( 6, TacticalEducation,  L"TacticalEducation")
    ENUM_MEMBER( 7, Language,           L"Language")
    ENUM_MEMBER( 8, LongContracts,      L"LongContracts")
    ENUM_MEMBER( 9, YoungPlayers,       L"YoungPlayers")
    ENUM_MEMBER(10, ExperiencedPlayers, L"ExperiencedPlayers")
    ENUM_MEMBER(11, KeepsTeamTogether,  L"KeepsTeamTogether")
    ENUM_DEFAULT_VALUE(None)

    ENUM_READ(reader, str) {
        if (!str.empty())
            SetFromInt(Utils::SafeConvertInt<UChar>(str));
        else
            SetDefaultValue();
    }

    ENUM_WRITE(writer) {
        if (writer.GetGameId() <= 7 && ToInt() == KeepsTeamTogether)
            writer.WriteOne(None);
        else
            writer.WriteOne(ToInt());
    }
ENUM_END(FifamManagerFocus)
