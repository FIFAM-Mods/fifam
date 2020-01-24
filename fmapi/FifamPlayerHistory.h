#pragma once
#include "FifamPlayerHistoryEntry.h"
#include "FifamReadWrite.h"

class FifamPlayerHistory {
public:
    // @since FM07
    Vector<FifamPlayerHistoryEntry> mEntries;

    void Read(FifamReader &reader);
    void Write(FifamWriter &writer);
};
