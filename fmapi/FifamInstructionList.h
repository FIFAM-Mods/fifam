#pragma once
#include "FifamTypes.h"
#include "FifamInstruction.h"
#include "FifamReadWrite.h"
#include "FifamCompDbType.h"

class FifamInstructionsList {
    friend class FifamCompetition;
    Vector<FifamAbstractInstruction *> stack;
public:
    FifamInstructionsList();
    FifamInstructionsList(FifamInstructionsList const &rhs);
    FifamInstructionsList &operator=(FifamInstructionsList const &rhs);
    UInt Size();
    Bool Empty();
    void Clear();
    void PushBack(FifamAbstractInstruction *instruction);
    void Insert(FifamAbstractInstruction *instruction, Int position);
    Bool Contains(FifamInstructionID instructionID);
    UInt Count(FifamInstructionID instructionID);
    FifamAbstractInstruction *FindFirstOccurence(FifamInstructionID instructionID);
    Int FindFirstOccurencePosition(FifamInstructionID instructionID);
    void ForAll(Function<void(FifamAbstractInstruction *)> callback);
    void ForAll(Function<void(FifamAbstractInstruction *, UInt)> callback);
    void ForAllCompetitionLinks(Function<void(FifamCompID &, UInt, FifamAbstractInstruction *)> callback);
    void ForAllCountryLinks(Function<void(FifamNation &, UInt, FifamAbstractInstruction *)> callback);
    void ForAllAssessmentPositions(Function<void(UInt &, UInt, FifamAbstractInstruction *)> callback);
    void ForAllLeagueLevels(Function<void(UInt &, UInt, FifamAbstractInstruction *)> callback);
    void Read(FifamReader &reader, FifamDatabase *database, FifamNation nationId);
    void Write(FifamWriter &writer, FifamDatabase *database, FifamCompDbType compDbType, FifamNation nationId, Bool useEndOfEntry = false);
};
