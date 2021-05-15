#include "FifamPlayerStartingConditions.h"
#include "FifamUtils.h"

Bool FifamPlayerStartingCondition::Enabled() {
    return mEnabled;
}

void FifamPlayerStartingCondition::Enable() {
    mEnabled = true;
}

void FifamPlayerStartingCondition::Disable() {
    mEnabled = false;
}

void FifamPlayerInjury::Setup(FifamDate const &startDate, FifamDate const &endDate, FifamPlayerInjuryType const &type) {
    Enable();
    mStartDate = startDate;
    mEndDate = endDate;
    mType = type;
}

void FifamPlayerLeagueBan::Setup(UInt numMatches) {
    Enable();
    mNumMatches = numMatches;
}

void FifamPlayerRetirement::Setup() {
    Enable();
}

void FifamPlayerLoan::Setup(FifamDate const &startDate, FifamDate const &endDate, FifamClubLink const &loanedClub, Int buyOptionValue) {
    Enable();
    mStartDate = startDate;
    mEndDate = endDate;
    mLoanedClub = loanedClub;
    mBuyOptionValue = buyOptionValue;
}

void FifamPlayerFutureTransfer::Setup(FifamDate const &transferDate, FifamDate const &newContractEndDate, FifamClubLink const &newClub, UInt transferFee) {
    Enable();
    mTransferDate = transferDate;
    mNewContractEndDate = newContractEndDate;
    mNewClub = newClub;
    mTransferFee = transferFee;
}

void FifamPlayerFutureLoan::Setup(FifamDate const &startDate, FifamDate const &endDate, FifamClubLink const &loanedClub, Int buyOptionValue, UInt loanFee) {
    Enable();
    mStartDate = startDate;
    mEndDate = endDate;
    mLoanedClub = loanedClub;
    mBuyOptionValue = buyOptionValue;
    mLoanFee = loanFee;
}

void FifamPlayerConditionDate::Setup(FifamDate const &date) {
    Enable();
    mDate = date;
}

UInt FifamPlayerStartingConditions::GetNumEnabledConditionsForWriting() {
    UInt result = 0;
    if (mInjury.mEnabled)
        result++;
    if (mLeagueBan.mEnabled)
        result++;
    if (mRetirement.mEnabled)
        result++;
    if (mLoan.mEnabled && FifamUtils::GetWriteableID(mLoan.mLoanedClub))
        result++;
    if (mFutureTransfer.mEnabled && FifamUtils::GetWriteableID(mFutureTransfer.mNewClub))
        result++;
    if (mFutureLoan.mEnabled && FifamUtils::GetWriteableID(mFutureLoan.mLoanedClub))
        result++;
    if (mBanUntil.mEnabled)
        result++;
    if (mFutureJoin.mEnabled)
        result++;
    if (mFutureReLoan.mEnabled && FifamUtils::GetWriteableID(mFutureReLoan.mLoanedClub))
        result++;
    if (mFutureLeave.mEnabled)
        result++;
    return result;
}

void FifamPlayerStartingConditions::Read(FifamReader &reader, UInt contractFlags) {
    if (reader.IsVersionGreaterOrEqual(0x2011, 0x03)) {
        UInt numConditions = reader.ReadLine<UInt>();
        for (UInt i = 0; i < numConditions; i++) {
            UInt type = 0;
            Array<UInt, 5> params;
            reader.ReadLine(type, params[0], params[1], params[2], params[3], params[4]);
            if (type == 1)
                mInjury.Setup(FifamDate(params[0]), FifamDate(params[1]), FifamPlayerInjuryType::MakeFromInt(params[3]));
            else if (type == 2)
                mLeagueBan.Setup(params[3]);
            else if (type == 3)
                mRetirement.Setup();
            else if (type == 4) {
                mLoan.Setup(FifamDate(params[0]), FifamDate(params[1]), FifamClubLink(nullptr), static_cast<Int>(params[3]));
                FifamUtils::SaveClubIDToClubLink(mLoan.mLoanedClub, params[2]);
            }
            else if (type == 5) {
                mFutureTransfer.Setup(FifamDate(params[0]), FifamDate(params[1]), FifamClubLink(nullptr), params[3]);
                FifamUtils::SaveClubIDToClubLink(mFutureTransfer.mNewClub, params[2]);
            }
            else if (type == 6) {
                mFutureLoan.Setup(FifamDate(params[0]), FifamDate(params[1]), FifamClubLink(nullptr),
                    static_cast<Int>(params[3]), params[4]);
                FifamUtils::SaveClubIDToClubLink(mFutureLoan.mLoanedClub, params[2]);
            }
            else if (type == 7)
                mBanUntil.Setup(FifamDate(params[1]));
            else if (type == 8)
                mFutureJoin.Setup(FifamDate(params[0]));
            else if (type == 9) {
                mFutureReLoan.Setup(FifamDate(params[0]), FifamDate(params[1]), FifamClubLink(nullptr),
                    static_cast<Int>(params[3]), params[4]);
                FifamUtils::SaveClubIDToClubLink(mFutureReLoan.mLoanedClub, params[2]);
            }
            else if (type == 10)
                mFutureLeave.Setup(FifamDate(params[0]));
        }
    }
    else {
        UChar startConditionFlags = 0;
        //if ()
        reader.ReadLine<UChar>();
        reader.SkipLine();
        UInt transferClub = reader.ReadLine<UInt>();
        FifamDate transferStartDate = reader.ReadLine<FifamDate>();
        FifamDate transferEndDate = reader.ReadLine<FifamDate>();
        UChar numBanMatches = reader.ReadLine<UChar>();
        UChar injuryType = reader.ReadLine<UChar>();
        FifamDate injuryStartDate = reader.ReadLine<FifamDate>();
        FifamDate injuryEndDate = reader.ReadLine<FifamDate>();
        Int buyOption = 0;
        if (reader.IsVersionGreaterOrEqual(0x2007, 0x10))
            reader.ReadLine(buyOption);
        UInt transferFee = reader.ReadLine<UInt>();
        if ((startConditionFlags & 1) || injuryType != 0)
            mInjury.Setup(injuryStartDate, injuryEndDate, FifamPlayerInjuryType::MakeFromInt(injuryType));
        if (startConditionFlags & 2)
            mLeagueBan.Setup(numBanMatches);
        if (startConditionFlags & 4)
            mRetirement.Setup();
        if (startConditionFlags & 8) {
            mLoan.Setup(transferStartDate, transferEndDate, FifamClubLink(nullptr), buyOption);
            FifamUtils::SaveClubIDToClubLink(mLoan.mLoanedClub, transferClub);
        }
        if (reader.GetGameId() >= 7) {
            if (startConditionFlags & 16) {
                mFutureTransfer.Setup(transferStartDate, transferEndDate, FifamClubLink(nullptr), transferFee);
                FifamUtils::SaveClubIDToClubLink(mFutureTransfer.mNewClub, transferClub);
            }
            if (startConditionFlags & 32) {
                mFutureLoan.Setup(transferStartDate, transferEndDate, FifamClubLink(nullptr), buyOption, transferFee);
                FifamUtils::SaveClubIDToClubLink(mFutureLoan.mLoanedClub, transferClub);
            }
            if (reader.GetGameId() >= 11) {
                if (startConditionFlags & 64)
                    mBanUntil.Setup(transferEndDate);
            }
        }
    }
}

void FifamPlayerStartingConditions::Write(FifamWriter &writer) {
    if (writer.IsVersionGreaterOrEqual(0x2011, 0x03)) {
        UInt numConditions = Utils::Min(4u, GetNumEnabledConditionsForWriting());
        writer.WriteLine(numConditions);
        UInt numWrittenConditions = 0;
        if (numWrittenConditions < numConditions && mLoan.Enabled() && FifamUtils::GetWriteableID(mLoan.mLoanedClub)) {
            writer.WriteLine(4, mLoan.mStartDate.GetDays(), mLoan.mEndDate.GetDays(),
                FifamUtils::GetWriteableID(mLoan.mLoanedClub), mLoan.mBuyOptionValue, 0);
            numWrittenConditions++;
        }
        if (numWrittenConditions < numConditions && mFutureTransfer.mEnabled && FifamUtils::GetWriteableID(mFutureTransfer.mNewClub)) {
            writer.WriteLine(5, mFutureTransfer.mTransferDate.GetDays(), mFutureTransfer.mNewContractEndDate.GetDays(),
                FifamUtils::GetWriteableID(mFutureTransfer.mNewClub), mFutureTransfer.mTransferFee, 0);
            numWrittenConditions++;
        }
        if (numWrittenConditions < numConditions && mFutureLoan.mEnabled && FifamUtils::GetWriteableID(mFutureLoan.mLoanedClub)) {
            writer.WriteLine(6, mFutureLoan.mStartDate.GetDays(), mFutureLoan.mEndDate.GetDays(),
                FifamUtils::GetWriteableID(mFutureLoan.mLoanedClub), mFutureLoan.mBuyOptionValue, mFutureLoan.mLoanFee);
            numWrittenConditions++;
        }
        if (writer.GetGameId() >= 12 && numWrittenConditions < numConditions && mFutureJoin.mEnabled) {
            writer.WriteLine(8, mFutureJoin.mDate.GetDays(), 0, 0, 0, 0);
            numWrittenConditions++;
        }
        if (writer.GetGameId() >= 12 && numWrittenConditions < numConditions && mFutureReLoan.mEnabled && FifamUtils::GetWriteableID(mFutureReLoan.mLoanedClub)) {
            writer.WriteLine(9, mFutureReLoan.mStartDate.GetDays(), mFutureReLoan.mEndDate.GetDays(),
                FifamUtils::GetWriteableID(mFutureReLoan.mLoanedClub), mFutureReLoan.mBuyOptionValue, mFutureReLoan.mLoanFee);
            numWrittenConditions++;
        }
        if (writer.GetGameId() >= 13 && numWrittenConditions < numConditions && mFutureLeave.mEnabled) {
            writer.WriteLine(10, mFutureLeave.mDate.GetDays(), 0, 0, 0, 0);
            numWrittenConditions++;
        }
        if (numWrittenConditions < numConditions && mInjury.mEnabled) {
            writer.WriteLine(1, mInjury.mStartDate.GetDays(), mInjury.mEndDate.GetDays(), 0, mInjury.mType, 0);
            numWrittenConditions++;
        }
        if (numWrittenConditions < numConditions && mLeagueBan.mEnabled) {
            writer.WriteLine(2, 0, 0, 0, mLeagueBan.mNumMatches, 0);
            numWrittenConditions++;
        }
        if (numWrittenConditions < numConditions && mRetirement.mEnabled) {
            writer.WriteLine(3, 0, 0, 0, 0, 0);
            numWrittenConditions++;
        }
        if (numWrittenConditions < numConditions && mBanUntil.mEnabled) {
            writer.WriteLine(7, 0, mBanUntil.mDate.GetDays(), 0, 0, 0);
            numWrittenConditions++;
        }
    }
    else {
        UInt numConditions = 0;
        UInt numWrittenConditions = 0;
        if (writer.GetGameId() >= 10)
            numConditions = Utils::Min(4u, GetNumEnabledConditionsForWriting());
        else
            numConditions = Utils::Min(1u, GetNumEnabledConditionsForWriting());

        UChar startConditionFlags = 0;
        FifamClubLink transferClub;
        FifamDate transferStartDate;
        FifamDate transferEndDate;
        UChar numBanMatches = 0;
        UInt injuryType;
        FifamDate injuryStartDate;
        FifamDate injuryEndDate;
        UInt transferFee = 0;
        Int transferBuyOption = 0;

        if (numWrittenConditions < numConditions && mLoan.Enabled() && FifamUtils::GetWriteableID(mLoan.mLoanedClub)) {
            startConditionFlags |= 8;
            transferStartDate = mLoan.mStartDate;
            transferEndDate = mLoan.mEndDate;
            transferClub = mLoan.mLoanedClub;
            transferBuyOption = mLoan.mBuyOptionValue;
            numWrittenConditions++;
        }
        else if (writer.GetGameId() >= 7 && numWrittenConditions < numConditions && mFutureTransfer.mEnabled && FifamUtils::GetWriteableID(mFutureTransfer.mNewClub)) {
            startConditionFlags |= 16;
            transferStartDate = mFutureTransfer.mTransferDate;
            transferEndDate = mFutureTransfer.mNewContractEndDate;
            transferClub = mFutureTransfer.mNewClub;
            transferFee = mFutureTransfer.mTransferFee;
            numWrittenConditions++;
        }
        else if (writer.GetGameId() >= 7 && numWrittenConditions < numConditions && mFutureLoan.mEnabled && FifamUtils::GetWriteableID(mFutureLoan.mLoanedClub)) {
            startConditionFlags |= 32;
            transferStartDate = mFutureLoan.mStartDate;
            transferEndDate = mFutureLoan.mEndDate;
            transferClub = mFutureLoan.mLoanedClub;
            transferBuyOption = mFutureLoan.mBuyOptionValue;
            transferFee = mFutureLoan.mLoanFee;
            numWrittenConditions++;
        }
        else if (writer.GetGameId() >= 11 && numWrittenConditions < numConditions && mBanUntil.mEnabled) {
            startConditionFlags |= 64;
            transferEndDate = mBanUntil.mDate;
            numWrittenConditions++;
        }
        
        if (numWrittenConditions < numConditions && mInjury.mEnabled) {
            startConditionFlags |= 1;
            injuryType = mInjury.mType.ToInt();
            injuryStartDate = mInjury.mStartDate;
            injuryEndDate = mInjury.mEndDate;
            numWrittenConditions++;
        }
        if (numWrittenConditions < numConditions && mLeagueBan.mEnabled) {
            startConditionFlags |= 2;
            numBanMatches = mLeagueBan.mNumMatches;
            numWrittenConditions++;
        }
        if (numWrittenConditions < numConditions && mRetirement.mEnabled) {
            startConditionFlags |= 4;
            numWrittenConditions++;
        }

        writer.WriteLine(startConditionFlags);
        UInt transferClubID = FifamUtils::GetWriteableID(transferClub);
        writer.WriteLine(FifamUtils::GetCountryIDFromClubID(transferClubID));
        writer.WriteLine(transferClubID);
        writer.WriteLine(transferStartDate);
        writer.WriteLine(transferEndDate);
        writer.WriteLine(numBanMatches);
        if (startConditionFlags & 1) {
            writer.WriteLine(TranslateInjuryTypeForGame(FifamPlayerInjuryType::MakeFromInt(injuryType),
                writer.GetGameId(), injuryEndDate - injuryStartDate));
        }
        else
            writer.WriteLine(0);
        writer.WriteLine(injuryStartDate);
        writer.WriteLine(injuryEndDate);
        if (writer.IsVersionGreaterOrEqual(0x2007, 0x10))
            writer.WriteLine(transferBuyOption);
        if (writer.IsVersionGreaterOrEqual(0x2007, 0x00))
            writer.WriteLine(transferFee);
    }
}

FifamPlayerInjuryType FifamPlayerStartingConditions::GetDefaultInjuryType(Int injuryDays) {
    if (injuryDays <= 120) {
        if (injuryDays > 60)
            return FifamPlayerInjuryType::TornCartilage;
        return FifamPlayerInjuryType::GroinStrain;
    }
    return FifamPlayerInjuryType::RupturedCruciateLigament;
}

FifamPlayerInjuryType FifamPlayerStartingConditions::TranslateInjuryTypeForGame(FifamPlayerInjuryType const &injury, UInt gameId, Int injuryDays) {
    if (gameId <= 10 && injury.ToInt() > 40)
        return GetDefaultInjuryType(injuryDays);
    if (gameId <= 11 && injury.ToInt() > 102)
        return GetDefaultInjuryType(injuryDays);
    return injury;
}
