#include "FifamCompRound.h"

FifamCompDbType FifamCompRound::GetDbType() const {
    return FifamCompDbType::Round;
}

void FifamCompRound::Read(FifamReader &reader, FifamDatabase *database) {
    if (!reader.IsVersionGreaterOrEqual(0x2011, 0x07))
        reader.ReadLineTranslationArray(mName);
    reader.ReadLine(mNumTeams);
    mRoundType.SetFromStr(reader.ReadFullLine());
    if (reader.IsVersionGreaterOrEqual(0x2006, 0x00)) {
        mFirstSeasonMatchdays[0] = reader.ReadLine<UShort>();
        mFirstSeasonMatchdays[1] = reader.ReadLine<UShort>();
        mSecondSeasonMatchdays[0] = reader.ReadLine<UShort>();
        mSecondSeasonMatchdays[1] = reader.ReadLine<UShort>();
        m1stLegFlags.SetFromStr(reader.ReadFullLine());
        m2ndLegFlags.SetFromStr(reader.ReadFullLine());
    }
    else {
        mFirstSeasonMatchdays[0] = reader.ReadLine<UShort>();
        mSecondSeasonMatchdays[0] = reader.ReadLine<UShort>();
        m1stLegFlags.SetFromStr(reader.ReadFullLine());
        mFirstSeasonMatchdays[1] = reader.ReadLine<UShort>();
        mSecondSeasonMatchdays[1] = reader.ReadLine<UShort>();
        m2ndLegFlags.SetFromStr(reader.ReadFullLine());
    }
    for (UInt i = 0; i < mBonuses.size(); i++)
        reader.ReadLine(mBonuses[i]);
    FifamCompetition::Read(reader, database);
}

void FifamCompRound::Write(FifamWriter &writer, FifamDatabase *database) {
    if (!writer.IsVersionGreaterOrEqual(0x2011, 0x07)) {
        if (!writer.IsVersionGreaterOrEqual(0, 0x01))
            writer.WriteLine(FifamTr(mName));
        else
            writer.WriteLineTranslationArray(mName);
    }
    writer.WriteLine(mNumTeams);
    writer.WriteLine(mRoundType.ToStr());
    if (writer.IsVersionGreaterOrEqual(0x2006, 0x00)) {
        writer.WriteLine(mFirstSeasonMatchdays[0]);
        writer.WriteLine(mFirstSeasonMatchdays[1]);
        writer.WriteLine(mSecondSeasonMatchdays[0]);
        writer.WriteLine(mSecondSeasonMatchdays[1]);
        writer.WriteLine(m1stLegFlags.ToStr(L", "));
        writer.WriteLine(m2ndLegFlags.ToStr(L", "));
    }
    else {
        writer.WriteLine(mFirstSeasonMatchdays[0]);
        writer.WriteLine(mSecondSeasonMatchdays[0]);
        writer.WriteLine(m1stLegFlags.ToStr(L", "));
        writer.WriteLine(mFirstSeasonMatchdays[1]);
        writer.WriteLine(mSecondSeasonMatchdays[1]);
        writer.WriteLine(m2ndLegFlags.ToStr(L", "));
    }
    for (UInt i = 0; i < mBonuses.size(); i++)
        writer.WriteLine(mBonuses[i]);
    FifamCompetition::Write(writer, database);
}
