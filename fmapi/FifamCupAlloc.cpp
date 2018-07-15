#include "FifamCupAlloc.h"

void FifamCupAlloc::Read(FifamReader &reader) {
    mRounds.clear();
    NameToId(mType, reader.ReadLine<String>());
    reader.ReadLine(mNameKey);
    reader.ReadLine(mTeamsCount);
    UInt numRounds = reader.ReadLine<UInt>();
    mRounds.resize(numRounds);
    for (UInt i = 0; i < numRounds; i++) {
        mRounds[i] = new FifamCupAlloc::Round;
        auto round = mRounds[i];
        ClearFlags(round->mFlags);
        auto params = reader.ReadPackedLineArray<String>();
        for (UInt i = 0; i < params.size(); i++) {
            if (i == 0)
                round->mRound = Utils::ToNumber(params[i]);
            else if (i == 1)
                round->mNumRounds = Utils::ToNumber(params[i]);
            else if (i == 2)
                round->mTeamsRound = Utils::ToNumber(params[i]);
            else if (i == 3)
                round->mNewTeamsRound = Utils::ToNumber(params[i]);
            else if (i == 4)
                round->mStartBeg = Utils::ToNumber(params[i]);
            else if (i == 5)
                round->mEndBeg = Utils::ToNumber(params[i]);
            else if (i == 6)
                NameToId(round->mRoundID, params[i]);
            else
                NameToId_Flag(round->mFlags, params[i]);
        }
    }
    // TODO: read instructions
}

void FifamCupAlloc::Write(FifamWriter &writer) {
    writer.WriteLine(IdToName(mType));
    writer.WriteLine(mNameKey);
    writer.WriteLine(mTeamsCount);
    for (UInt i = 0; i < mRounds.size(); i++) {
        auto round = mRounds[i];
        writer.Write(Utils::Format(L"{ %d, %d, %2d, %2d, %3d, %3d, %s",
            round->mRound, round->mNumRounds, round->mTeamsRound, round->mNewTeamsRound,
            round->mStartBeg, round->mEndBeg, IdToName(round->mRoundID).c_str()));
        IterateFlags(round->mFlags, [&](FifamBeg flag) {
            writer.Write(L", ");
            writer.Write(IdToName(flag));
        });
        writer.WriteLine(L" }");
    }
    // TODO: write instructions
}

void FifamCupAlloc::Clear() {
    for (auto round : mRounds)
        delete round;
    mRounds.clear();
}


FifamCupAlloc::~FifamCupAlloc() {
    Clear();
}
