#include "FifamCountryLeagueLevelInfo.h"
#include "FifamDatabase.h"

void FifamCountryLeagueLevelInfo::Read(FifamReader &reader) {
    if (FifamDatabase::mReadingOptions.mReadCountryCompetitions) {
        reader.ReadLine(mEqualPointsSorting);
        reader.ReadLine(mNumNonEUPlayersAllowed);
        reader.ReadLine(mNumRelegatedTeams);
        reader.ReadLine(mRating);
    }
    else
        reader.SkipLines(4);
}

void FifamCountryLeagueLevelInfo::Write(FifamWriter &writer) {
    writer.WriteLine(mEqualPointsSorting);
    writer.WriteLine(mNumNonEUPlayersAllowed);
    writer.WriteLine(mNumRelegatedTeams);
    writer.WriteLine(mRating);
}
