#pragma once

#include "FifamTypes.h"
#include "FifamReadWrite.h"

// @since FM07
// templates for cup system
class FifamCupAlloc {
public:

    // @since FM07
    class Round {
    public:
        // @since FM07
        unsigned char mRound = 0;

        // @since FM07
        // @max 10 in FM07
        unsigned char mNumRounds = 0;

        // @since FM07
        unsigned short mTeamsRound = 0;

        // @since FM07
        unsigned short mNewTeamsRound = 0;
        
        // @since FM07
        unsigned short mStartBeg = 0;

        // @since FM07
        unsigned short mEndBeg = 0;

        // @since FM07
        FifamRoundID mRoundID = FifamRoundID::_1;

        // @since FM07
        FifamBeg mFlags = FifamBeg::_1stLeg | FifamBeg::WithExtraTime | FifamBeg::WithPenalty;
    };

    // @since FM07
    FifamCupAllocType mType = FifamCupAllocType::Misc;

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
