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
        return !(year % 4); // 2013, 2017
    case FifamCompType::QualiEC:
        return (year % 4) == 2; // 2015, 2019
    case FifamCompType::WorldCup:
        return (year % 4) == 1; // 2014, 2018
    case FifamCompType::EuroCup:
        return (year % 4) == 3; // 2016
    case FifamCompType::U20WorldCup:
        return (year % 2) == 1; // 2014, 2016, 2018
    case FifamCompType::ConfedCup:
        return !(year % 4); // 2013, 2017
    case FifamCompType::CopaAmerica:
        return (year % 4) == 2; // 2015, 2019
    }
    return true;
}
