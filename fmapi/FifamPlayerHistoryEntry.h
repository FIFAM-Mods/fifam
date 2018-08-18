#pragma once
#include "FifamTypes.h"
#include "FifamDate.h"
#include "FifamClubLink.h"
#include "FifamReadWrite.h"

class FifamDatabase;

class FifamPlayerHistoryEntry {
public:
    // @since FM07
    FifamDate mStartDate;
    // @since FM07
    FifamDate mEndDate;
    // @since FM07
    FifamClubLink mClub;
    // @since FM07
    UShort mMatches = 0;
    // @since FM08
    UShort mReserveMatches = 0;
    // @since FM07
    UShort mGoals = 0;
    // @since FM07
    UShort mReserveGoals = 0;
    // @since FM08
    UInt mTransferFee = 0;
    // @since FM07
    Bool mLoan = false;
    // @since FM08
    Bool mStillInThisClub = false;

    struct {
        // @since FM07
        // bWasACaptain flag?
        Bool _1 = false;
    } Unknown;

    void Read(FifamReader &reader, FifamDatabase *database);
    void Write(FifamWriter &writer, FifamDatabase *database);
};
