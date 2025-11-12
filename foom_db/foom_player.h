#pragma once
#include "FifamTypes.h"
#include "foom_person.h"
#include "foom_local_region.h"

namespace foom {

struct injury;
struct city;

struct player : public person {
    Int mEthnicity = 0;
    Int mHairColour = 0;
    Int mHairLength = 0;
    Int mSkinTone = 0;
    Int mAgent = 0;
    Bool mDeclaredForNation = 0;
    nation *mDeclaredForNationYouth = 0;
    Int mNationalityInfo = 0;
    Bool mInternationalRetirement = 0;
    Date mInternationalRetirementDate;
    Int mInternationalApps = 0;
    Int mInternationalGoals = 0;
    String mShirtName;
    Int mCurrentAbility = 0;
    Int mRecommendedCurrentAbility = 0;
    Int mPotentialAbility = 0;
    Int mPerceivedPotentialAbility = 0;
    Int mPeakCA = 0;
    Int mPeakPA = 0;
    Int mOriginalCA = 0;
    Int mOriginalPA = 0;
    Int mCurrentReputation = 0;
    Int mHomeReputation = 0;
    Int mWorldReputation = 0;
    Int mHeight = 0;
    Int mBodyType = 0;
    Int mLeftFoot = 0;
    Int mRightFoot = 0;
    Int mPosition = 0;
    Int mGoalkeeper = 0;
    Int mDefenderLeft = 0;
    Int mDefenderCentral = 0;
    Int mDefenderRight = 0;
    Int mDefensiveMidfielder = 0;
    Int mWingBackLeft = 0;
    Int mWingBackRight = 0;
    Int mMidfielderLeft = 0;
    Int mMidfielderCentral = 0;
    Int mMidfielderRight = 0;
    Int mAttackingMidfielderLeft = 0;
    Int mAttackingMidfielderCentral = 0;
    Int mAttackingMidfielderRight = 0;
    Int mStriker = 0;
    Int mAggression = 0;
    Int mAnticipation = 0;
    Int mBravery = 0;
    Int mComposure = 0;
    Int mConcentration = 0;
    Int mConsistency = 0;
    Int mVision = 0;
    Int mDecisions = 0;
    Int mDetermination = 0;
    Int mDirtiness = 0;
    Int mFlair = 0;
    Int mImportantMatches = 0;
    Int mLeadership = 0;
    Int mMovement = 0;
    Int mPositioning = 0;
    Int mTeamWork = 0;
    Int mWorkRate = 0;
    Int mAcceleration = 0;
    Int mAgility = 0;
    Int mBalance = 0;
    Int mInjuryProneness = 0;
    Int mJumpingReach = 0;
    Int mNaturalFitness = 0;
    Int mPace = 0;
    Int mStamina = 0;
    Int mStrength = 0;
    Int mCorners = 0;
    Int mCrossing = 0;
    Int mDribbling = 0;
    Int mFinishing = 0;
    Int mFirstTouch = 0;
    Int mFreeKicks = 0;
    Int mHeading = 0;
    Int mLongShots = 0;
    Int mLongThrows = 0;
    Int mMarking = 0;
    Int mPassing = 0;
    Int mPenaltyTaking = 0;
    Int mTackling = 0;
    Int mTechnique = 0;
    Int mVersatility = 0;
    Int mAerialAbility = 0;
    Int mCommandOfArea = 0;
    Int mCommunication = 0;
    Int mEccentricity = 0;
    Int mHandling = 0;
    Int mKicking = 0;
    Int mOneOnOnes = 0;
    Int mReflexes = 0;
    Int mRushingOut = 0;
    Int mTendencyToPunch = 0;
    Int mThrowing = 0;
    Bool mRunsWithBallDownLeft = 0;
    Bool mRunsWithBallDownRight = 0;
    Bool mRunsWithBallThroughTheCentre = 0;
    Bool mGetsIntoOppositionArea = 0;
    Bool mMovesIntoChannels = 0;
    Bool mGetsForwardWheneverPossible = 0;
    Bool mPlaysShortSimplePasses = 0;
    Bool mTriesKillerBallsOften = 0;
    Bool mShootsFromDistance = 0;
    Bool mShootsWithPower = 0;
    Bool mPlacesShots = 0;
    Bool mCurlsBall = 0;
    Bool mLikesToRoundKeeper = 0;
    Bool mLikesToTryToBreakOffsideTrap = 0;
    Bool mArguesWithOfficials = 0;
    Bool mLikesToLobKeeper = 0;
    Bool mPlaysNoThroughBalls = 0;
    Bool mDwellsOnBall = 0;
    Bool mArrivesLateInOppositionArea = 0;
    Bool mTriesToPlayWayOutOfTrouble = 0;
    Bool mStaysBackAtAllTimes = 0;
    Bool mDivesIntoTackles = 0;
    Bool mDoesNotDiveIntoTackles = 0;
    Bool mHitsFreekicksWithPower = 0;
    Bool mRunsWithBallOften = 0;
    Bool mRunsWithBallRarely = 0;
    Bool mAvoidsUsingWeakerFoot = 0;
    Bool mTriesLongRangeFreeKicks = 0;
    Bool mCutsInsideFromBothWings = 0;
    Bool mPlaysOneTwos = 0;
    Bool mDictatesTempo = 0;
    Bool mAttemptsOverheadKicks = 0;
    Bool mKnocksBallPastOpponent = 0;
    Bool mTriesLongRangePasses = 0;
    Bool mLikesToSwitchBallToOtherFlank = 0;
    Bool mComesDeepToGetBall = 0;
    Bool mHugsLine = 0;
    Bool mLooksForPassRatherThanAttemptingToScore = 0;
    Bool mMarksOpponentTightly = 0;
    Bool mPlaysWithBackToGoal = 0;
    Bool mPossessesLongFlatThrow = 0;
    Bool mStopsPlay = 0;
    Bool mTriesFirstTimeShots = 0;
    Bool mUsesLongThrowToStartCounterAttacks = 0;
    Bool mRefrainsFromTakingLongShots = 0;
    Bool mPenaltyBoxPlayer = 0;
    Bool mCutsInsideFromLeftWing = 0;
    Bool mCutsInsideFromRightWing = 0;
    Bool mCrossesEarly = 0;
    Bool mBringBallOutofDefence = 0;
    Bool mEarliestDOB = 0;
    Bool mWearsHijab = 0;
    Bool mSlowRetirement = 0;
    nation *mNationOfBirth = 0;
    local_region *mRegionOfBirth = 0;
    city *mCityOfBirth = 0;

    struct contract {
        club *mClub = 0;
        Int mJob = 0;
        Int mSecondaryJob = 0;
        Date mDateJoined;
        Date mContractExpires;
        Int mContractType = 0;
        Int mWage = 0;
        Bool mOnRollingContract = 0;
        Int mSquadStatus = 0;
        Int mSquadNumber = 0;
        Int mPreferredSquadNumber = 0;
        Int mAppearanceFee = 0;
        Int mGoalBonus = 0;
        Int mCleanSheetBonus = 0;
        Int mInternationalCapBonus = 0;
        Int mYearlyWageRise = 0;
        Int mPromotionWageRise = 0;
        Int mRelegationWageDrop = 0;
        Int mOneYearExtensionAfterLeagueGamesFinalSeason = 0;
        Int mOptionalContractExtensionByClub = 0;
        Bool mMatchHighestEarnerClause = 0;
        Bool mWillLeaveAtEndOfContract = 0;
        Int mMinimumFeeReleaseClause = 0;
        Date mMinimumFeeReleaseClauseExpiryDate;
        Int mSellOnFeePercentage = 0;
    } mContract;

    struct loan {
        club *mClub = 0;
        Date mStartDate;
        Date mEndDate;
        Int mSquadNumber = 0;
        Int mWage = 0;
        Int mMonthlyFee = 0;
        Int mFeeToBuy = 0;
    } mLoan;

    struct future_transfer {
        club *mClub = 0;
        Date mTransferDate;
        Date mContractEndDate;
        Int mTransferFee = 0;
        Int mNewJob = 0;
    } mFutureTransfer;

    struct playing_history {
        Int mYear = 0;
        Int mOrder = 0;
        club *mClub = 0;
        comp *mDivision = 0;
        Date mDateJoined;
        Date mDateLeft;
        Bool mOnLoan = 0;
        Bool mYouthTeam = 0;
        Int mApps = 0;
        Int mGoals = 0;
        Int mTransferFee = 0;
    };

    Vector<playing_history> mVecPlayingHistory;

    struct favourite_people {
        person *mPerson = 0;
        Int mLevel = 0;
        Int mReason = 0;
        Bool mPermanent = 0;
    };

    Vector<favourite_people> mVecFavouritePeople;

    struct ban {
        Int mBanType = 0;
        Date mStartDate;
        Date mEndDate;
        Int mNumberMatches = 0;
    };

    Vector<ban> mVecBans;

    struct retirement {
        Int mType = 0;
        Date mDate;
    };

    Vector<retirement> mVecRetirements;

    struct injury {
        foom::injury *mInjury = 0;
        Date mStartDate;
        Date mEndDate;
        Bool mFuture = 0;
        Bool mPermanent = 0;
        Int mSeverity = 0;
    };

    Vector<injury> mVecInjuries;

    player() {
        mIsPlayer = true;
    }

    Bool IsBasque() {
        return mRegionOfBirth &&
            (mRegionOfBirth->mID == 67249095 ||
                mRegionOfBirth->mID == 67023591 ||
                mRegionOfBirth->mID == 34015446 ||
                mRegionOfBirth->mID == 67023592);
    }
};

}
