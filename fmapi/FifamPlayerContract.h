#pragma once
#include "FifamTypes.h"
#include "FifamReadWrite.h"
#include "FifamPlayerContractSquadRole.h"
#include "FifamClubLink.h"

class FifamDatabase;

struct FifamPlayerContractClauseOption {
    // @since FM10
    Bool mEnabled = false;
    // @since FM12
    // @range 0-150
    UChar mPercent = 0; // % of MV
    // @since FM10
    // @until FM12
    UInt mValue = 0; // money value
};

// @since FM07
class FifamPlayerContract {
public:
    // @since
    // @range 0-34
    UChar mMatchesInternationalBonus = 0; // Appearances International Club Match Bonus - matches count
    // @since
    FifamPlayerContractSquadRole mSquadRole;
    // @since
    // @range 0-100
    UChar mSalaryIncrease = 0; // Salary Improvement per Year (%)
    // @since
    // @range 0-100
    UChar mSalaryDecreaseOnRelegation = 0; // %
    // @since
    FifamPlayerContractClauseOption mClauseForeignClub; // Release Clause for Clubs from Abroad
    // @since
    FifamPlayerContractClauseOption mClauseHigherLeague; // Release Clause for Clubs from Higher Leagues
    // @since
    FifamPlayerContractClauseOption mClauseRelegation; // Release Clause in Case of Relegation
    // @since
    FifamPlayerContractClauseOption mClauseNoInternational; // Release Clause if Club is not Playing International
    // @since
    FifamPlayerContractClauseOption mClauseNoPromotion; // Release Clause in Case of Missing Promotion
    // @since
    // @range 0-34
    UChar mAutoExtend = 0; // Automatic Extension after Matches - matches count
    // @since
    UChar mPromisedRoleInTeam = 0;
    // @since
    UChar mPromisedFuture = 100;
    // @since
    // @range 0-34
    UChar mMatchesAppearanceBonus = 0; // League Matches Appearance Bonus - matches count
    // @since
    // @range 0-100
    UChar mSellOnPercentage = 0; // Participation in Case of Transfer (%)
    // @since
    // @range 0-2
    UChar mOptionClub = 0; // Option Club (in Years)
    // @since
    Bool mHighestPaidPlayer = false; // Best Paid Player
    // @since
    Bool mExtendAvoidRelegation = false; // Contract will Extend in Case of Avoiding Relegation
    // @since
    Bool mJobOption = false; // Job in Club after Active Career
    // @since
    Bool mLoaned = false; // probably unused
    // @since
    // @range 0-2
    UChar mOptionPlayer = 0; // Option Player (in Years)
    // @since
    UInt mSigningFee = 0; // money for player for signing a contract
    // @since
    UInt mBasicSalary = 0; // Basic Salary
    // @since
    UInt mMatchBonus = 0; // General Match Bonus
    // @since
    UInt mAppearanceBonus = 0; // League Match Bonus
    // @since
    UInt mNoRelegationBonus = 0; // Bonus in Case of Avoiding Relegation
    // @since
    UInt mPromotionBonus = 0; // Bonus in Case of Promotion
    // @since
    UInt mInternationalPremiereBonus = 0; // International Team Debut
    // @since
    UInt mGoalsCleanSheetBonus = 0; // Clean Sheet Bonus
    // @since
    UInt mFixTransferFee = 0; // Release Clause
    // @since
    UInt mSellOnFee = 0; // Minumum Fee for Participation in Case of Transfer
    // @since
    UInt mInternationalBonus = 0; // International Club Match Bonus
    // @since
    FifamDate mJoined; // Joined Club
    // @since
    FifamDate mValidUntil; // Contract Valid Until
    // @since
    Bool mBuyBackClauseEnabled = false;
    // @since
    UInt mBuyBackClauseCost = 0;
    // @since
    FifamClubLink mBuyBackClauseClub; // Buy-back Clause
    // @since
    FifamDate mBuyBackClauseValidUntil; // Clause Valid Until

    // @since
    struct {
        // @since
        UChar _1 = 0;
    } Unknown;

    void Read(FifamReader &reader, FifamDatabase *database);
    void Write(FifamWriter &writer, FifamDatabase *database);
};
