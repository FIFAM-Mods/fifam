#pragma once
#include "FifamPerson.h"
#include "FifamTypes.h"
#include "FifamDate.h"
#include "FifamLanguage.h"
#include "FifamReadWrite.h"
#include "FifamPlayerAppearance.h"
#include "FifamNation.h"
#include "FifamPersonType.h"
#include "FifamPlayerAttributes.h"
#include "FifamPlayerHistory.h"
#include "FifamPlayerContract.h"
#include "FifamPlayerStartingConditions.h"
#include "FifamClubLink.h"
#include "FifamPlayerPosition.h"
#include "FifamPlayerPlayingStyle.h"
#include "FifamShoeType.h"
#include "FifamPlayerAgent.h"
#include "FifamFormation.h"
#include "FifamChairmanStability.h"
#include "FifamPlayerCharacter.h"
#include "FifamFlags.h"

class FifamClub;

// @since FM07
class FifamPlayer : public FifamPerson {
public:
    // @since FM07
    UInt mEmpicsId = 0;
    // @since FM07
    // @maxsize 19
    String mLastName;
    // @since FM07
    // @maxsize 15
    String mFirstName;
    // @since FM07
    // @maxsize 19 FM07
    // @maxsize 29 FM13
    String mPseudonym;
    // @since FM07
    // @maxsize 19
    String mNickname;
    // @since FM07
    Array<FifamNation, 2> mNationality;
    // @since FM07
    Array<FifamLanguage, 4> mLanguages;
    // @since FM07
    FifamDate mBirthday;
    // @since FM07
    // there are two boolean flags, `IsReserve` and `IsYouth`
    // if none of them enabled then player is in the first team
    // both flags can be enabled in FM07, can't be enabled since FM08
    Bool mInReserveTeam = false;
    // @since FM07
    Bool mInYouthTeam = false;
    // @since FM07
    // called as `Does not Count as a Foreign Player by Individual Decision of Local FA` in FM07
    Bool mIsNaturalised = false;
    // @since FM07
    Bool mIsBasque = false; 
    // @since FM07
    Bool mIsRealPlayer = false;
    // @since FM07
    // @range [0;9]
    UChar mTalent = 0;
    // @since FM07
    // @range 0-7
    UChar mTacticalEducation = 0;
    // @since FM07
    // @until FM08
    // @range [0;99]
    UChar mPotential = 0;
    // @since FM07
    // @range [0;4]
    UChar mLeftFoot = 0;
    // @since FM07
    // @range [0;4]
    UChar mRightFoot = 0;
    // @since FM07
    // @range [0;10]
    UChar mHeroStatus = 0;
    // @since FM07
    // @range [0;99]
    UChar mShirtNumberFirstTeam = 0;
    // @since FM07
    // @range [0;99]
    UChar mShirtNumberReserveTeam = 0;
    // @since FM07
    // @until FM11
    // @range 0-6
    UChar mNationalExperience = 0;
    // @since FM07
    // @until FM11
    // @range 0-4
    UChar mInternationalExperience = 0;
    // @since FM11
    // @range 0-18
    UChar mGeneralExperience = 0;
    // @since FM07
    FifamPlayerPosition mMainPosition;
    // @since FM11
    // @range 0-100
    Array<Float, 18> mPositionBias =  {};
    // @since FM11
    Float mMaxBias = 100.0f;
    // @since FM07
    FifamPlayerPlayingStyle mPlayingStyle;
    // @since FM07
    FifamPlayerAttributes mAttributes;
    // @since FM07
    FifamFlags<FifamPlayerCharacter> mCharacter;
    // @since FM07
    FifamPlayerAppearance mAppearance;
    // @since FM07
    UInt mSpecialFace = 0;
    // @since FM07
    UChar mHeight = 178;
    // @since FM07
    UChar mWeight = 70;
    // @since FM07
    FifamShoeType mShoeType;
    // @since FM07
    UChar mLongSleeves = 0;
    // @since FM07
    // @until FM08
    FifamClubLink mFirstClub;
    // @since FM07
    // @until FM08
    FifamClubLink mPreviousClub;
    // @since FM07
    UChar mNationalTeamMatches = 0;
    // @since FM07
    UChar mNationalTeamGoals = 0;
    // @since FM07
    Bool mCurrentlyInNationalTeam = false;
    // @since FM07
    Bool mRetiredFromNationalTeam = false;
    // @since FM13
    Bool mNoContractExtension = false; // TODO: Check if it works in FM12
    // @since FM10
    // @until FM12
    Bool mContractIsCancelledIfRelegated = false;
    // @since FM13
    Bool mIsCaptain = false; // TODO: Check if it works in FM12
    // @since FM07
    FifamPlayerStartingConditions mStartingConditions;
    // @since FM07
    FifamPlayerHistory mHistory;
    // @since 12
    FifamPlayerContract mContract;
    // @since FM07
    UInt mCurrentEstimatedMarketValue = 0;
    // @since FM11
    FifamPlayerAgent mPlayerAgent;
    // @since FM07
    FifamClubLink mFavouriteClub;
    // @since FM07
    FifamClubLink mWouldnSignFor;
    // @since FM07
    FifamPlayer *mManagerFavouritePlayer = nullptr;
    // @since FM13
    Array<FifamClubLink, 3> mTransferRumors;
    // @since FM07
    // @maxsize 64 FM13
    String mComment;

    // Employee data

    // @since FM07
    // @range 0-15
    UChar mManagerMotivationSkills = 0;
    // @since FM07
    // @range 0-15
    UChar mManagerCoachingSkills = 0;
    // @since FM07
    // @range 0-15
    UChar mManagerGoalkeepersTraining = 0;
    // @since FM07
    // @range 0-15
    UChar mManagerNegotiationSkills = 0;
    // @since FM07
    FifamFormation mManagerFavouriteFormation;
    // @since FM07
    FifamChairmanStability mChairmanStability;

    // Unknown data
    struct {
        UChar _1 = 0;
    } Unknown;

    FifamClub *mClub = nullptr;

    FifamPlayer();
    String GetName() const;
    void Read(FifamReader &reader, FifamDatabase *database);
    void Write(FifamWriter &writer, FifamDatabase *database);
    UChar GetLevel(FifamPlayerPosition position, FifamPlayerPlayingStyle style, Bool experience = true);
    UChar GetLevel(FifamPlayerPosition position, Bool experience = true);
    UChar GetLevel(FifamPlayerPlayingStyle style, Bool experience = true);
    UChar GetLevel(Bool experience = true);
    static Bool SortPlayersByLevel(FifamPlayer *player1, FifamPlayer *player2);
};
