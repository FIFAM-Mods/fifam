#include "FifamCountryLeagueLevelInfo.h"

void FifamCountryLeagueLevelInfo::Read(FifamReader &reader) {
    reader.ReadLine(mEqualPointsSorting);
    reader.ReadLine(mNumNonEUPlayersAllowed);
    reader.ReadLine(mNumRelegatedTeams);
    reader.ReadLine(mRating);
}

void FifamCountryLeagueLevelInfo::Write(FifamWriter &writer) {
    writer.WriteLine(mEqualPointsSorting);
    writer.WriteLine(mNumNonEUPlayersAllowed);
    writer.WriteLine(mNumRelegatedTeams);
    writer.WriteLine(mRating);
}
