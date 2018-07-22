#include "FifamKit.h"

void FifamKit::Read(FifamReader &reader) {
    if (reader.ReadStartIndex(L"KIT")) {
        UChar secClrInd, trdClrInd;
        if (reader.GetGameId() < 9) {
            secClrInd = 2;
            trdClrInd = 1;
        }
        else {
            secClrInd = 1;
            trdClrInd = 2;
        }
        if (reader.IsVersionGreaterOrEqual(0x2012, 0x01)) {
            reader.ReadLine(mSpecialKitId);
            for (UInt i = 0; i < 2; i++) {
                reader.ReadLine(mSets[i].mShirt, mSets[i].mShorts, mSets[i].mSocks);
                reader.ReadLine(mSets[i].mShirtColors[0], mSets[i].mShirtColors[secClrInd], mSets[i].mShirtColors[trdClrInd]);
                reader.ReadLineArray(mSets[i].Unknown._1);
                reader.ReadLine(mSets[i].mShortsColors[0], mSets[i].mShortsColors[secClrInd], mSets[i].mShortsColors[trdClrInd]);
                reader.ReadLineArray(mSets[i].mSocksColors);
                reader.ReadLine(mSets[i].mShirtNumberColor, mSets[i].mBadgePosition);
            }
        }
        else {
            for (UInt i = 0; i < 2; i++) {
                reader.ReadLine(mSets[i].mShirt);
                reader.ReadLine(mSets[i].mShorts);
                reader.ReadLine(mSets[i].mSocks);
                reader.ReadLine(mSets[i].mShirtColors[0]);
                reader.ReadLine(mSets[i].mShirtColors[secClrInd]);
                reader.ReadLine(mSets[i].mShirtColors[trdClrInd]);
                for (UInt j = 0; j < 3; j++)
                    reader.ReadLine(mSets[i].Unknown._1[j]);
                reader.ReadLine(mSets[i].mShortsColors[0]);
                reader.ReadLine(mSets[i].mShortsColors[secClrInd]);
                reader.ReadLine(mSets[i].mShortsColors[trdClrInd]);
                for (UInt j = 0; j < 3; j++)
                    reader.ReadLine(mSets[i].mSocksColors[j]);
                reader.ReadLine(mSets[i].mShirtNumberColor);
                reader.ReadLine(mSets[i].mBadgePosition);
            }
            reader.ReadLine(mSpecialKitId);
        }
        reader.ReadEndIndex(L"KIT");
    }
}

void FifamKit::Write(FifamWriter &writer) {
    writer.WriteStartIndex(L"KIT");
    UChar shirt2ndClr, shirt3rdClr, shorts2ndClr, shorts3rdClr;
    if (writer.GetGameId() < 9) {
        shirt2ndClr = 2;
        shorts2ndClr = 2;
        shirt3rdClr = 1;
        shorts3rdClr = 1;
    }
    else {
        shirt2ndClr = 1;
        shorts2ndClr = 1;
        shirt3rdClr = 2;
        shorts3rdClr = 2;
    }
    Array<UChar, 2> shirt = {};
    for (UInt i = 0; i < 2; i++) {
        shirt[i] = mSets[i].mShirt;
        if (writer.GetGameId() < 12 && shirt[i] >= 43 && shirt[i] <= 66) {
            static Pair<UChar, Bool> kit12to11table[] = {
                { 42, false },
                { 14, false },
                { 19, false },
                {  1, false },
                { 13, false },
                { 21, false },
                {  1, false },
                {  1, false },
                { 10, false },
                { 10, false },
                { 22, false },
                { 12, false },
                { 14, false },
                { 22, false },
                { 14, true  },
                {  4, false },
                { 22, false },
                {  6, true  },
                { 26, false },
                { 11, true  },
                { 14, true  },
                { 21, false },
                { 21, false },
                { 11, false }
            };
            auto const &cnv = kit12to11table[shirt[i] - 43];
            shirt[i] = cnv.first;
            if (cnv.second) {
                std::swap(shirt2ndClr, shirt3rdClr);
            }
        }
    }
    if (writer.IsVersionGreaterOrEqual(0x2012, 0x01)) {
        writer.WriteLine(mSpecialKitId);
        for (UInt i = 0; i < 2; i++) {
            writer.WriteLine(shirt[i], mSets[i].mShorts, mSets[i].mSocks);
            writer.WriteLine(mSets[i].mShirtColors[0], mSets[i].mShirtColors[shirt2ndClr], mSets[i].mShirtColors[shirt3rdClr]);
            writer.WriteLineArray(mSets[i].Unknown._1);
            writer.WriteLine(mSets[i].mShortsColors[0], mSets[i].mShortsColors[shorts2ndClr], mSets[i].mShortsColors[shorts3rdClr]);
            writer.WriteLineArray(mSets[i].mSocksColors);
            writer.WriteLine(mSets[i].mShirtNumberColor, mSets[i].mBadgePosition);
        }
    }
    else {
        for (UInt i = 0; i < 2; i++) {
            writer.WriteLine(shirt[i]);
            writer.WriteLine(mSets[i].mShorts);
            writer.WriteLine(mSets[i].mSocks);
            writer.WriteLine(mSets[i].mShirtColors[0]);
            writer.WriteLine(mSets[i].mShirtColors[shirt2ndClr]);
            writer.WriteLine(mSets[i].mShirtColors[shirt3rdClr]);
            for (UInt j = 0; j < 3; j++)
                writer.WriteLine(mSets[i].Unknown._1[j]);
            writer.WriteLine(mSets[i].mShortsColors[0]);
            writer.WriteLine(mSets[i].mShortsColors[shorts2ndClr]);
            writer.WriteLine(mSets[i].mShortsColors[shorts3rdClr]);
            for (UInt j = 0; j < 3; j++)
                writer.WriteLine(mSets[i].mSocksColors[j]);
            writer.WriteLine(mSets[i].mShirtNumberColor);
            writer.WriteLine(mSets[i].mBadgePosition);
        }
        writer.WriteLine(mSpecialKitId);
    }
    writer.WriteEndIndex(L"KIT");
}
