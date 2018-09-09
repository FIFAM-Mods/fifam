#pragma once
#include "FifamPlayerHistoryEntry.h"
#include "FifamReadWrite.h"

class FifamPlayerHistory {
public:
    // @since FM07
    MultiSet<FifamPlayerHistoryEntry> mEntries;

    void Read(FifamReader &reader);
    void Write(FifamWriter &writer);
};
