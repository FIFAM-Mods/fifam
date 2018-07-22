#pragma once
#include "FifamTypes.h"
#include "FifamReadWrite.h"
#include "FifamRefereeType.h"

class FifamCountry;

// @since FM07
class FifamReferee {
public:
    // @since FM07
    // @maxsize 19
    String mFirstName;
    // @since FM07
    // @maxsize 19
    String mLastName;
    // @since FM07
    FifamRefereeType mType;

    FifamCountry *mCountry = nullptr;

public:
    FifamReferee();
    FifamReferee(FifamReferee const &rhs);
    FifamReferee(String const &firstName, String const &lastName, FifamRefereeType type);
    void Read(FifamReader &reader);
    void Write(FifamWriter &writer);
};
