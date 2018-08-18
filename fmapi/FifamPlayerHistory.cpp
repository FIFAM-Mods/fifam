#include "FifamPlayerHistory.h"

void FifamPlayerHistory::Read(FifamReader &reader, FifamDatabase *database) {
    if (reader.ReadStartIndex(L"HIST")) {
        UInt numRecords = reader.ReadLine<UInt>();
        if (numRecords > 0) {
            mEntries.resize(numRecords);
            for (UInt i = 0; i < numRecords; i++)
                mEntries[i].Read(reader, database);
        }
        reader.ReadEndIndex(L"HIST");
    }
}

void FifamPlayerHistory::Write(FifamWriter &writer, FifamDatabase *database) {
    writer.WriteStartIndex(L"HIST");
    writer.WriteLine(mEntries.size());
    if (mEntries.size() > 0) {
        for (UInt i = 0; i < mEntries.size(); i++)
            mEntries[i].Write(writer, database);
        writer.WriteEndIndex(L"HIST");
    }
}
