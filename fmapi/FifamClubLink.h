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
    Bool IsValid() const;
    Bool IsFirstTeam() const;
    Bool IsReserveTeam() const;
    Bool IsYouthATeam() const;
    Bool IsYouthBTeam() const;
    Bool IsYouthTeam() const;
    String GetTeamName() const;
    void Clear();
};

Bool operator==(FifamClubLink const &lhs, FifamClubLink const &rhs);
