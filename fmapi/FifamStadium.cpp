#include "FifamStadium.h"

void FifamStadium::Read(FifamReader &reader) {
    if (reader.ReadStartIndex(L"STADIUM")) {
        reader.ReadFullLine(mName);
        reader.ReadFullLine(mCity);
        reader.ReadLine(Unknown._1);
        for (UInt i = 0; i < 8; i++) {
            reader.ReadLine(Unknown._data1[i]._1);
            reader.ReadLine(Unknown._data1[i]._2);
            reader.ReadLine(Unknown._data1[i]._3);
        }
        reader.ReadLine(Unknown._2);
        reader.ReadLine(Unknown._3);
        reader.ReadEndIndex(L"STADIUM");
    }
}

void FifamStadium::Write(FifamWriter &writer) {
    writer.WriteStartIndex(L"STADIUM");
    writer.WriteLine(mName);
    writer.WriteLine(mCity);
    writer.WriteLine(Unknown._1);
    for (UInt i = 0; i < 8; i++) {
        writer.WriteLine(Unknown._data1[i]._1);
        writer.WriteLine(Unknown._data1[i]._2);
        writer.WriteLine(Unknown._data1[i]._3);
    }
    writer.WriteLine(Unknown._2);
    writer.WriteLine(Unknown._3);
    writer.WriteEndIndex(L"STADIUM");
}
