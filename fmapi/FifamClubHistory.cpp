#include "FifamClubHistory.h"

void FifamClubHistory::Read(FifamReader &reader) {
    if (reader.ReadStartIndex(L"HIST")) {
        mLeagueWinYears = reader.ReadLineArray<UShort>(L',', true);
        mCupWinYears = reader.ReadLineArray<UShort>(L',', true);
        mSuperCupsWinYears = reader.ReadLineArray<UShort>(L',', true);
        mLeagueCupWinYears = reader.ReadLineArray<UShort>(L',', true);
        mEuroTrophyWinYears = reader.ReadLineArray<UShort>(L',', true);
        mChampionsCupWinYears = reader.ReadLineArray<UShort>(L',', true);
        mWorldChampionshipWinYears = reader.ReadLineArray<UShort>(L',', true);
        mWorldClubChampionshipWinYears = reader.ReadLineArray<UShort>(L',', true);
        mRecordHomeWin.Read(reader);
        mRecordHomeDefeat.Read(reader);
        mRecordAwayWin.Read(reader);
        mRecordAwayDefeat.Read(reader);
        mRecordAttendance.Read(reader);
        reader.ReadEndIndex(L"HIST");
    }
}

void FifamClubHistory::Write(FifamWriter &writer) {
    writer.WriteStartIndex(L"HIST");
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
    writer.WriteEndIndex(L"HIST");
}
