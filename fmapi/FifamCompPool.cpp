#include "FifamCompPool.h"
#include "FifamUtils.h"

FifamCompDbType FifamCompPool::GetDbType() const {
    return FifamCompDbType::Pool;
}

void FifamCompPool::Read(FifamReader &reader, FifamDatabase *database) {
    reader.ReadLine(mNumPools);
    reader.ReadLine(mNumTeams);
    auto sortingFlags = reader.ReadLineArray<String>(L',', true);
    for (auto const &flag : sortingFlags) {
        if (flag == L"POOL_RESERVE_TEAMS")
            mReserveTeamsAllowed = true;
        else if (flag == L"POOL_SORT_LAST_16")
            mSorting = FifamPoolSorting::ChampionscupLast16;
        else
            mSorting.SetFromStr(flag);
    }
    UShort numCompConstraints = reader.ReadLine<UShort>();
    mCompConstraints.resize(numCompConstraints);
    for (UShort i = 0; i < numCompConstraints; i++)
        FifamUtils::SaveCompetitionIDToPtr(mCompConstraints[i], FifamUtils::ExtractCompetitionID(reader.ReadFullLine()).ToInt());
    for (UInt i = 0; i < mBonuses.size(); i++)
        reader.ReadLine(mBonuses[i]);
    FifamCompetition::Read(reader, database);
}

void FifamCompPool::Write(FifamWriter &writer, FifamDatabase *database) {
    writer.WriteLine(mNumPools);
    writer.WriteLine(mNumTeams);
    Vector<String> sortingFlags;
    if (writer.GetGameId() <= 11 && mSorting == FifamPoolSorting::ChampionscupLast16)
        sortingFlags.push_back(L"POOL_SORT_LAST_16");
    else
        sortingFlags.push_back(mSorting.ToStr());
    if (mReserveTeamsAllowed)
        sortingFlags.push_back(L"POOL_RESERVE_TEAMS");
    writer.WriteLineArray(sortingFlags);
    auto compConstraints = FifamUtils::MakeWriteableIDsList(mCompConstraints);
    UShort numCompConstraints = Utils::Min(3, compConstraints.size());
    writer.WriteLine(L"; how many comps to check");
    writer.WriteLine(numCompConstraints);
    writer.WriteLine(L"; which comps to check");
    for (UShort i = 0; i < compConstraints.size(); i++)
        writer.WriteLine(FifamCompID(compConstraints[i]).ToStr());
    for (UInt i = 0; i < mBonuses.size(); i++)
        writer.WriteLine(mBonuses[i]);
    FifamCompetition::Write(writer, database);
}
