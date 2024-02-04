#pragma once
#include "FifamPropertied.h"
#include "FifamTypes.h"
#include "FifamReadWrite.h"
#include "FifamRefereeType.h"

class FifamCountry;

// @since FM07
class FifamReferee : public FifamPropertied {
public:
    // @since FM07
    // @maxsize 19
    String mFirstName;
    // @since FM07
    // @maxsize 19
    String mLastName;
    // @since FM07
    FifamRefereeType mType;
    // @since FM22
    Int mFootballManagerID = -1;
    // @since FM24
    UInt mCreator = 0;

    FifamCountry *mCountry = nullptr;

public:
    FifamReferee();
    FifamReferee(FifamReferee const &rhs);
    FifamReferee(String const &firstName, String const &lastName, FifamRefereeType type);
    void Read(FifamReader &reader);
    void Write(FifamWriter &writer);
};
