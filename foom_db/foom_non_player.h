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
    Int mJobTechnicalDirector = 0;
    Int mJobHeadOfPlayerDevelopment = 0;
    Int mCurrentAbility = 0;
    Int mPotentialAbility = 0;
    Int mOriginalCA = 0;
    Int mOriginalPA = 0;
    Int mCurrentReputation = 0;
    Int mWorldReputation = 0;
    Int mHomeReputation = 0;
    Int mPreferredFormation = 0;
    Int mSecondPreferredFormation = 0;
    Int mCoachingStyle = 0;
    Int mAttacking = 0;
    Int mDetermination = 0;
    Int mTacticalKnowledge = 0;
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
    Int mNegotiations = 0;
    Int mInterference = 0;
    Int mPatience = 0;
    Int mResources = 0;
    Int mPhysiotherapy = 0;
    Int mSportsScience = 0;
    Int mSignsALotOfYouthPlayers = 0;
    Int mSignsYoungPlayersForTheFirstTeam = 0;
    Int mWillMakeEarlyTacticalChanges = 0;
    Int mWillFitPlayersIntoPreferredTactic = 0;
    Int mWillLookToPlayOutOfDefence = 0;

    struct club_contract {
        club *mClub = 0;
        Int mJob = 0;
        Int mSecondaryJob = 0;
        Date mDateJoined;
        Date mContractExpires;
        Int mContractType = 0;
        Bool mOnRollingContract = 0;
        Int mSquadStatus = 0;
    };

    club_contract mClubContract;

    struct nation_contract {
        nation *mNation = 0;
        Int mJob = 0;
        Date mDateJoined;
        Date mContractExpires;
    };

    nation_contract mNationContract;

    struct days_at_club_or_nation {
        team *mClubOrNation = 0;
        Int mDays = 0;
    };

    Vector<days_at_club_or_nation> mDaysAtClubOrNation;
};

}
