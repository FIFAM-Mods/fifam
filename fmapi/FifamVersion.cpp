#include "FifamVersion.h"
#include "Utils.h"

FifamVersion::FifamVersion() {
    mIntValue = 0;
}

FifamVersion::FifamVersion(UShort year, UShort number) {
    Set(year, number);
}

void FifamVersion::Set(UShort year, UShort number) {
    mYear = year;
    mNumber = number;
}

void FifamVersion::Set(UInt intValue) {
    mIntValue = intValue;
}

UShort FifamVersion::GetYear() const {
    return mYear;
}

UShort FifamVersion::GetNumber() const {
    return mNumber;
}

UInt FifamVersion::GetIntValue() const {
    return mIntValue;
}

Bool FifamVersion::IsGreaterOrEqual(UShort year, UShort number) const {
    if (mYear > year)
        return true;
    return mYear == year && mNumber >= number;
}

String FifamVersion::GetString() const {
    return Utils::Format(L"%04X.%02X", mYear, mNumber);
}
