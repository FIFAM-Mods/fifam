#include "FifamVersion.h"
#include "Utils.h"

FifamVersion::FifamVersion() {
    mIntValue = 0;
}

void FifamVersion::Set(UShort year, UShort number) {
    mYear = year;
    mNumber = number;
}

void FifamVersion::Set(UInt intValue) {
    mIntValue = intValue;
}

UShort FifamVersion::GetYear() {
    return mYear;
}

UShort FifamVersion::GetNumber() {
    return mNumber;
}

UInt FifamVersion::GetIntValue() {
    return mIntValue;
}

Bool FifamVersion::IsGreaterOrEqual(UShort year, UShort number) {
    if (mYear > year)
        return true;
    return mYear == year && mNumber >= number;
}

String FifamVersion::GetString() {
    return Utils::Format(L"%04X.%02X", mYear, mNumber);
}
