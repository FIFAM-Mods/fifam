#include "FifamStadium.h"

void FifamStadium::Read(FifamReader &reader) {
    if (reader.ReadStartIndex(L"STADIUM")) {
        reader.ReadLine(Unknown._1);
        reader.ReadFullLine(mName);
        reader.ReadFullLine(mCity);
        reader.ReadLine(mStadiumType);
        for (UInt i = 0; i < mParts.size(); i++) {
            reader.ReadLine(mParts[i].mSeats);
            reader.ReadLine(mParts[i].mTerraces);
            reader.ReadLine(mParts[i].mVIPSeats);
        }
        UChar seasonTicketBlockFlags = 0;
        reader.ReadLine(seasonTicketBlockFlags);
        for (UInt i = 0; i < mParts.size(); i++)
            mParts[i].mSeasonTicketBlock = (seasonTicketBlockFlags & (1 << i)) == (1 << i);
        UChar awaySupportersBlockFlags = 0;
        reader.ReadLine(awaySupportersBlockFlags);
        for (UInt i = 0; i < mParts.size(); i++)
            mParts[i].mAwaySupportersBlock = (awaySupportersBlockFlags & (1 << i)) == (1 << i);
        reader.ReadEndIndex(L"STADIUM");
    }
}

void FifamStadium::Write(FifamWriter &writer) {
    writer.WriteStartIndex(L"STADIUM");
    writer.WriteLine(Unknown._1);
    writer.WriteLine(mName);
    writer.WriteLine(mCity);
    writer.WriteLine(mStadiumType);
    for (UInt i = 0; i < mParts.size(); i++) {
        writer.WriteLine(mParts[i].mSeats);
        writer.WriteLine(mParts[i].mTerraces);
        writer.WriteLine(mParts[i].mVIPSeats);
    }
    UChar seasonTicketBlockFlags = 0;
    for (UInt i = 0; i < mParts.size(); i++) {
        if (mParts[i].mSeasonTicketBlock)
            seasonTicketBlockFlags |= (1 << i);
    }
    writer.WriteLine(seasonTicketBlockFlags);
    UChar awaySupportersBlockFlags = 0;
    for (UInt i = 0; i < mParts.size(); i++) {
        if (mParts[i].mAwaySupportersBlock)
            awaySupportersBlockFlags |= (1 << i);
    }
    writer.WriteLine(awaySupportersBlockFlags);
    writer.WriteEndIndex(L"STADIUM");
}
