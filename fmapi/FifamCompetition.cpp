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
        mPredecessors[i].SetFromInt(predecessors[i].ToInt());
    auto successors = FifamUtils::ExtractCompetitionIDs(reader.ReadFullLine(), FifamCompRegion::MakeFromInt(nationId.ToInt()));
    mSuccessors.resize(successors.size());
    for (UInt i = 0; i < mSuccessors.size(); i++)
        mSuccessors[i].SetFromInt(successors[i].ToInt());
    mInstructions.Read(reader, database, nationId);
}

void FifamCompetition::Write(FifamWriter &writer, FifamDatabase *database, FifamNation nationId) {
    writer.WriteLine(mNumSubsAllowed);
    writer.WriteLine(mCompetitionLevel);
    if (!mPredecessors.empty()) {
        auto predecessors = FifamUtils::MakeWriteableIDsList(mPredecessors, writer.GetGameId());
        Vector<FifamCompID> compIDs;
        for (UInt i = 0; i < predecessors.size(); i++)
            compIDs.push_back(predecessors[i]);
        writer.WriteLine(FifamUtils::JoinCompetitionIDs(compIDs));
    }
    else
        writer.WriteLine(0);
    if (!mSuccessors.empty()) {
        auto successors = FifamUtils::MakeWriteableIDsList(mSuccessors, writer.GetGameId());
        Vector<FifamCompID> compIDs;
        for (UInt i = 0; i < successors.size(); i++)
            compIDs.push_back(successors[i]);
        writer.WriteLine(FifamUtils::JoinCompetitionIDs(compIDs));
    }
    else
        writer.WriteLine(0);
    mInstructions.Write(writer, database, GetDbType(), nationId, mUseEndOfEntry);
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

bool FifamCompetition::TakesPlaceInSeason(UShort year) const {
    switch (mID.mType.ToInt()) {
    case FifamCompType::QualiWC:
    case FifamCompType::ConfedCup:
        return !(year % 4); // 16/17, 20/21
    case FifamCompType::QualiEC:
    case FifamCompType::AsiaCup:
    case FifamCompType::AsiaCupQ:
        return (year % 4) == 2; // 14/15, 18/19
    case FifamCompType::WorldCup:
        return (year % 4) == 1; // 13/14, 17/18
    case FifamCompType::EuroCup:
    case FifamCompType::OfcCup:
    case FifamCompType::CopaAmerica:
        return (year % 4) == 3; // 19/20, 23/24
    case FifamCompType::U20WorldCup:
        return (year % 2) == 1; // 17/18, 19/20
    case FifamCompType::EuroNL:
    case FifamCompType::EuroNLQ:
    case FifamCompType::AfricaCup:
    case FifamCompType::AfricaCupQ:
    case FifamCompType::NamCup:
        return !(year % 2); // 16/17, 18/19
    case FifamCompType::NamNL:
    case FifamCompType::NamNLQ:
        return (year % 2) == 1; // 17/18, 19/20
    }
    return true;
}
