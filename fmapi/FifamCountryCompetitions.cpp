#include "FifamCountryCompetitions.h"

Bool FifamCountryCompetitions::Read(FifamReader & reader) {
    if (reader.ReadStartIndex(L"COMPETITION")) {
        // TODO
        reader.ReadEndIndex(L"COMPETITION");
    }
    return true;
}

Bool FifamCountryCompetitions::Write(FifamWriter & writer) {
    writer.WriteStartIndex(L"COMPETITION");
    // TODO
    writer.WriteLine(0);
    writer.WriteLine(0);
    writer.WriteEndIndex(L"COMPETITION");
    return true;
}
