#include "FifamPlayerContract.h"
#include "FifamUtils.h"

void FifamPlayerContract::Read(FifamReader &reader, FifamDatabase *database) {
    if (reader.ReadStartIndex(L"CONTRACT")) {
        UChar releaseClauseFlags = 0;
        UShort flags = 0;
        reader.ReadLine(
            mMatchesInternationalBonus,
            mSquadRole,
            mSalaryIncrease,
            mSalaryDecreaseOnRelegation,
            releaseClauseFlags,
            mAutoExtend,
            mPromisedRoleInTeam,
            mPromisedFuture,
            Unknown._1,
            mMatchesAppearanceBonus,
            mSellOnPercentage,
            flags,
            mSigningFee,
            mBasicSalary,
            mMatchBonus,
            mAppearanceBonus,
            mNoRelegationBonus,
            mPromotionBonus,
            mInternationalPremiereBonus,
            mGoalsCleanSheetBonus,
            mFixTransferFee,
            mSellOnFee,
            mInternationalBonus,
            mJoined
        );
        reader.ReadLine(mValidUntil);
        reader.ReadLine(
            mClauseForeignClub.mPercent,
            mClauseHigherLeague.mPercent,
            mClauseRelegation.mPercent,
            mClauseNoInternational.mPercent,
            mClauseNoPromotion.mPercent
        );
        if (reader.IsVersionGreaterOrEqual(0x2013, 0x09)) {
            UInt buyBackClauseClub;
            reader.ReadLine(mBuyBackClauseEnabled, mBuyBackClauseCost, buyBackClauseClub, mBuyBackClauseValidUntil);
            FifamUtils::SaveClubIDToClubLink(mBuyBackClauseClub, buyBackClauseClub);
        }
        reader.ReadEndIndex(L"CONTRACT");

        mClauseForeignClub.mEnabled = (releaseClauseFlags & 0x01) == 0x01;
        mClauseHigherLeague.mEnabled = (releaseClauseFlags & 0x02) == 0x02;
        mClauseRelegation.mEnabled = (releaseClauseFlags & 0x04) == 0x04;
        mClauseNoInternational.mEnabled = (releaseClauseFlags & 0x08) == 0x08;
        mClauseNoPromotion.mEnabled = (releaseClauseFlags & 0x10) == 0x10;

        mOptionClub = (flags >> 2) & 3;
        mHighestPaidPlayer = (flags & 0x0010) == 0x0010;
        mExtendAvoidRelegation = (flags & 0x0020) == 0x0020;
        mJobOption = (flags & 0x0040) == 0x0040;
        mOptionPlayer = (flags >> 13) & 3;
    }
}

void FifamPlayerContract::Write(FifamWriter &writer, FifamDatabase *database) {
    writer.WriteStartIndex(L"CONTRACT");
    UChar releaseClauseFlags = 0;
    if (mClauseForeignClub.mEnabled)
        releaseClauseFlags |= 0x01;
    if (mClauseHigherLeague.mEnabled)
        releaseClauseFlags |= 0x02;
    if (mClauseRelegation.mEnabled)
        releaseClauseFlags |= 0x04;
    if (mClauseNoInternational.mEnabled)
        releaseClauseFlags |= 0x08;
    if (mClauseNoPromotion.mEnabled)
        releaseClauseFlags |= 0x10;
    UShort flags = 0;
    if (mOptionClub)
        flags |= (mOptionClub & 3) << 2;
    if (mHighestPaidPlayer)
        flags |= 0x0010;
    if (mExtendAvoidRelegation)
        flags |= 0x0020;
    if (mJobOption)
        flags |= 0x0040;
    if (mOptionPlayer)
        flags |= (mOptionPlayer & 3) << 13;
    writer.WriteLine(
        mMatchesInternationalBonus,
        mSquadRole,
        mSalaryIncrease,
        mSalaryDecreaseOnRelegation,
        releaseClauseFlags,
        mAutoExtend,
        mPromisedRoleInTeam,
        mPromisedFuture,
        Unknown._1,
        mMatchesAppearanceBonus,
        mSellOnPercentage,
        flags,
        mSigningFee,
        mBasicSalary,
        mMatchBonus,
        mAppearanceBonus,
        mNoRelegationBonus,
        mPromotionBonus,
        mInternationalPremiereBonus,
        mGoalsCleanSheetBonus,
        mFixTransferFee,
        mSellOnFee,
        mInternationalBonus,
        mJoined
    );
    writer.WriteLine(mValidUntil);
    writer.WriteLine(
        mClauseForeignClub.mPercent,
        mClauseHigherLeague.mPercent,
        mClauseRelegation.mPercent,
        mClauseNoInternational.mPercent,
        mClauseNoPromotion.mPercent
    );
    if (writer.IsVersionGreaterOrEqual(0x2013, 0x09)) {
        writer.WriteLine(mBuyBackClauseEnabled, mBuyBackClauseCost,
            FifamUtils::DBClubLinkToID(database, mBuyBackClauseClub, writer.GetGameId()), mBuyBackClauseValidUntil);
    }
    writer.WriteEndIndex(L"CONTRACT");
}
