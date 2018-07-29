#include "FifamClubRecord.h"

void FifamClubRecord::Read(FifamReader &reader) {
    if (reader.ReadStartIndex(L"RECORD")) {
        if (reader.IsVersionGreaterOrEqual(0x2012, 0x01)) {
            reader.ReadFullLine(mOpponentName);
            reader.ReadLine(mYear, mScore1, mScore2, mAttendance);
        }
        else {
            reader.ReadLine(mScore1);
            reader.ReadLine(mScore2);
            reader.ReadLine(mAttendance);
            reader.ReadFullLine(mOpponentName);
            reader.ReadLine(mYear);
        }
        reader.ReadEndIndex(L"RECORD");
    }
}

void FifamClubRecord::Write(FifamWriter &writer) {
    writer.WriteStartIndex(L"RECORD");
    if (writer.IsVersionGreaterOrEqual(0x2012, 0x01)) {
        writer.WriteLine(mOpponentName);
        writer.WriteLine(mYear, mScore1, mScore2, mAttendance);
    }
    else {
        writer.WriteLine(mScore1);
        writer.WriteLine(mScore2);
        writer.WriteLine(mAttendance);
        writer.WriteLine(mOpponentName);
        writer.WriteLine(mYear);
    }
    writer.WriteEndIndex(L"RECORD");
}
