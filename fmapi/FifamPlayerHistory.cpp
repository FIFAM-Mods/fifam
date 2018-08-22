#include "FifamPlayerHistory.h"

void FifamPlayerHistory::Read(FifamReader &reader, FifamDatabase *database) {
    if (reader.ReadStartIndex(L"HIST")) {
        UInt numRecords = reader.ReadLine<UInt>();
        for (UInt i = 0; i < numRecords; i++) {
            FifamPlayerHistoryEntry entry;
            entry.Read(reader, database);
            mEntries.insert(entry);
        }
        reader.ReadEndIndex(L"HIST");
    }
}

void FifamPlayerHistory::Write(FifamWriter &writer, FifamDatabase *database) {
    writer.WriteStartIndex(L"HIST");
    writer.WriteLine(mEntries.size());
    for (auto entry : mEntries)
        entry.Write(writer, database);
    writer.WriteEndIndex(L"HIST");
}
