#pragma once
#include "TestsEnvironment.h"
#include "FifamTypes.h"
#include "FifamReadWrite.h"

class FifaFifamIDsEnvironment {
public:
    Map<UInt, UInt> mFifamClubsToFifa; // FIFAM ID => FIFA ID
    Map<UInt, UInt> mFifaClubs; // FIFA ID => FIFAM ID
    Map<UInt, UInt> mFifamClubsToFoom; // FIFAM ID => FooM ID
    Map<UInt, UInt> mFoomClubs; // FooM ID => FIFAM ID

    Path mInfoPath = L"D:\\Projects\\fifam\\db";

    FifaFifamIDsEnvironment() {
        {
            FifamReader reader(mInfoPath / L"fifam_countries.txt", 0);
            if (reader.Available()) {
                reader.SkipLine();
                while (!reader.IsEof()) {
                    if (!reader.EmptyLine()) {
                        String dummy;
                        UInt fmId = 0, fifaId = 0, foomId = 0;
                        reader.ReadLineWithSeparator(L'\t', dummy, dummy, dummy, dummy, foomId, dummy, dummy, fmId, dummy, fifaId);
                        if (fmId != 0) {
                            if (fifaId != 0)
                                mFifamClubsToFifa[(fmId << 16) | 0xFFFF] = fifaId;
                            if (foomId != 0)
                                mFifamClubsToFoom[(fmId << 16) | 0xFFFF] = foomId;
                        }
                    }
                    else
                        reader.SkipLine();
                }
            }
        }
        {
            FifamReader reader(mInfoPath / L"fifam-uids.csv", 0);
            if (reader.Available()) {
                reader.SkipLine();
                while (!reader.IsEof()) {
                    if (!reader.EmptyLine()) {
                        String dummy;
                        UInt fmId = 0, fifaId = 0, foomId = 0;
                        reader.ReadLine(dummy, dummy, dummy, dummy, dummy, foomId, Hexadecimal(fmId), fifaId);
                        if (fmId != 0) {
                            if (fifaId != 0)
                                mFifamClubsToFifa[fmId] = fifaId;
                            if (foomId != 0)
                                mFifamClubsToFoom[fmId] = foomId;
                        }
                    }
                    else
                        reader.SkipLine();
                }
            }
        }
        
        for (auto const &c : mFifamClubsToFifa) {
            if (c.second != 0)
                mFifaClubs[c.second] = c.first;
        }
        for (auto const& c : mFifamClubsToFoom) {
            if (c.second != 0)
                mFoomClubs[c.second] = c.first;
        }
    }
};
