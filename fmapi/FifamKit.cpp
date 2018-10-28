#include "FifamKit.h"

Color FifamKit::GetShirtBackColor(UInt shirtId, Color const & firstColor, Color const & secondColor, Color const & thirdColor) {
    if (shirtId == 64)
        return thirdColor;
    return firstColor;
}

void FifamKit::Read(FifamReader &reader) {
    if (reader.ReadStartIndex(L"KIT")) {
        UChar shirtColors[2][3];
        UChar sleevesColors[2][3];
        UChar shortsColors[2][3];
        UChar socksColors[2][2];
        UChar armbandColor[2];
        UChar shirtNumberColor[2];
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
                reader.ReadLine(shirtColors[i][0], shirtColors[i][secClrInd], shirtColors[i][trdClrInd]);
                reader.ReadLine(sleevesColors[i][0], sleevesColors[i][secClrInd], sleevesColors[i][trdClrInd]);
                reader.ReadLine(shortsColors[i][0], shortsColors[i][secClrInd], shortsColors[i][trdClrInd]);
                reader.ReadLine(socksColors[i][0], socksColors[i][1], armbandColor[i]);
                reader.ReadLine(shirtNumberColor[i], mSets[i].mBadgePosition);
            }
        }
        else {
            for (UInt i = 0; i < 2; i++) {
                reader.ReadLine(mSets[i].mShirt);
                reader.ReadLine(mSets[i].mShorts);
                reader.ReadLine(mSets[i].mSocks);
                reader.ReadLine(shirtColors[i][0]);
                reader.ReadLine(shirtColors[i][secClrInd]);
                reader.ReadLine(shirtColors[i][trdClrInd]);
                reader.ReadLine(sleevesColors[i][0]);
                reader.ReadLine(sleevesColors[i][secClrInd]);
                reader.ReadLine(sleevesColors[i][trdClrInd]);
                reader.ReadLine(shortsColors[i][0]);
                reader.ReadLine(shortsColors[i][secClrInd]);
                reader.ReadLine(shortsColors[i][trdClrInd]);
                for (UInt j = 0; j < 2; j++)
                    reader.ReadLine(socksColors[i][j]);
                reader.ReadLine(armbandColor[i]);
                reader.ReadLine(shirtNumberColor[i]);
                reader.ReadLine(mSets[i].mBadgePosition);
            }
            reader.ReadLine(mSpecialKitId);
        }
        reader.ReadEndIndex(L"KIT");
        for (UInt i = 0; i < 2; i++) {
            for (UInt j = 0; j < 3; j++) {
                mSets[i].mShirtColors[j].SetFromTable(mKitColorTable, shirtColors[i][j]);
                mSets[i].mSleevesColors[j].SetFromTable(mKitColorTable, sleevesColors[i][j]);
                mSets[i].mShortsColors[j].SetFromTable(mKitColorTable, shortsColors[i][j]);
                if (j != 2)
                    mSets[i].mSocksColors[j].SetFromTable(mKitColorTable, socksColors[i][j]);
            }
            mSets[i].mCaptainArmbandColor.SetFromTable(mKitColorTable, armbandColor[i]);
            mSets[i].mShirtNumberColor.SetFromTable(mShirtNumberColorTable, shirtNumberColor[i]);
        }
    }
}

void FifamKit::Write(FifamWriter &writer) {
    writer.WriteStartIndex(L"KIT");
    UChar shirtColors[2][3];
    UChar sleevesColors[2][3];
    UChar shortsColors[2][3];
    UChar socksColors[2][2];
    UChar armbandColor[2];
    UChar shirtNumberColor[2];
    for (UInt i = 0; i < 2; i++) {
        for (UInt j = 0; j < 3; j++) {
            shirtColors[i][j] = static_cast<UChar>(mSets[i].mShirtColors[j].FindIndexInTable(mKitColorTable));
            sleevesColors[i][j] = static_cast<UChar>(mSets[i].mSleevesColors[j].FindIndexInTable(mKitColorTable));
            shortsColors[i][j] = static_cast<UChar>(mSets[i].mShortsColors[j].FindIndexInTable(mKitColorTable));
            if (j != 2)
                socksColors[i][j] = static_cast<UChar>(mSets[i].mSocksColors[j].FindIndexInTable(mKitColorTable));
        }
        armbandColor[i] = static_cast<UChar>(mSets[i].mCaptainArmbandColor.FindIndexInTable(mKitColorTable));
        shirtNumberColor[i] = static_cast<UChar>(mSets[i].mShirtNumberColor.FindIndexInTable(mShirtNumberColorTable));
    }
    UChar shirt2ndClr, shirt3rdClr, _2ndClr, _3rdClr;
    if (writer.GetGameId() < 9) {
        shirt2ndClr = 2;
        _2ndClr = 2;
        shirt3rdClr = 1;
        _3rdClr = 1;
    }
    else {
        shirt2ndClr = 1;
        _2ndClr = 1;
        shirt3rdClr = 2;
        _3rdClr = 2;
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
            writer.WriteLine(shirtColors[i][0], shirtColors[i][shirt2ndClr], shirtColors[i][shirt3rdClr]);
            writer.WriteLine(sleevesColors[i][0], sleevesColors[i][_2ndClr], sleevesColors[i][_3rdClr]);
            writer.WriteLine(shortsColors[i][0], shortsColors[i][_2ndClr], shortsColors[i][_3rdClr]);
            writer.WriteLine(socksColors[i][0], socksColors[i][1], armbandColor[i]);
            writer.WriteLine(shirtNumberColor[i], mSets[i].mBadgePosition);
        }
    }
    else {
        for (UInt i = 0; i < 2; i++) {
            writer.WriteLine(shirt[i]);
            writer.WriteLine(mSets[i].mShorts);
            writer.WriteLine(mSets[i].mSocks);
            writer.WriteLine(shirtColors[i][0]);
            writer.WriteLine(shirtColors[i][shirt2ndClr]);
            writer.WriteLine(shirtColors[i][shirt3rdClr]);
            writer.WriteLine(sleevesColors[i][0]);
            writer.WriteLine(sleevesColors[i][_2ndClr]);
            writer.WriteLine(sleevesColors[i][_3rdClr]);
            writer.WriteLine(shortsColors[i][0]);
            writer.WriteLine(shortsColors[i][_2ndClr]);
            writer.WriteLine(shortsColors[i][_3rdClr]);
            for (UInt j = 0; j < 2; j++)
                writer.WriteLine(socksColors[i][j]);
            writer.WriteLine(armbandColor[i]);
            writer.WriteLine(shirtNumberColor[i]);
            writer.WriteLine(mSets[i].mBadgePosition);
        }
        writer.WriteLine(mSpecialKitId);
    }
    writer.WriteEndIndex(L"KIT");
}
