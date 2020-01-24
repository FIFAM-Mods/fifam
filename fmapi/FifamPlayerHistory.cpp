#include "FifamPlayerHistory.h"

void FifamPlayerHistory::Read(FifamReader &reader) {
    if (reader.ReadStartIndex(L"HIST")) {
        UInt numRecords = reader.ReadLine<UInt>();
        for (UInt i = 0; i < numRecords; i++) {
            FifamPlayerHistoryEntry entry;
            entry.Read(reader);
            mEntries.push_back(entry);
        }
        reader.ReadEndIndex(L"HIST");
    }
}

void FifamPlayerHistory::Write(FifamWriter &writer) {
    writer.WriteStartIndex(L"HIST");
    writer.WriteLine(mEntries.size());
    for (auto entry : mEntries)
        entry.Write(writer);
    writer.WriteEndIndex(L"HIST");
}
