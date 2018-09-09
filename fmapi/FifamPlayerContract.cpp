#include "FifamPlayerContract.h"
#include "FifamUtils.h"

void FifamPlayerContract::Read(FifamReader &reader) {
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

        mClauseForeignClub.mEnabled = Utils::CheckFlag(releaseClauseFlags, 0x01);
        mClauseHigherLeague.mEnabled = Utils::CheckFlag(releaseClauseFlags, 0x02);
        mClauseRelegation.mEnabled = Utils::CheckFlag(releaseClauseFlags, 0x04);
        mClauseNoInternational.mEnabled = Utils::CheckFlag(releaseClauseFlags, 0x08);
        mClauseNoPromotion.mEnabled = Utils::CheckFlag(releaseClauseFlags, 0x10);

        mOptionClub = (flags >> 2) & 3;
        mHighestPaidPlayer = Utils::CheckFlag(flags, 0x0010);
        mExtendAvoidRelegation = Utils::CheckFlag(flags, 0x0020);
        mJobOption = Utils::CheckFlag(flags, 0x0040);
        mLoaned = Utils::CheckFlag(flags, 0x0080);
        mOptionPlayer = (flags >> 13) & 3;
    }
}

void FifamPlayerContract::Write(FifamWriter &writer) {
    writer.WriteStartIndex(L"CONTRACT");
    UChar releaseClauseFlags = 0;
    Utils::SetFlag(releaseClauseFlags, 0x01, mClauseForeignClub.mEnabled);
    Utils::SetFlag(releaseClauseFlags, 0x02, mClauseHigherLeague.mEnabled);
    Utils::SetFlag(releaseClauseFlags, 0x04, mClauseRelegation.mEnabled);
    Utils::SetFlag(releaseClauseFlags, 0x08, mClauseNoInternational.mEnabled);
    Utils::SetFlag(releaseClauseFlags, 0x10, mClauseNoPromotion.mEnabled);
    UShort flags = 0;
    if (mOptionClub)
        flags |= (mOptionClub & 3) << 2;
    Utils::SetFlag(flags, 0x0010, mHighestPaidPlayer);
    Utils::SetFlag(flags, 0x0020, mExtendAvoidRelegation);
    Utils::SetFlag(flags, 0x0040, mJobOption);
    Utils::SetFlag(flags, 0x0080, mLoaned);
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
        UInt buyBackClauseClub = FifamUtils::GetWriteableID(mBuyBackClauseClub);
        if (buyBackClauseClub)
            writer.WriteLine(mBuyBackClauseEnabled, mBuyBackClauseCost, buyBackClauseClub, mBuyBackClauseValidUntil);
        else
            writer.WriteLine(0, 0, 0, FifamDate(0, 0, 0));
    }
    writer.WriteEndIndex(L"CONTRACT");
}
