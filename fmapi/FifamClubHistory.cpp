#include "FifamClubHistory.h"

void FifamClubHistory::Read(FifamReader &reader) {
    if (reader.ReadStartIndex(L"HISTORY")) {
        mLeagueWinYears = reader.ReadLineArray<UShort>();
        mCupWinYears = reader.ReadLineArray<UShort>();
        mSuperCupsWinYears = reader.ReadLineArray<UShort>();
        mLeagueCupWinYears = reader.ReadLineArray<UShort>();
        mEuroTrophyWinYears = reader.ReadLineArray<UShort>();
        mChampionsCupWinYears = reader.ReadLineArray<UShort>();
        mWorldChampionshipWinYears = reader.ReadLineArray<UShort>();
        mWorldClubChampionshipWinYears = reader.ReadLineArray<UShort>();
        mRecordHomeWin.Read(reader);
        mRecordHomeDefeat.Read(reader);
        mRecordAwayWin.Read(reader);
        mRecordAwayDefeat.Read(reader);
        mRecordAttendance.Read(reader);
        reader.ReadEndIndex(L"HISTORY");
    }
}

void FifamClubHistory::Write(FifamWriter &writer) {
    writer.WriteStartIndex(L"HISTORY");
    writer.WriteLineArray(mLeagueWinYears);
    writer.WriteLineArray(mCupWinYears);
    writer.WriteLineArray(mSuperCupsWinYears);
    writer.WriteLineArray(mLeagueCupWinYears);
    writer.WriteLineArray(mEuroTrophyWinYears);
    writer.WriteLineArray(mChampionsCupWinYears);
    writer.WriteLineArray(mWorldChampionshipWinYears);
    writer.WriteLineArray(mWorldClubChampionshipWinYears);
    mRecordHomeWin.Write(writer);
    mRecordHomeDefeat.Write(writer);
    mRecordAwayWin.Write(writer);
    mRecordAwayDefeat.Write(writer);
    mRecordAttendance.Write(writer);
    writer.WriteEndIndex(L"HISTORY");
}
