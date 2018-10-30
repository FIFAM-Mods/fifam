#pragma once
#include "FifamTypes.h"
#include "foom_person.h"

namespace foom {

struct club;
struct nation;

struct non_player : public person {
    Int mJobManager = 0;
    Int mJobAssistantManager = 0;
    Int mJobCoach = 0;
    Int mJobFitnessCoach = 0;
    Int mJobGkCoach = 0;
    Int mJobPhysio = 0;
    Int mJobScout = 0;
    Int mJobDataAnalyst = 0;
    Int mJobSportsScientist = 0;
    Int mJobDirectorOfFootball = 0;
    Int mJobHeadOfYouth = 0;
    Int mJobChairman = 0;
    Int mCurrentAbility = 0;
    Int mPotentialAbility = 0;
    Int mCurrentReputation = 0;
    Int mWorldReputation = 0;
    Int mHomeReputation = 0;
    Int mPreferredFormation = 0;
    Int mSecondPreferredFormation = 0;
    Int mCoachingStyle = 0;
    Int mAttacking = 0;
    Int mDepth = 0;
    Int mDetermination = 0;
    Int mDirectness = 0;
    Int mDirtinessAllowance = 0;
    Int mFlamboyancy = 0;
    Int mFlexibility = 0;
    Int mFreeRoles = 0;
    Int mMarking = 0;
    Int mOffside = 0;
    Int mPressing = 0;
    Int mSittingBack = 0;
    Int mTacticalKnowledge = 0;
    Int mTempo = 0;
    Int mUseOfPlaymaker = 0;
    Int mUseOfSubs = 0;
    Int mWidth = 0;
    Int mBuyingPlayers = 0;
    Int mHardnessOfTraining = 0;
    Int mJudgingPlayerAbility = 0;
    Int mJudgingPlayerPotential = 0;
    Int mJudgingPlayerData = 0;
    Int mJudgingTeamData = 0;
    Int mPresentingData = 0;
    Int mLevelOfDiscipline = 0;
    Int mManManagement = 0;
    Int mMindGames = 0;
    Int mMotivating = 0;
    Int mVersatility = 0;
    Int mSquadRotation = 0;
    Int mWorkingWithYoungsters = 0;
    Int mCoachingAttacking = 0;
    Int mCoachingDefending = 0;
    Int mCoachingFitness = 0;
    Int mCoachingGKDistribution = 0;
    Int mCoachingGKHandling = 0;
    Int mCoachingGKShotStopping = 0;
    Int mCoachingMental = 0;
    Int mCoachingTactical = 0;
    Int mCoachingTechnical = 0;
    Int mBusiness = 0;
    Int mInterference = 0;
    Int mPatience = 0;
    Int mResources = 0;
    Int mPhysiotherapy = 0;
    Int mSportsScience = 0;
    Bool mSignsALotOfYouthPlayers = 0;
    Bool mSignsYoungPlayersForTheFirstTeam = 0;
    Bool mWillMakeEarlyTacticalChanges = 0;
    Bool mWillFitPlayersIntoPreferredTactic = 0;
    Bool mExpectAttackingFootball = 0;
    Bool mExpectYoungSigningsForTheFirstTeam = 0;

    struct club_contract {
        club *mClub = 0;
        Int mJob = 0;
        Int mSecondaryJob = 0;
        Date mDateJoined;
        Date mContractExpires;
        Int mContractType = 0;
        Bool mOnRollingContract = 0;
    };

    club_contract mClubContract;

    struct nation_contract {
        nation *mNation = 0;
        Int mJob = 0;
        Date mDateJoined;
        Date mContractExpires;
    };

    nation_contract mNationContract;
};

}
