#pragma once
#include "FifamPlayerHistoryEntry.h"
#include "FifamReadWrite.h"

class FifamPlayerHistory {
public:
    // @since FM07
    Set<FifamPlayerHistoryEntry> mEntries;

    void Read(FifamReader &reader, FifamDatabase *database);
    void Write(FifamWriter &writer, FifamDatabase *database);
};
