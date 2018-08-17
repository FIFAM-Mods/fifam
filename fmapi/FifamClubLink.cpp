#include "FifamClubLink.h"
#include "FifamClub.h"

FifamClubLink::FifamClubLink() {}

FifamClubLink::FifamClubLink(FifamClub *ptr, FifamClubTeamType const &teamType) {
    mPtr = ptr;
    mTeamType = teamType;
}

bool FifamClubLink::IsValid() const {
    return mPtr != nullptr;
}

bool FifamClubLink::IsFirstTeam() const {
    return IsValid() && mTeamType == FifamClubTeamType::First;
}

bool FifamClubLink::IsReserveTeam() const {
    return IsValid() && mTeamType == FifamClubTeamType::Reserve;
}

bool FifamClubLink::IsYouthATeam() const {
    return IsValid() && mTeamType == FifamClubTeamType::YouthA;
}

bool FifamClubLink::IsYouthBTeam() const {
    return IsValid() && mTeamType == FifamClubTeamType::YouthB;
}

bool FifamClubLink::IsYouthTeam() const {
    return IsYouthATeam() || IsYouthBTeam();
}

String FifamClubLink::GetTeamName() const {
    String result;
    if (!IsValid())
        result = L"None";
    else {
        result = FifamTr(mPtr->mName);
        if (!IsFirstTeam())
            result += L" (" + mTeamType.ToStr() + L")";
    }
    return result;
}
