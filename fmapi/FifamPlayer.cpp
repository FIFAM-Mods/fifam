#include "FifamPlayer.h"
#include "FifamPlayerLevel.h"
#include "FifamUtils.h"
#include "Random.h"

FifamPlayer::FifamPlayer() {
    mPersonType = FifamPersonType::Player;
}

String FifamPlayer::GetName() const {
    if (!mPseudonym.empty())
        return mPseudonym;
    if (!mFirstName.empty())
        return mFirstName + L" " + mLastName;
    return mLastName;
}

void FifamPlayer::Read(FifamReader &reader, FifamDatabase *database) {
    if (reader.ReadStartIndex(L"PLAYER")) {
        if (reader.IsVersionGreaterOrEqual(0x2012, 0x01)) {
            reader.ReadLine(mPersonType);
            Array<String, 4> namesArray;
            reader.ReadLineArray(namesArray, '|');
            mFirstName = namesArray[0];
            mLastName = namesArray[1];
            mNickname = namesArray[2];
            mPseudonym = namesArray[3];
            reader.ReadLineArray(mNationality);
            reader.ReadLineArray(mLanguages);
            reader.ReadLine(mBirthday);
            if (reader.IsVersionGreaterOrEqual(0x2013, 0x04)) {
                UChar leadership = 0;
                reader.ReadLine(mTalent, mTacticalEducation, leadership, mGeneralExperience, mMainPosition);
                // convert leadership from [1;10] to [0;99]
                UChar leadershipTable[10] = { 20, 30, 40, 50, 60, 70, 75, 80, 86, 95 };
                if (leadership < 1)
                    mAttributes.Leadership = 10;
                else if (leadership > 10)
                    mAttributes.Leadership = 99;
                else
                    mAttributes.Leadership = leadershipTable[leadership - 1];
                //
            }
            else {
                reader.ReadLine(mTalent, mTacticalEducation, mGeneralExperience, mMainPosition);
                if (reader.IsVersionGreaterOrEqual(0x2013, 0x01))
                    mAttributes.Leadership = 50;
            }
            if (reader.IsVersionGreaterOrEqual(0x2013, 0x01)) {
                Array<UChar, 14> positionBias;
                reader.ReadLineArray(positionBias);
                mPositionBias = FifamPlayerLevel::GetDefaultBiasValues(mMainPosition);
                mPositionBias[FifamPlayerPosition::None] = positionBias[0];
                mPositionBias[FifamPlayerPosition::GK]   = positionBias[1];
                mPositionBias[FifamPlayerPosition::RB]   = positionBias[2];
                mPositionBias[FifamPlayerPosition::LB]   = positionBias[3];
                mPositionBias[FifamPlayerPosition::CB]   = positionBias[4];
                mPositionBias[FifamPlayerPosition::DM]   = positionBias[5];
                mPositionBias[FifamPlayerPosition::RM]   = positionBias[6];
                mPositionBias[FifamPlayerPosition::LM]   = positionBias[7];
                mPositionBias[FifamPlayerPosition::CM]   = positionBias[8];
                mPositionBias[FifamPlayerPosition::RW]   = positionBias[9];
                mPositionBias[FifamPlayerPosition::LW]   = positionBias[10];
                mPositionBias[FifamPlayerPosition::AM]   = positionBias[11];
                mPositionBias[FifamPlayerPosition::CF]   = positionBias[12];
                mPositionBias[FifamPlayerPosition::ST]   = positionBias[13];
            }
            else {
                Array<UChar, 18> positionBias;
                reader.ReadLineArray(positionBias);
                for (UInt i = 0; i < 18; i++)
                    mPositionBias[i] = positionBias[i];
            }
            mAttributes.Read(reader);
            reader.ReadLine(mInReserveTeam);
            UInt flags = reader.ReadLine<UInt>();
            mIsNaturalised = Utils::CheckFlag(flags, 0x1);
            mIsBasque = Utils::CheckFlag(flags, 0x2);
            mIsRealPlayer = Utils::CheckFlag(flags, 0x4);
            mInYouthTeam = Utils::CheckFlag(flags, 0x8);
            mRetiredFromNationalTeam = Utils::CheckFlag(flags, 0x10);
            mCurrentlyInNationalTeam = Utils::CheckFlag(flags, 0x20);
            mNoContractExtension = Utils::CheckFlag(flags, 0x40);
            mIsCaptain = Utils::CheckFlag(flags, 0x80);
            UChar footPrefs = reader.ReadLine<UChar>();
            mRightFoot = footPrefs & 0xF;
            mLeftFoot = (footPrefs >> 4) & 0xF;
            reader.ReadLine(mHeroStatus);
            reader.ReadLine(mPlayingStyle);
            if (reader.IsVersionGreaterOrEqual(0x2013, 0x03)) {
                reader.ReadLine(mCharacter);
                if (mCharacter.Check(FifamPlayerCharacter::AttitudeToWin))
                    mCharacter.Set(FifamPlayerCharacter::Diva, true);
                if (mCharacter.Check(FifamPlayerCharacter::Complacent))
                    mCharacter.Set(FifamPlayerCharacter::LifestyleIcon, true);
                mCharacter.Set(FifamPlayerCharacter::AttitudeToWin, false);
                mCharacter.Set(FifamPlayerCharacter::Complacent, false);
            }
            else
                reader.ReadLine(mCharacter);
            reader.ReadLine(mAppearance.mGenericFace,
                mAppearance.mHairStyle,
                mAppearance.mHairColor,
                mAppearance.mBeardType,
                mAppearance.mSideburns,
                mAppearance.mBeardColor,
                mAppearance.mSkinColor,
                mAppearance.mFaceVariation,
                mAppearance.mEyeColour,
                mShoeType,
                mLongSleeves);
            reader.ReadLine(mSpecialFace, mEmpicsId, mHeight, mWeight, mShirtNumberFirstTeam, mShirtNumberReserveTeam);
            reader.ReadLine(mPlayerAgent);
            reader.ReadLine(mNationalTeamMatches, mNationalTeamGoals);
            FifamUtils::SaveClubIDToClubLink(mFavouriteClub, reader.ReadLine<UInt>());
            FifamUtils::SaveClubIDToClubLink(mWouldnSignFor, reader.ReadLine<UInt>());
            if (reader.IsVersionGreaterOrEqual(0x2013, 0x02)) {
                FifamUtils::SaveClubIDToClubLink(mTransferRumors[0], reader.ReadLine<UInt>());
                FifamUtils::SaveClubIDToClubLink(mTransferRumors[1], reader.ReadLine<UInt>());
                FifamUtils::SaveClubIDToClubLink(mTransferRumors[2], reader.ReadLine<UInt>());
            }
            mStartingConditions.Read(reader, database);
            mHistory.Read(reader, database);
            mContract.Read(reader, database);
            reader.ReadLine(mManagerMotivationSkills, mManagerCoachingSkills, mManagerGoalkeepersTraining, mManagerNegotiationSkills);
            reader.ReadLine(mManagerFavouriteFormation);
            reader.ReadLine(mChairmanStability);
            reader.ReadFullLine(mComment);
        }
        else {
            reader.ReadFullLine(mFirstName);
            reader.ReadFullLine(mLastName);
            reader.ReadFullLine(mNickname);
            reader.ReadFullLine(mPseudonym);
            reader.ReadLine(mPersonType);
            reader.ReadLine(mInReserveTeam);
            reader.ReadLine(mNationality[0]);
            reader.ReadLine(mNationality[1]);
            UChar playerBasicFlags = reader.ReadLine<UChar>();
            if (playerBasicFlags & 1)
                mIsNaturalised = true;
            if (playerBasicFlags & 2)
                mIsBasque = true;
            if (playerBasicFlags & 4)
                mIsRealPlayer = true;
            reader.ReadLine(mBirthday);
            if (reader.IsVersionGreaterOrEqual(0x2009, 0x02)) {
                reader.ReadLine(mTalent);
                UChar footPrefs = reader.ReadLine<UChar>();
                mRightFoot = footPrefs & 0xF;
                mLeftFoot = (footPrefs >> 4) & 0xF;
            }
            else {
                UChar flags = reader.ReadLine<UChar>();
                UChar footPrefs = (flags >> 3) & 7;
                mTalent = (flags & 7) * 2 + 1;
                mInYouthTeam = Utils::CheckFlag(flags, 0x40);
                switch (footPrefs) {
                case 0:
                    mRightFoot = 4;
                    mLeftFoot = 0;
                    break;
                case 1:
                    mRightFoot = 0;
                    mLeftFoot = 4;
                    break;
                case 2:
                    mRightFoot = 4;
                    mLeftFoot = 2;
                    break;
                case 3:
                    mRightFoot = 2;
                    mLeftFoot = 4;
                    break;
                default:
                    break;
                }
            }
            reader.ReadLine(mHeroStatus);
            if (reader.IsVersionGreaterOrEqual(0x2011, 0x04)) {
                for (UInt i = 0; i < 18; i++)
                    reader.ReadLine(mPositionBias[i]);
                reader.ReadLine(mMainPosition);
            }
            else {
                Array<FifamPlayerPosition, 3> PreferredPositions;
                Array<FifamPlayerPosition, 3> SecondaryPositions;
                for (UInt i = 0; i < 3; i++)
                    reader.ReadLine(PreferredPositions[i]);
                for (UInt i = 0; i < 3; i++)
                    reader.ReadLine(SecondaryPositions[i]);
                mMainPosition = PreferredPositions[0];
                mPositionBias = FifamPlayerLevel::GetDefaultBiasValues(mMainPosition);
                for (UInt i = 0; i < 3; i++) {
                    if (PreferredPositions[i] != FifamPlayerPosition::None)
                        mPositionBias[PreferredPositions[i].ToInt()] = (Float)(100 - i);
                }
                for (UInt i = 0; i < 3; i++) {
                    if (SecondaryPositions[i] != FifamPlayerPosition::None)
                        mPositionBias[SecondaryPositions[i].ToInt()] = Float(Utils::Clamp(i - 97, 96, 100));
                }
            }
            mAttributes.Read(reader);
            reader.ReadLine(mPlayingStyle);
            if (!reader.IsVersionGreaterOrEqual(0x2011, 0x04))
                reader.ReadLine(mPotential);
            reader.ReadLine(mCharacter);
            FifamPlayerAppearance::AppearanceInfo07 info07;
            reader.ReadLine(info07.hairColor);
            reader.ReadLine(info07.faceId);
            reader.ReadLine(info07.hairId);
            reader.ReadLine(info07.beardType);
            mAppearance.SetFrom07AppearanceInfo(info07);
            reader.ReadLine(mHeight);
            reader.ReadLine(mWeight);
            if (reader.IsVersionGreaterOrEqual(0x2011, 0x04))
                reader.ReadLine(mGeneralExperience);
            else {
                reader.ReadLine(mNationalExperience);
                reader.ReadLine(mInternationalExperience);
            }
            reader.ReadLine(mShirtNumberFirstTeam);
            reader.ReadLine(mShirtNumberReserveTeam);
            reader.ReadLine(mSpecialFace);
            reader.ReadLine(mEmpicsId);
            if (!reader.IsVersionGreaterOrEqual(0x2007, 0x16)) {
                reader.SkipLine();
                FifamUtils::SaveClubIDToClubLink(mFirstClub, reader.ReadLine<UInt>());
                reader.SkipLine();
                FifamUtils::SaveClubIDToClubLink(mPreviousClub, reader.ReadLine<UInt>());
            }
            mContract.mJoined.Set(1, 7, reader.ReadLine<UShort>());
            mContract.mValidUntil.Set(30, 6, reader.ReadLine<UShort>());
            reader.ReadLine(mContract.mBasicSalary);
            reader.ReadLine(mContract.mFixTransferFee);
            reader.ReadLine(mCurrentEstimatedMarketValue);
            reader.ReadLine(mNationalTeamMatches);
            reader.ReadLine(mNationalTeamGoals);
            reader.ReadLine(mManagerMotivationSkills);
            reader.ReadLine(mManagerCoachingSkills);
            reader.ReadLine(mManagerGoalkeepersTraining);
            reader.ReadLine(mManagerNegotiationSkills);
            reader.ReadLine(mLanguages[0]);
            reader.ReadLine(mLanguages[1]);
            reader.ReadLine(mLanguages[2]);
            reader.ReadLine(mLanguages[3]);
            
            if (reader.IsVersionGreaterOrEqual(0x2009, 0x0A)) {
                FifamUtils::SaveClubIDToClubLink(mFavouriteClub, reader.ReadLine<UInt>());
                FifamUtils::SaveClubIDToClubLink(mWouldnSignFor, reader.ReadLine<UInt>());
                reader.SkipLine();
            }
            else {
                reader.SkipLine();
                FifamUtils::SaveClubIDToClubLink(mFavouriteClub, reader.ReadLine<UInt>());
                if (reader.IsVersionGreaterOrEqual(0x2007, 0x0E))
                    FifamUtils::SaveClubIDToClubLink(mWouldnSignFor, reader.ReadLine<UInt>());
                reader.SkipLines(2);
            }
            FifamUtils::SavePlayerIDToPlayerPtr(mManagerFavouritePlayer, reader.ReadLine<UInt>());
            reader.ReadLine(mManagerFavouriteFormation);
            reader.ReadLine(mChairmanStability);
            reader.ReadLine(mTacticalEducation);
            UInt contractFlags = reader.ReadLine<UInt>();
            mRetiredFromNationalTeam = Utils::CheckFlag(contractFlags, 0x1);
            mCurrentlyInNationalTeam = Utils::CheckFlag(contractFlags, 0x2);
            mContract.mOptionClub = 0;
            mContract.mOptionPlayer = 0;
            if (reader.IsVersionGreaterOrEqual(0x2009, 0x0A)) {
                mContract.mExtendAvoidRelegation = Utils::CheckFlag(contractFlags, 0x100);
                mContract.mClauseNoInternational.mEnabled = Utils::CheckFlag(contractFlags, 0x400);
                mContractIsCancelledIfRelegated = Utils::CheckFlag(contractFlags, 0x800);
                mContract.mClauseRelegation.mEnabled = Utils::CheckFlag(contractFlags, 0x1000);
                mContract.mClauseNoPromotion.mEnabled = Utils::CheckFlag(contractFlags, 0x2000);
            }
            if (mContract.mOptionClub == 0 && Utils::CheckFlag(contractFlags, 0x40))
                mContract.mOptionClub = 1;
            if (mContract.mOptionPlayer == 0 && Utils::CheckFlag(contractFlags, 0x80))
                mContract.mOptionPlayer = 1;
            if (reader.IsVersionGreaterOrEqual(0x2009, 0x07)) {
                UChar optionPlayer = reader.ReadLine<UChar>();
                if (Utils::CheckFlag(contractFlags, 0x8000)) {
                    mContract.mOptionPlayer = optionPlayer;
                    if (mContract.mOptionPlayer == 0)
                        mContract.mOptionPlayer = 2;
                }
                UChar optionClub = reader.ReadLine<UChar>();
                if (Utils::CheckFlag(contractFlags, 0x200)) {
                    mContract.mOptionClub = optionClub;
                    if (mContract.mOptionClub == 0)
                        mContract.mOptionClub = 2;
                }
                UChar autoExtendAfterMatches = reader.ReadLine<UChar>();
                if (Utils::CheckFlag(contractFlags, 0x4000))
                    mContract.mAutoExtend = autoExtendAfterMatches;
                mContract.mClauseNoInternational.mValue = reader.ReadLine<UInt>();
                mContract.mClauseRelegation.mValue = reader.ReadLine<UInt>();
                mContract.mClauseNoPromotion.mValue = reader.ReadLine<UInt>();
            }
            mStartingConditions.Read(reader, database);
            reader.ReadLine(mShoeType);
            reader.ReadLine(mLongSleeves);
            mHistory.Read(reader, database);
            reader.ReadLine(mComment);
            if (reader.IsVersionGreaterOrEqual(0x2011, 0x01))
                reader.ReadLine(mPlayerAgent);
            else
                reader.ReadLine(Unknown._1);

            //if (playerBasicFlags >= 8)
            //    Error("playerBasicFlags: unknown value (%u)", playerBasicFlags);
        }
        reader.ReadEndIndex(L"PLAYER");

        if (reader.IsVersionGreaterOrEqual(0x2013, 0x01)) {
            mAttributes.TacklingGeneral = (mAttributes.TackleSliding + mAttributes.TackleStanding) / 2;
            mAttributes.Technique = (mAttributes.BallControl + mAttributes.Dribbling) / 2;
            mAttributes.Creativity = mAttributes.Vision;
            Array<UChar, 4> flairAttributes;
            flairAttributes[0] = mAttributes.Dribbling;
            flairAttributes[1] = mAttributes.Technique;
            flairAttributes[2] = Utils::Max(mAttributes.TackleSliding, mAttributes.TackleStanding);
            flairAttributes[3] = mAttributes.Passing;
            std::sort(flairAttributes.begin(), flairAttributes.end());
            mAttributes.Flair = (mAttributes.Vision + (flairAttributes[3] + flairAttributes[4]) / 2) / 2;
            mAttributes.Touch = (mAttributes.BallControl + mAttributes.Technique) / 2;
            mAttributes.ForwardRuns = (mAttributes.PosOffensive + (mAttributes.Pace + mAttributes.Acceleration) / 2) / 2;

            mAttributes.Anticipation = (UChar)(Utils::Max(mAttributes.PosDefensive, mAttributes.PosOffensive) * 0.6f +
                mAttributes.Vision * 0.4f);
            mAttributes.Concentration = mAttributes.Composure;
            mAttributes.Decision = (mAttributes.Composure + mAttributes.Consistency) / 2;
            mAttributes.Determination = (mAttributes.Composure + mAttributes.Consistency) / 2;
            mAttributes.Intimidation = mAttributes.Aggression;
            mAttributes.TeamWork = (UChar)(mAttributes.Anticipation * 0.8f + mAttributes.Stamina * 0.2f);
            if (mMainPosition == FifamPlayerPosition::GK)
                mAttributes.TeamWork /= 2;
            mAttributes.WorkRate = (UChar)(mAttributes.Stamina * 0.8f + mAttributes.TeamWork * 0.2f);

            mAttributes.GkCrosses = (mAttributes.Positioning + mAttributes.Handling) / 2;
            mAttributes.ShotStopping = mAttributes.Reflexes;
            mAttributes.Punching = (UChar)(mAttributes.Strength * 0.15f + mAttributes.ShotStopping * 0.85f);
            if (mMainPosition == FifamPlayerPosition::GK)
                mAttributes.Throwing = (mAttributes.Kicking + mAttributes.Handling) / 2;
            else
                mAttributes.Throwing = 25;
            if (mAttributes.Throwing < 90 && mCharacter.Check(FifamPlayerCharacter::LongThrowOuts))
                mAttributes.Throwing = 90;
        }
        else {
            // re-calculate bias values
            mMaxBias = *std::max_element(mPositionBias.begin(), mPositionBias.end());
            if (mMaxBias != 100.0f && mMaxBias != 0.0f) {
                Float mp = 100.0f / mMaxBias;
                for (UInt i = 0; i < 18; i++)
                    mPositionBias[i] *= mp;
            }
            // generate all other attributes
            mAttributes.BallControl = (mAttributes.Technique + mAttributes.Touch) / 2;
            mAttributes.Volleys = (mAttributes.Technique + mAttributes.ShotPower) / 2;
            mAttributes.TackleStanding = (mAttributes.TacklingGeneral + mAttributes.ManMarking) / 2;
            Float slidingTackleCoeff = 1.0f;
            switch (mMainPosition.ToInt()) {
            case FifamPlayerPosition::RB:
            case FifamPlayerPosition::LB:
            case FifamPlayerPosition::CB:
            case FifamPlayerPosition::SW:
            case FifamPlayerPosition::RWB:
            case FifamPlayerPosition::LWB:
            case FifamPlayerPosition::ANC:
            case FifamPlayerPosition::DM:
                slidingTackleCoeff = 2.0f;
                break;
            case FifamPlayerPosition::CM:
            case FifamPlayerPosition::LM:
            case FifamPlayerPosition::RM:
                slidingTackleCoeff = 4.0f;
                break;
            case FifamPlayerPosition::RW:
            case FifamPlayerPosition::LW:
            case FifamPlayerPosition::AM:
            case FifamPlayerPosition::CF:
            case FifamPlayerPosition::ST:
                slidingTackleCoeff = 6.0f;
                break;
            default:
                break;
            }
            mAttributes.TackleSliding = (UChar)((slidingTackleCoeff *
                (Float)mAttributes.TacklingGeneral + (Float)mAttributes.Decision) / (slidingTackleCoeff + 1.0f) + 0.5f);
            mAttributes.Balance = mAttributes.Agility;
            mAttributes.PosOffensive = (mAttributes.Finishing + mAttributes.ForwardRuns) / 2;
            mAttributes.PosDefensive = (mAttributes.ManMarking + mAttributes.Anticipation) / 2;
            mAttributes.Vision = (mAttributes.Creativity + mAttributes.Flair) / 2;
            mAttributes.Reactions = (mAttributes.Concentration + mAttributes.Decision + mAttributes.Determination) / 3;
            mAttributes.Diving = (mAttributes.ShotStopping + mAttributes.Jumping) / 2;
            mAttributes.Reflexes = (mAttributes.ShotStopping + mAttributes.Concentration) / 2;
            UChar tacticalAwarenessTable[8] = { 20, 30, 40, 50, 60, 70, 80, 90 };
            if (mTacticalEducation > 7)
                mAttributes.TacticAwareness = 99;
            else
                mAttributes.TacticAwareness = tacticalAwarenessTable[mTacticalEducation];
            //
        }

        UInt totalLeagueMatches = 0;
        for (auto &careerEntry : mHistory.mEntries)
            totalLeagueMatches += careerEntry.mMatches + careerEntry.mReserveMatches;
        UInt totalInternationalMatches = mNationalTeamMatches;

        if (reader.IsVersionGreaterOrEqual(0x2011, 0x04)) {
            UChar potentilValues[10] = { 50, 55, 60, 64, 68, 72, 76, 82, 88, 95 };
            if (mTalent > 9)
                mPotential = 99;
            else
                mPotential = potentilValues[mTalent];
            UChar playerLevel = GetLevel();
            if (playerLevel > mPotential)
                mPotential = playerLevel;

            UChar expLevel = mGeneralExperience / 3;

            if (totalLeagueMatches > 450)
                mNationalExperience = 6;
            else if (totalLeagueMatches > 350)
                mNationalExperience = 5;
            else if (totalLeagueMatches > 250)
                mNationalExperience = 4;
            else if (totalLeagueMatches > 150)
                mNationalExperience = 3;
            else if (totalLeagueMatches > 50)
                mNationalExperience = 2;
            else if (totalLeagueMatches > 20)
                mNationalExperience = 1;
            else
                mNationalExperience = 0;

            mNationalExperience = Utils::Min(mNationalExperience, expLevel);

            if (totalInternationalMatches > 100)
                mInternationalExperience = 4;
            else if (totalInternationalMatches > 60)
                mInternationalExperience = 3;
            else if (totalInternationalMatches > 30)
                mInternationalExperience = 2;
            else if (totalInternationalMatches > 10)
                mInternationalExperience = 1;
            if (expLevel >= 6)
                mInternationalExperience += 2;
            else if (expLevel >= 5)
                mInternationalExperience += 1;
            if (mInternationalExperience > 4)
                mInternationalExperience = 4;
        }
        else {
            mGeneralExperience = (UChar)((Float)(mNationalExperience * 2 + mInternationalExperience * 3) * 0.8f);
            if (mGeneralExperience > 18)
                mGeneralExperience = 18;

            // set player agent
            if (GetLevel() >= 55) {
                int randVal = Random::Get(0, 100);
                if (randVal >= 65) {
                    if (randVal >= 90)
                        mPlayerAgent = FifamPlayerAgent::Lawyer;
                    else
                        mPlayerAgent = FifamPlayerAgent::Agent;
                }
                else
                    mPlayerAgent = FifamPlayerAgent::Agency;
            }
            else
                mPlayerAgent = FifamPlayerAgent::None;
        }

        if (reader.IsVersionGreaterOrEqual(0x2007, 0x16)) {
            if (!mHistory.mEntries.empty()) {
                mFirstClub = (*mHistory.mEntries.begin()).mClub;
                if (mHistory.mEntries.size() > 1)
                    mPreviousClub = (*std::next(mHistory.mEntries.rbegin())).mClub;
            }
        }

        //FifamCheckEnum(mPersonType);
        //FifamCheckEnum(mNationality[0]);
        //FifamCheckEnum(mNationality[1]);
        //FifamCheckEnum(mMainPosition);
        //FifamCheckEnum(mPlayingStyle);
        //FifamCheckEnum(mPlayerAgent);
        //FifamCheckEnum(mChairmanStability);
        //if (mLeftFoot > 4)
        //    Error("mLeftFoot: unknown value (%u)", mLeftFoot);
        //if (mRightFoot > 4)
        //    Error("mRightFoot: unknown value (%u)", mRightFoot);
    }
}

void FifamPlayer::Write(FifamWriter &writer, FifamDatabase *database) {

}

UChar FifamPlayer::GetLevel(FifamPlayerPosition position, FifamPlayerPlayingStyle style, Bool experience) {
    return FifamPlayerLevel::GetPlayerLevel(this, position, style, experience);
}

UChar FifamPlayer::GetLevel(FifamPlayerPosition position, Bool experience) {
    return GetLevel(position, mPlayingStyle, experience);
}

UChar FifamPlayer::GetLevel(FifamPlayerPlayingStyle style, Bool experience) {
    return GetLevel(mMainPosition, style, experience);
}

UChar FifamPlayer::GetLevel(Bool experience) {
    return GetLevel(mMainPosition, mPlayingStyle, experience);
}
