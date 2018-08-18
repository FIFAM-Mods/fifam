#include "FifamPlayerHistoryEntry.h"
#include "FifamUtils.h"
#include "FifamClub.h"
#include "FifamCountry.h"

void FifamPlayerHistoryEntry::Read(FifamReader &reader, FifamDatabase *database) {
    UChar flags = 0;
    UInt club = 0;
    if (reader.IsVersionGreaterOrEqual(0x2012, 0x01)) {
        reader.ReadLine(mStartDate);
        reader.ReadLine(mEndDate);
        reader.ReadLine(club, Unknown._1, mMatches, mReserveMatches, mGoals, mReserveGoals, mTransferFee, flags);
    }
    else if (reader.IsVersionGreaterOrEqual(0x2007, 0x0D)) {
        reader.ReadLine(Unknown._1);
        reader.ReadLine(club);
        reader.ReadLine(mStartDate);
        reader.ReadLine(mEndDate);
        reader.ReadLine(mMatches);
        reader.ReadLine(mReserveMatches);
        reader.ReadLine(mGoals);
        reader.ReadLine(mReserveGoals);
        reader.ReadLine(mTransferFee);
        reader.ReadLine(flags);
    }
    else {
        reader.ReadLine(Unknown._1);
        reader.SkipLine();
        reader.ReadLine(club);
        reader.ReadLine(mStartDate);
        reader.ReadLine(mEndDate);
        reader.ReadLine(mMatches);
        reader.ReadLine(mGoals);
        reader.ReadLine(flags);
    }
    FifamUtils::SaveClubIDToClubLink(mClub, club);
    if (flags & 1)
        mLoan = true;
    if (reader.IsVersionGreaterOrEqual(0x2007, 0x0D) && flags & 2)
        mStillInThisClub = true;
}

void FifamPlayerHistoryEntry::Write(FifamWriter &writer, FifamDatabase *database) {
    UChar flags = 0;
    if (mLoan)
        flags |= 1;
    if (writer.IsVersionGreaterOrEqual(0x2007, 0x0D) && mStillInThisClub)
        flags |= 2;
    UInt club = FifamUtils::DBClubLinkToID(database, mClub, writer.GetGameId());
    if (writer.IsVersionGreaterOrEqual(0x2012, 0x01)) {
        writer.WriteLine(mStartDate);
        writer.WriteLine(mEndDate);
        writer.WriteLine(club, Unknown._1, mMatches, mReserveMatches, mGoals, mReserveGoals, mTransferFee, flags);
    }
    else if (writer.IsVersionGreaterOrEqual(0x2007, 0x0D)) {
        writer.WriteLine(Unknown._1);
        writer.WriteLine(club);
        writer.WriteLine(mStartDate);
        writer.WriteLine(mEndDate);
        writer.WriteLine(mMatches);
        writer.WriteLine(mReserveMatches);
        writer.WriteLine(mGoals);
        writer.WriteLine(mReserveGoals);
        writer.WriteLine(mTransferFee);
        writer.WriteLine(flags);
    }
    else {
        writer.WriteLine(Unknown._1);
        if (mClub.IsValid() && mClub.mPtr->mCountry && club) {
            writer.WriteLine(mClub.mPtr->mCountry->mId);
            writer.WriteLine(club);
        }
        else {
            writer.WriteLine<UInt>(0);
            writer.WriteLine<UInt>(0);
        }
        writer.WriteLine(mStartDate);
        writer.WriteLine(mEndDate);
        writer.WriteLine(mMatches);
        writer.WriteLine(mGoals);
        writer.WriteLine(flags);
    }
}
