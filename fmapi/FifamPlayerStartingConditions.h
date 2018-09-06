#pragma once
#include "FifamTypes.h"
#include "FifamReadWrite.h"
#include "FifamPlayerInjuryType.h"
#include "FifamClubLink.h"

class FifamDatabase;

class FifamPlayerStartingCondition {
public:
    Bool mEnabled = false;

    Bool Enabled();
    void Enable();
    void Disable();
};

class FifamPlayerInjury : public FifamPlayerStartingCondition {
public:
    FifamDate mStartDate;
    FifamDate mEndDate;
    FifamPlayerInjuryType mType;

    void Setup(FifamDate const &startDate, FifamDate const &endDate, FifamPlayerInjuryType const &type);
};

class FifamPlayerLeagueBan : public FifamPlayerStartingCondition {
public:
    UInt mNumMatches = 0;

    void Setup(UInt numMatches);
};

class FifamPlayerRetirement : public FifamPlayerStartingCondition {
public:
    void Setup();
};

class FifamPlayerLoan : public FifamPlayerStartingCondition {
public:
    FifamDate mStartDate;
    FifamDate mEndDate;
    FifamClubLink mLoanedClub;
    Int mBuyOptionValue = 0; // 0 - no buying option, -1 - ingame-generated

    void Setup(FifamDate const &startDate, FifamDate const &endDate, FifamClubLink const &loanedClub, Int buyOptionValue);
};

class FifamPlayerFutureTransfer : public FifamPlayerStartingCondition {
public:
    FifamDate mTransferDate;
    FifamDate mNewContractEndDate;
    FifamClubLink mNewClub;
    UInt mTransferFee = 0;

    void Setup(FifamDate const &transferDate, FifamDate const &newContractEndDate, FifamClubLink const &newClub, UInt transferFee);
};

class FifamPlayerFutureLoan : public FifamPlayerStartingCondition {
public:
    FifamDate mStartDate;
    FifamDate mEndDate;
    FifamClubLink mLoanedClub;
    Int mBuyOptionValue = 0; // 0 - no buying option, -1 - ingame-generated
    UInt mLoanFee = 0;

    void Setup(FifamDate const &startDate, FifamDate const &endDate, FifamClubLink const &loanedClub, Int buyOptionValue, UInt loanFee);
};

class FifamPlayerConditionDate : public FifamPlayerStartingCondition {
public:
    FifamDate mDate;

    void Setup(FifamDate const &date);
};

// @since FM07
class FifamPlayerStartingConditions {
public:
    FifamPlayerInjury mInjury;
    FifamPlayerLeagueBan mLeagueBan;
    FifamPlayerRetirement mRetirement;
    FifamPlayerLoan mLoan;
    FifamPlayerFutureTransfer mFutureTransfer;
    FifamPlayerFutureLoan mFutureLoan;
    FifamPlayerConditionDate mBanUntil;
    FifamPlayerConditionDate mFutureJoin;
    FifamPlayerFutureLoan mFutureReLoan;
    FifamPlayerConditionDate mFutureLeave;

    static FifamPlayerInjuryType GetDefaultInjuryType(Int injuryDays);
    static FifamPlayerInjuryType TranslateInjuryTypeForGame(FifamPlayerInjuryType const &injury, UInt gameId, Int injuryDays);
    UInt GetNumEnabledConditions();
    void Read(FifamReader &reader, FifamDatabase *database);
    void Write(FifamWriter &writer, FifamDatabase *database);
};
