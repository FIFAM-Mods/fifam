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

void FifamGeoCoord::SetFromFloat(Float value) {
    if (value < 0) {
        mDirection = 1;
        value = -value;
    }
    else
        mDirection = 0;
    Float i = 0;
    Float f = modf(value, &i);
    Set(mDirection, (UShort)i, (UShort)(f * 60.0f + 0.5f));
}

Float FifamGeoCoord::ToFloat() {
    Float value = (Float)mDegrees + (Float)mMinutes / 60.0f;
    if (mDirection)
        value = -value;
    return value;
}

Int FifamGeoCoord::ToInt() {
    Int value =  mMinutes | (mDegrees << 16);
    if (mDirection)
        value = -value;
    return value;
}

void FifamLatitude::Set(UChar direction, UShort degrees, UShort minutes) {
    mDirection = Utils::Clamp(direction, 0, 1);
    mMinutes = Utils::Clamp(minutes, 0, 59);
    mDegrees = Utils::Clamp(degrees, 0, 90);
    if (mDegrees == 90)
        mDegrees = 0;
}

void FifamLongitude::Set(UChar direction, UShort degrees, UShort minutes) {
    mDirection = Utils::Clamp(direction, 0, 1);
    mMinutes = Utils::Clamp(minutes, 0, 59);
    mDegrees = Utils::Clamp(degrees, 0, 180);
    if (mDegrees == 180)
        mDegrees = 0;
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

void FifamGeoCoords::SetFromFloat(Float latitudeFloatValue, Float longitudeFloatValue) {
    mLatitude.SetFromFloat(latitudeFloatValue);
    mLongitude.SetFromFloat(longitudeFloatValue);
}
