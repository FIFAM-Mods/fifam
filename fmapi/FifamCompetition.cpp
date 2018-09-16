#include "FifamCompetition.h"
#include "FifamUtils.h"
#include "FifamDatabase.h"

FifamCompetition::~FifamCompetition() {}

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

void FifamCompetition::Read(FifamReader &reader, FifamDatabase *database, FifamNation nationId) {
    reader.ReadLine(mNumSubsAllowed);
    reader.ReadLine(mCompetitionLevel);
    auto predecessors = FifamUtils::ExtractCompetitionIDs(reader.ReadFullLine(), FifamCompRegion::MakeFromInt(nationId.ToInt()));
    mPredecessors.resize(predecessors.size());
    for (UInt i = 0; i < mPredecessors.size(); i++)
        FifamUtils::SaveCompetitionIDToPtr(mPredecessors[i], predecessors[i].ToInt());
    auto successors = FifamUtils::ExtractCompetitionIDs(reader.ReadFullLine(), FifamCompRegion::MakeFromInt(nationId.ToInt()));
    mSuccessors.resize(successors.size());
    for (UInt i = 0; i < mSuccessors.size(); i++)
        FifamUtils::SaveCompetitionIDToPtr(mSuccessors[i], successors[i].ToInt());
    mInstructions.Read(reader, database, nationId);
}

void FifamCompetition::Write(FifamWriter &writer, FifamDatabase *database, FifamNation nationId) {
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
    mInstructions.Write(writer, database, nationId);
}

String FifamCompetition::GetName() const {
    return FifamTr(mName);
}

String FifamCompetition::GetCompIDStr(bool countryName) const {
    return mID.ToStr(countryName);
}

void FifamCompetition::SetName(String const &name) {
    FifamTrSetAll(mName, name);
}

void FifamCompetition::SetName(FifamTrArray<String> const &names) {
    mName = names;
}
