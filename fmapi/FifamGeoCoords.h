#pragma once
#include "FifamTypes.h"
#include "Utils.h"

struct FifamGeoCoord {
    UShort mMinutes = 0;
    UShort mDegrees = 0;
    UChar mDirection = 0;

    void SetFromInt(Int value);
    void SetFromFloat(Float value);
    Int ToInt();
    Float ToFloat();
    virtual void Set(UChar direction, UShort degrees, UShort minutes) = 0;
};

struct FifamLatitude : public FifamGeoCoord {
    enum { North, South };

    String ToStr();
    void Set(UChar direction, UShort degrees, UShort minutes);
    FifamLatitude();
    FifamLatitude(Int value);
    FifamLatitude(UChar direction, UShort degrees, UShort minutes);
};

struct FifamLongitude : public FifamGeoCoord {
    enum { East, West };

    String ToStr();
    void Set(UChar direction, UShort degrees, UShort minutes);
    FifamLongitude();
    FifamLongitude(Int value);
    FifamLongitude(UChar direction, UShort degrees, UShort minutes);
};

struct FifamGeoCoords {
    FifamLatitude mLatitude;
    FifamLongitude mLongitude;

    String ToStr();
    FifamGeoCoords();
    FifamGeoCoords(Int latitudeValue, Int longitudeValue);
    FifamGeoCoords(FifamLatitude const &latitude, FifamLongitude const &longitude);
    void Set(Int latitudeValue, Int longitudeValue);
    void SetFromFloat(Float latitudeFloatValue, Float longitudeFloatValue);
};
