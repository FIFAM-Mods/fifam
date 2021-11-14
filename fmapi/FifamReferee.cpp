#include "FifamReferee.h"
#include "FifamNames.h"

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
        if (reader.IsVersionGreaterOrEqual(0x2013, 0x0C))
            reader.ReadLine(mFootballManagerID);
        reader.ReadEndIndex(L"REFEREE");
    }
}

void FifamReferee::Write(FifamWriter &writer) {
    writer.WriteStartIndex(L"REFEREE");
    writer.WriteLine(mFirstName);
    writer.WriteLine(mLastName);
    writer.WriteLine(mType);
    if (writer.IsVersionGreaterOrEqual(0x2013, 0x0C))
        writer.WriteLine(mFootballManagerID);
    writer.WriteEndIndex(L"REFEREE");
}
