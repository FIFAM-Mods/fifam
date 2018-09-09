#include "FifamCompetition.h"
#include "FifamUtils.h"

FifamCompLeague *FifamCompetition::AsLeague() {
    return GetDbType() == FifamCompDbType::League ? reinterpret_cast<FifamCompLeague *>(this) : nullptr;
}

FifamCompRound *FifamCompetition::AsRound() {
    return GetDbType() == FifamCompDbType::Round ? reinterpret_cast<FifamCompRound *>(this) : nullptr;
}

FifamCompCup *FifamCompetition::AsCup() {
    return GetDbType() == FifamCompDbType::Cup ? reinterpret_cast<FifamCompCup *>(this) : nullptr;
}

FifamCompPool *FifamCompetition::AsPool() {
    return GetDbType() == FifamCompDbType::Pool ? reinterpret_cast<FifamCompPool *>(this) : nullptr;
}

void FifamCompetition::Read(FifamReader &reader) {
    reader.ReadLine(mNumSubsAllowed);
    reader.ReadLine(mCompetitionLevel);
    auto predecessors = FifamUtils::ExtractCompetitionIDs(reader.ReadFullLine());
    mPredecessors.resize(predecessors.size());
    for (UInt i = 0; i < mPredecessors.size(); i++)
        FifamUtils::SaveCompetitionIDToPtr(mPredecessors[i], predecessors[i].ToInt());
    auto successors = FifamUtils::ExtractCompetitionIDs(reader.ReadFullLine());
    mSuccessors.resize(successors.size());
    for (UInt i = 0; i < mSuccessors.size(); i++)
        FifamUtils::SaveCompetitionIDToPtr(mSuccessors[i], successors[i].ToInt());
    UInt numInstructions = reader.ReadLine<UInt>();
    for (UInt i = 0; i < numInstructions; i++) {
        FifamScriptInstruction instruction;
        instruction.Read(reader);
        if (instruction.mID.GetWasSetFromUnknown() || instruction.mID == FifamScriptInstructionID::ID_END_OF_ENTRY)
            break;
        mInstructions.push_back(instruction);
    }
}

void FifamCompetition::Write(FifamWriter &writer) {
    writer.WriteLine(mNumSubsAllowed);
    writer.WriteLine(mCompetitionLevel);
    if (!mPredecessors.empty()) {
        auto predecessors = FifamUtils::MakeWriteableIDsList(mPredecessors);
        Vector<FifamCompID> compIDs;
        for (UInt i = 0; i < predecessors.size(); i++)
            compIDs.push_back(predecessors[i]);
        writer.WriteLine(FifamUtils::JoinCompetitionIDs(compIDs));
    }
    else
        writer.WriteLine(0);
    if (!mSuccessors.empty()) {
        auto successors = FifamUtils::MakeWriteableIDsList(mSuccessors);
        Vector<FifamCompID> compIDs;
        for (UInt i = 0; i < successors.size(); i++)
            compIDs.push_back(successors[i]);
        writer.WriteLine(FifamUtils::JoinCompetitionIDs(compIDs));
    }
    else
        writer.WriteLine(0);
    writer.WriteLine(mInstructions.size());
    for (UInt i = 0; i < mInstructions.size(); i++)
        mInstructions[i].Write(writer);
}

String FifamCompetition::GetName() {
    return FifamTr(mName);
}

String FifamCompetition::GetCompIDStr(bool countryName) {
    return mID.ToStr(countryName);
}
