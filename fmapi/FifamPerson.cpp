#include "FifamPerson.h"
#include "FifamUtils.h"

FifamPlayer * FifamPerson::AsPlayer() {
    return mPersonType == FifamPersonType::Player ? (FifamPlayer *)this : nullptr;
}

FifamStaff * FifamPerson::AsStaff() {
    return mPersonType != FifamPersonType::Player ? (FifamStaff *)this : nullptr;
}

String FifamPerson::GetName() const {
    if (!mPseudonym.empty())
        return mPseudonym;
    if (!mFirstName.empty())
        return mFirstName + L" " + mLastName;
    return mLastName;
}

UInt FifamPerson::GetAge(FifamDate const & currentDate) {
    return FifamUtils::GetAge(mBirthday, currentDate);
}
