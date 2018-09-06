#pragma once

#include "FifamTypes.h"
#include "FifamReadWrite.h"
#include "FifamFlags.h"
#include "FifamRoundID.h"
#include "FifamBeg.h"
#include "FifamCupAllocType.h"

// @since FM07
// templates for cup system
class FifamCupAlloc {
public:
    // @since FM07
    class Round {
    public:
        // @since FM07
        UChar mRound = 0;
        // @since FM07
        // @max 10 in FM07
        UChar mNumRounds = 0;
        // @since FM07
        UShort mTeamsRound = 0;
        // @since FM07
        UShort mNewTeamsRound = 0;
        // @since FM07
        UShort mStartBeg = 0;
        // @since FM07
        UShort mEndBeg = 0;
        // @since FM07
        FifamRoundID mRoundID;
        // @since FM07
        FifamFlags<FifamBeg> mFlags = FifamBeg::_1stLeg | FifamBeg::WithExtraTime | FifamBeg::WithPenalty;
    };

    // @since FM07
    FifamCupAllocType mType;
    // @since FM07
    // @example _ID_FACUP_GERMANY
    String mNameKey;
    // @since FM07
    UChar mTeamsCount = 0; // can be 0 (auto-calculate?)
    // @since FM07
    // @max 10 rounds in FM07
    Vector<Round *> mRounds;

    void Read(FifamReader &reader);
    void Write(FifamWriter &writer);
    void Clear();
    ~FifamCupAlloc();
};
