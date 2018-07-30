#include "FifamGeoCoords.h"

void FifamGeoCoord::SetFromInt(Int value) {
    if (value < 0) {
        mDirection = 1;
        value = -value;
    }
    else
        mDirection = 0;
    Set(mDirection, (value >> 16) & 0x7FFF, value & 0xFFFF);
}

Int FifamGeoCoord::ToInt() {
    Int value =  mMinutes | (mDegrees << 16);
    if (mDirection)
        value = -value;
    return value;
}

void FifamGeoCoord::Set(UChar direction, UShort degrees, UShort minutes) {
    mDirection = Utils::Clamp(direction, 0, 1);
    mMinutes = Utils::Clamp(minutes, 0, 59);
    mDegrees = Utils::Clamp(degrees, 0, 359);
}

String FifamLatitude::ToStr() {
    return Utils::Format(L"%u°%u'%C", mDegrees, mMinutes, mDirection ? 'S' : 'N');
}

String FifamLongitude::ToStr() {
    return Utils::Format(L"%u°%u'%C", mDegrees, mMinutes, mDirection ? 'W' : 'E');
}

String FifamGeoCoords::ToStr() {
    return mLatitude.ToStr() + L"," + mLongitude.ToStr();
}

FifamGeoCoord::FifamGeoCoord() {}

FifamGeoCoord::FifamGeoCoord(Int value) {
    SetFromInt(value);
}

FifamGeoCoord::FifamGeoCoord(UChar direction, UShort degrees, UShort minutes) {
    Set(direction, degrees, minutes);
}

FifamLatitude::FifamLatitude() {}

FifamLatitude::FifamLatitude(Int value) {
    SetFromInt(value);
}

FifamLatitude::FifamLatitude(UChar direction, UShort degrees, UShort minutes) {
    Set(direction, degrees, minutes);
}

FifamLongitude::FifamLongitude() {}

FifamLongitude::FifamLongitude(Int value) {
    SetFromInt(value);
}

FifamLongitude::FifamLongitude(UChar direction, UShort degrees, UShort minutes) {
    Set(direction, degrees, minutes);
}

FifamGeoCoords::FifamGeoCoords() {}

FifamGeoCoords::FifamGeoCoords(Int latitudeValue, Int longitudeValue) {
    Set(latitudeValue, longitudeValue);
}

FifamGeoCoords::FifamGeoCoords(FifamLatitude const &latitude, FifamLongitude const &longitude) {
    mLatitude = latitude;
    mLongitude = longitude;
}

void FifamGeoCoords::Set(Int latitudeValue, Int longitudeValue) {
    mLatitude.SetFromInt(latitudeValue);
    mLongitude.SetFromInt(longitudeValue);
}
