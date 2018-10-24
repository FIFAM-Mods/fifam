#include "FifamPerson.h"

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
    if (currentDate.year <= mBirthday.year)
        return 0;
    UInt age = currentDate.year - mBirthday.year - 1;
    if (mBirthday.month < 7)
        ++age;
    return age;
}
