#include "FifamReferee.h"

FifamReferee::FifamReferee() {}

FifamReferee::FifamReferee(FifamReferee const &rhs) {
    mFirstName = rhs.mFirstName;
    mLastName = rhs.mLastName;
    mType = rhs.mType;
}

FifamReferee::FifamReferee(String const &firstName, String const &lastName, Type type) {
    mFirstName = firstName;
    mLastName = lastName;
    mType = type;
}

void FifamReferee::Read(FifamReader &reader) {
    reader.ReadStartIndex(L"REFEREE");
    reader.ReadLine(mFirstName);
    reader.ReadLine(mLastName);
    Char type = reader.ReadLine<Char>();
    if (reader.GetGameId() > 8)
        mType = ArrayElement(m09TypeIds, type, Type::None);
    else
        mType = ArrayElement(m07TypeIds, type, Type::None);
    reader.ReadEndIndex(L"REFEREE");
}

void FifamReferee::Write(FifamWriter &writer) {
    writer.WriteStartIndex(L"REFEREE");
    writer.WriteLine(mFirstName);
    writer.WriteLine(mLastName);
    if (writer.GetGameId() > 8)
        writer.WriteLine(ArrayIndex(m09TypeIds, mType));
    else
        writer.WriteLine(ArrayIndex(m07TypeIds, mType));
    writer.WriteEndIndex(L"REFEREE");
}
