#include "FifamSponsor.h"

FifamSponsor::FifamSponsor() {}

FifamSponsor::FifamSponsor(FifamSponsor const &rhs) {
    Unknown._1 = rhs.Unknown._1;
    Unknown._2 = rhs.Unknown._2;
    Unknown._3 = rhs.Unknown._3;
    Unknown._4 = rhs.Unknown._4;
}

void FifamSponsor::Read(FifamReader &reader) {
    if (reader.ReadStartIndex(L"SPONSOR")) {
        reader.ReadFullLine(Unknown._1);
        reader.ReadLine(Unknown._2);
        reader.ReadFullLine(Unknown._3);
        reader.ReadFullLine(Unknown._4);
        reader.ReadEndIndex(L"SPONSOR");
    }
}

void FifamSponsor::Write(FifamWriter &writer) {
    writer.WriteStartIndex(L"SPONSOR");
    writer.WriteLine(Unknown._1);
    writer.WriteLine(Unknown._2);
    writer.WriteLine(Unknown._3);
    writer.WriteLine(Unknown._4);
    writer.WriteEndIndex(L"SPONSOR");
}
