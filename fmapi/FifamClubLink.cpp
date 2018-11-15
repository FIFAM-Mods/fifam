#include "FifamClubLink.h"
#include "FifamClub.h"

FifamClubLink::FifamClubLink() {}

FifamClubLink::FifamClubLink(FifamClub *ptr, FifamClubTeamType const &teamType) {
    mPtr = ptr;
    mTeamType = teamType;
}

Bool FifamClubLink::IsValid() const {
    return mPtr != nullptr;
}

Bool FifamClubLink::IsFirstTeam() const {
    return IsValid() && mTeamType == FifamClubTeamType::First;
}

Bool FifamClubLink::IsReserveTeam() const {
    return IsValid() && mTeamType == FifamClubTeamType::Reserve;
}

Bool FifamClubLink::IsYouthATeam() const {
    return IsValid() && mTeamType == FifamClubTeamType::YouthA;
}

Bool FifamClubLink::IsYouthBTeam() const {
    return IsValid() && mTeamType == FifamClubTeamType::YouthB;
}

Bool FifamClubLink::IsYouthTeam() const {
    return IsYouthATeam() || IsYouthBTeam();
}

String FifamClubLink::GetTeamName() const {
    String result;
    if (IsValid()) {
        result = FifamTr(mPtr->mName);
        if (!IsFirstTeam())
            result += L" (" + mTeamType.ToStr() + L")";
    }
    return result;
}

void FifamClubLink::Clear() {
    mPtr = nullptr;
}

Bool operator==(FifamClubLink const & lhs, FifamClubLink const & rhs) {
    return lhs.mPtr == rhs.mPtr && lhs.mTeamType == rhs.mTeamType;
}
