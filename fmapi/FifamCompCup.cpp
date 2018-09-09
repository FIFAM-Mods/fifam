#include "FifamCompCup.h"

FifamCompDbType FifamCompCup::GetDbType() {
    return FifamCompDbType::Cup;
}

void FifamCompCup::Read(FifamReader &reader) {
    reader.ReadLineTranslationArray(mName);
    reader.SkipLine();
    if (reader.IsVersionGreaterOrEqual(0, 0x1)) {
        reader.ReadLine(mDrawPeriodInWeeks);
        reader.SkipLine();
    }
    UInt numMatchdays = 0;
    UInt numRounds = 0;
    reader.ReadLine(mNumTeams, numRounds, numMatchdays);
    mFirstSeasonMatchdays.resize(numMatchdays);
    mSecondSeasonMatchdays.resize(numMatchdays);
    if (reader.ReadStartIndex(L"MATCHDAYS")) {
        reader.ReadLineArray(mFirstSeasonMatchdays);
        reader.ReadLineArray(mSecondSeasonMatchdays);
        reader.ReadEndIndex(L"MATCHDAYS");
    }
    if (reader.ReadStartIndex(L"ROUNDINFO")) {
        for (UInt i = 0; i < numRounds; i++) {
            Round &round = mRounds.emplace_back();
            UInt dummy;
            String flags;
            reader.ReadLine(dummy, round.mRoundID, round.mTeamsRound, round.mNewTeamsRound, round.mStartBeg, round.mEndBeg, flags,
                round.mBonuses[0], round.mBonuses[1], round.mBonuses[2], round.mBonuses[3]);
            round.mFlags.SetFromStr(flags, L'|');
        }
        reader.ReadEndIndex(L"ROUNDINFO");
    }
    FifamCompetition::Read(reader);
}

void FifamCompCup::Write(FifamWriter &writer) {
    if (!writer.IsVersionGreaterOrEqual(0, 0x01))
        writer.WriteLine(FifamTr(mName));
    else
        writer.WriteLineTranslationArray(mName);
    writer.WriteLine(0);
    if (writer.IsVersionGreaterOrEqual(0, 0x1)) {
        writer.WriteLine(mDrawPeriodInWeeks);
        writer.WriteLine(mRounds.size());
    }
    writer.WriteLine(mNumTeams, mRounds.size(), mFirstSeasonMatchdays.size());
    writer.WriteStartIndex(L"MATCHDAYS");
    writer.WriteLineArray(mFirstSeasonMatchdays);
    writer.WriteLineArray(mSecondSeasonMatchdays);
    writer.WriteEndIndex(L"MATCHDAYS");
    writer.WriteStartIndex(L"ROUNDINFO");
    for (UInt i = 0; i < mRounds.size(); i++) {
        Round const &round = mRounds[i];
        writer.WriteLine(i, round.mRoundID, round.mTeamsRound, round.mNewTeamsRound, round.mStartBeg, round.mEndBeg,
            round.mFlags.ToStr(L"| "), round.mBonuses[0], round.mBonuses[1], round.mBonuses[2], round.mBonuses[3]);
    }
    writer.WriteEndIndex(L"ROUNDINFO");
    FifamCompetition::Write(writer);
}
