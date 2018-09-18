#pragma once
#include "FifamDbWriteable.h"
#include "FifamTypes.h"
#include "FifamCompID.h"
#include "FifamInstructionList.h"
#include "FifamCompDbType.h"

class FifamCompLeague;
class FifamCompRound;
class FifamCompCup;
class FifamCompPool;
class FifamDatabase;

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
    FifamInstructionsList mInstructions;

    virtual FifamCompDbType GetDbType() const = 0;
    virtual void Read(FifamReader &reader, FifamDatabase *database, FifamNation nationId);
    virtual void Write(FifamWriter &writer, FifamDatabase *database, FifamNation nationId);
    virtual ~FifamCompetition();

    FifamCompLeague *AsLeague();
    FifamCompRound *AsRound();
    FifamCompCup *AsCup();
    FifamCompPool *AsPool();

    String GetName() const;
    String GetCompIDStr(bool countryName = false) const;
    void SetName(String const &name);
    void SetName(FifamTrArray<String> const &names);
};

using FifamCompEntry = Pair<FifamCompID, FifamCompetition *>;
