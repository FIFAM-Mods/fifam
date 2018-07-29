#include "FifamReferee.h"

FifamReferee::FifamReferee() {}

FifamReferee::FifamReferee(FifamReferee const &rhs) {
    mFirstName = rhs.mFirstName;
    mLastName = rhs.mLastName;
    mType = rhs.mType;
}

FifamReferee::FifamReferee(String const &firstName, String const &lastName, FifamRefereeType type) {
    mFirstName = firstName;
    mLastName = lastName;
    mType = type;
}

void FifamReferee::Read(FifamReader &reader) {
    if (reader.ReadStartIndex(L"REFEREE")) {
        reader.ReadFullLine(mFirstName);
        reader.ReadFullLine(mLastName);
        reader.ReadLine(mType);
        reader.ReadEndIndex(L"REFEREE");
    }
}

void FifamReferee::Write(FifamWriter &writer) {
    writer.WriteStartIndex(L"REFEREE");
    writer.WriteLine(mFirstName);
    writer.WriteLine(mLastName);
    writer.WriteLine(mType);
    writer.WriteEndIndex(L"REFEREE");
}
