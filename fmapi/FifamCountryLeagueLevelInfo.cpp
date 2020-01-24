#include "FifamCountryLeagueLevelInfo.h"
#include "FifamDatabase.h"

void FifamCountryLeagueLevelInfo::Read(FifamReader &reader) {
    if (FifamDatabase::mReadingOptions.mReadCountryCompetitions) {
        if (!reader.IsVersionGreaterOrEqual(0x2006, 0)) {
            reader.SkipLines(2);
            reader.ReadLine(Unknown._1);
            reader.ReadLine(Unknown._2);
        }
        reader.ReadLine(mEqualPointsSorting);
        reader.ReadLine(mNumNonEUPlayersAllowed);
        reader.ReadLine(mNumRelegatedTeams);
        if (reader.IsVersionGreaterOrEqual(0x2007, 0))
            reader.ReadLine(mRating); // TODO: auto-calculate this value for conversion
    }
    else
        reader.SkipLines(4);
}

void FifamCountryLeagueLevelInfo::Write(FifamWriter &writer, UInt minLeagueIndex, UInt maxLeagueIndex) {
    if (!writer.IsVersionGreaterOrEqual(0x2006, 0)) {
        writer.WriteLine(minLeagueIndex);
        writer.WriteLine(maxLeagueIndex);
        writer.WriteLine(Unknown._1);
        writer.WriteLine(Unknown._2);
    }
    writer.WriteLine(mEqualPointsSorting);
    writer.WriteLine(mNumNonEUPlayersAllowed);
    writer.WriteLine(mNumRelegatedTeams);
    if (writer.IsVersionGreaterOrEqual(0x2007, 0))
        writer.WriteLine(mRating);
}
