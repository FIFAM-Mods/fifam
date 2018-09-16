#include "FifamCupAlloc.h"

void FifamCupAlloc::Read(FifamReader &reader, FifamDatabase *database) {
    mRounds.clear();

    reader.ReadLine(mType);
    reader.ReadFullLine(mNameKey);
    reader.ReadLine(mTeamsCount);
    UInt numRounds = reader.ReadLine<UInt>();
    mRounds.resize(numRounds);
    for (UInt i = 0; i < numRounds; i++) {
        mRounds[i] = new FifamCupAlloc::Round;
        auto round = mRounds[i];
        round->mFlags.Clear();
        auto params = reader.ReadPackedLineArray<String>();
        for (UInt i = 0; i < params.size(); i++) {
            if (i == 1)
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
                round->mRoundID.SetFromStr(params[i]);
            else if(i != 0)
                round->mFlags.Set(params[i], true);
        }
    }
    mInstructions.Read(reader, database, FifamNation::None);
}

void FifamCupAlloc::Write(FifamWriter &writer, FifamDatabase *database) {
    writer.WriteLine(mType);
    writer.WriteLine(mNameKey);
    writer.WriteLine(mTeamsCount);
    writer.WriteLine(mRounds.size() + 1);
    for (UInt i = 0; i < mRounds.size(); i++) {
        auto round = mRounds[i];
        writer.Write(Utils::Format(L"{ %d, %d, %2d, %2d, %3d, %3d, %s",
            i, round->mNumRounds, round->mTeamsRound, round->mNewTeamsRound,
            round->mStartBeg, round->mEndBeg, round->mRoundID.ToCStr()));
        writer.Write(round->mFlags.ToStr(L", "));
        writer.WriteLine(L" }");
    }
    // TODO: write instructions
    writer.WriteLine(0);
}

void FifamCupAlloc::Clear() {
    for (auto round : mRounds)
        delete round;
    mRounds.clear();
}


FifamCupAlloc::~FifamCupAlloc() {
    Clear();
}
