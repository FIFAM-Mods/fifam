#include "FifamCompPool.h"
#include "FifamUtils.h"

FifamCompDbType FifamCompPool::GetDbType() const {
    return FifamCompDbType::Pool;
}

void FifamCompPool::Read(FifamReader &reader, FifamDatabase *database, FifamNation nationId) {
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
    FifamCompetition::Read(reader, database, nationId);
}

void FifamCompPool::Write(FifamWriter &writer, FifamDatabase *database, FifamNation nationId) {
    writer.WriteLine(mNumPools);
    writer.WriteLine(mNumTeams);
    Vector<String> sortingFlags;
    Bool sortingCantBeUsedInThisVersion = false;
    if (writer.IsVersionGreaterOrEqual(0, 0x1)) {
        if (writer.GetGameId() < 13) {
            if (mSorting == FifamPoolSorting::ChampionscupGroup ||
                mSorting == FifamPoolSorting::PreviousPointsDiff ||
                mSorting == FifamPoolSorting::Continent ||
                mSorting == FifamPoolSorting::DrawWorldCup ||
                mSorting == FifamPoolSorting::DrawEuroCup ||
                mSorting == FifamPoolSorting::CopaAmerica)
            {
                sortingCantBeUsedInThisVersion = true;
            }
        }
    }
    else {
        if (writer.GetGameId() < 11) {
            if (mSorting == FifamPoolSorting::DrawWorldCup ||
                mSorting == FifamPoolSorting::DrawEuroCup ||
                mSorting == FifamPoolSorting::CopaAmerica)
            {
                sortingCantBeUsedInThisVersion = true;
            }
            else if (writer.GetGameId() < 8) {
                if (mSorting == FifamPoolSorting::PreviousPointsDiff ||
                    mSorting == FifamPoolSorting::Continent)
                {
                    sortingCantBeUsedInThisVersion = true;
                }
            }
        }
    }
    if (sortingCantBeUsedInThisVersion)
        writer.WriteLine(L";; !!! THE '" + mSorting.ToStr() + L"' SORTING IS NOT AVAILABLE IN THIS VERSION SO IT WAS DISABLED !!!");
    else {
        if (writer.GetGameId() <= 11 && mSorting == FifamPoolSorting::ChampionscupLast16)
            sortingFlags.push_back(L"POOL_SORT_LAST_16");
        else if (mSorting != FifamPoolSorting::None)
            sortingFlags.push_back(mSorting.ToStr());
    }
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
    FifamCompetition::Write(writer, database, nationId);
}
