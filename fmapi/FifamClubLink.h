#pragma once
#include "FifamTypes.h"
#include "FifamClubTeamType.h"

class FifamClub;

class FifamClubLink {
public:
    FifamClub *mPtr = nullptr;
    FifamClubTeamType mTeamType;

    FifamClubLink();
    FifamClubLink(FifamClub *ptr, FifamClubTeamType const &teamType = FifamClubTeamType::First);
    bool IsValid() const;
    bool IsFirstTeam() const;
    bool IsReserveTeam() const;
    bool IsYouthATeam() const;
    bool IsYouthBTeam() const;
    bool IsYouthTeam() const;
    String GetTeamName() const;
};
