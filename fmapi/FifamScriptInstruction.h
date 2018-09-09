#pragma once
#include "FifamScriptInstructionID.h"
#include "FifamReadWrite.h"

class FifamCompetition;

// @since FM07
class FifamScriptInstruction {
public:
    // @since FM07
    // instruction ID
    FifamScriptInstructionID mID;
    // @since FM07
    // competition parameter
    FifamCompetition *mCompParam;
    // @since FM07
    // instruction parameters
    Array<UInt, 4> mParams = {};

    void Read(FifamReader &reader) {

    }

    void Write(FifamWriter &writer) {

    }
};
