#pragma once
#include "FifamDbWriteable.h"
#include "FifamTypes.h"
#include "FifamCompID.h"
#include "FifamScriptInstruction.h"
#include "FifamCompDbType.h"

class FifamCompLeague;
class FifamCompRound;
class FifamCompCup;
class FifamCompPool;

class FifamCompetition : public FifamDbWriteable {
protected:
    FifamCompetition() {}
public:
    // @since FM07
    // competition name
    FifamTrArray<String> mName;
    // @since FM07
    // competition id
    FifamCompID mID;
    // @since FM07
    // max substitutions allowed in competition
    UChar mNumSubsAllowed = 0;
    // @since FM07
    // 0 = highest - international competitions always have 0
    UChar mCompetitionLevel = 0;
    // @since FM07
    // competition predecessors
    Vector<FifamCompetition *> mPredecessors;
    // @since FM07
    // competition successors
    Vector<FifamCompetition *> mSuccessors;
    // @since FM07
    // instructions
    Vector<FifamScriptInstruction> mInstructions;

    virtual FifamCompDbType GetDbType() = 0;
    virtual void Read(FifamReader &reader);
    virtual void Write(FifamWriter &writer);

    FifamCompLeague *AsLeague();
    FifamCompRound *AsRound();
    FifamCompCup *AsCup();
    FifamCompPool *AsPool();

    String GetName();
    String GetCompIDStr(bool countryName = false);
};
