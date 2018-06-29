#include "FifamVersion.h"
#include "Utils.h"

FifamVersion::FifamVersion() {
    mIntValue = 0;
}

void FifamVersion::Set(unsigned short year, unsigned short number) {
    mYear = year;
    mNumber = number;
}

void FifamVersion::Set(unsigned int intValue) {
    mIntValue = intValue;
}

unsigned short FifamVersion::GetYear() {
    return mYear;
}

unsigned short FifamVersion::GetNumber() {
    return mNumber;
}

unsigned int FifamVersion::GetIntValue() {
    return mIntValue;
}

bool FifamVersion::IsGreaterOrEqual(unsigned short year, unsigned short number) {
    return mYear >= year && mNumber >= number;
}

String FifamVersion::GetString() {
    return Utils::Format(L"%04X.%02X", mYear, mNumber);
}
