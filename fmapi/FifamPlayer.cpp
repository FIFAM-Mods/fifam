#include "FifamPlayer.h"
#include "FifamPlayerLevel.h"
#include "FifamUtils.h"
#include "FifamNames.h"
#include "Random.h"

FifamPlayer::FifamPlayer() {
    mPersonType = FifamPersonType::Player;
}

void FifamPlayer::Read(FifamReader &reader) {
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
                mTransferRumors.resize(3);
                FifamUtils::SaveClubIDToClubLink(mTransferRumors[0], reader.ReadLine<UInt>());
                FifamUtils::SaveClubIDToClubLink(mTransferRumors[1], reader.ReadLine<UInt>());
                FifamUtils::SaveClubIDToClubLink(mTransferRumors[2], reader.ReadLine<UInt>());
            }
            mStartingConditions.Read(reader);
            mHistory.Read(reader);
            mContract.Read(reader);
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
            mIsNaturalised = Utils::CheckFlag(playerBasicFlags, 1);
            mIsBasque = Utils::CheckFlag(playerBasicFlags, 2);
            mIsRealPlayer = Utils::CheckFlag(playerBasicFlags, 4);
            reader.ReadLine(mBirthday);
            if (reader.IsVersionGreaterOrEqual(0x2009, 0x02)) {
                mInYouthTeam = Utils::CheckFlag(playerBasicFlags, 8);
                reader.ReadLine(mTalent);
                UChar footPrefs = reader.ReadLine<UChar>();
                mRightFoot = footPrefs & 0xF;
                mLeftFoot = (footPrefs >> 4) & 0xF;
            }
            else {
                UChar flags = reader.ReadLine<UChar>();
                UChar footPrefs = (flags >> 3) & 7;
                mTalent = (flags & 7) * 2 + 1;
                if (reader.IsVersionGreaterOrEqual(0x2006, 0)) {
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
                else {
                    if (reader.IsVersionGreaterOrEqual(0x2005, 0)) {
                        switch (footPrefs) {
                        case 0:
                        case 1:
                            mRightFoot = 4;
                            mLeftFoot = 4;
                            break;
                        case 2:
                            mRightFoot = 4;
                            mLeftFoot = 0;
                            break;
                        case 3:
                            mRightFoot = 0;
                            mLeftFoot = 4;
                            break;
                        default:
                            break;
                        }
                    }
                    else {
                        switch (footPrefs) {
                        case 0:
                        case 1:
                            mRightFoot = 4;
                            mLeftFoot = 4;
                            break;
                        case 2:
                            mRightFoot = 4;
                            mLeftFoot = 0;
                            break;
                        case 3:
                            mRightFoot = 0;
                            mLeftFoot = 4;
                            break;
                        default:
                            break;
                        }
                    }
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
                        mPositionBias[SecondaryPositions[i].ToInt()] = Float(Utils::Clamp(97 - i, 96, 100));
                }
            }
            mAttributes.Read(reader);
            reader.ReadLine(mPlayingStyle);
            if (!reader.IsVersionGreaterOrEqual(0x2011, 0x04))
                reader.ReadLine(mPotential);
            reader.ReadLine(mCharacter);
            if (reader.IsVersionGreaterOrEqual(0x2011, 0x01)) {
                reader.ReadLine(mAppearance.mGenericFace);
                reader.ReadLine(mAppearance.mHairStyle);
                reader.ReadLine(mAppearance.mHairColor);
                reader.ReadLine(mAppearance.mBeardType);
                reader.ReadLine(mAppearance.mSideburns);
                reader.ReadLine(mAppearance.mBeardColor);
                reader.ReadLine(mAppearance.mSkinColor);
                reader.ReadLine(mAppearance.mFaceVariation);
                reader.ReadLine(mAppearance.mEyeColour);
            }
            else {
                FifamPlayerAppearance::AppearanceInfo07 info07;
                reader.ReadLine(info07.hairColor);
                reader.ReadLine(info07.faceId);
                reader.ReadLine(info07.hairId);
                reader.ReadLine(info07.beardType);
                mAppearance.SetFrom07AppearanceInfo(info07);
            }
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
                if (reader.IsVersionGreaterOrEqual(0x2007, 0x1E))
                    FifamUtils::SaveClubIDToClubLink(mWouldnSignFor, reader.ReadLine<UInt>());
                reader.SkipLines(2);
            }
            FifamUtils::SavePlayerIDToPtr(mManagerFavouritePlayer, reader.ReadLine<UInt>());
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
            if (mContract.mOptionClub == 0 && Utils::CheckFlag(contractFlags, 0x40))
                mContract.mOptionClub = 1;
            if (mContract.mOptionPlayer == 0 && Utils::CheckFlag(contractFlags, 0x80))
                mContract.mOptionPlayer = 1;
            mStartingConditions.Read(reader);
            reader.ReadLine(mShoeType);
            reader.ReadLine(mLongSleeves);
            mHistory.Read(reader);
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
            mAttributes.Flair = (mAttributes.Vision + (flairAttributes[2] + flairAttributes[3]) / 2) / 2;
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
            if (reader.IsVersionGreaterOrEqual(0x2011, 0x04)) {
                mMaxBias = *std::max_element(mPositionBias.begin(), mPositionBias.end());
                if (mMaxBias != 100.0f && mMaxBias != 0.0f) {
                    Float mp = 100.0f / mMaxBias;
                    for (UInt i = 0; i < 18; i++)
                        mPositionBias[i] *= mp;
                }
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
            UChar potentialValues[10] = { 50, 55, 60, 64, 68, 72, 76, 82, 88, 95 };
            if (mTalent > 9)
                mPotential = 99;
            else
                mPotential = potentialValues[mTalent];
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
                Int randVal = Random::Get(0, 100);
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

void FifamPlayer::Write(FifamWriter &writer) {
    FifamPlayerPlayingStyle playingStyle = mPlayingStyle;
    if (writer.GetGameId() < 13 && playingStyle.ToInt() >= FifamPlayerPlayingStyle::BusyAttacker)
        playingStyle = FifamPlayerLevel::GetBestStyleForPlayer(this, false);
    Array<UChar, 18> positionBias12;
    if (!writer.IsVersionGreaterOrEqual(0x2013, 0x01)) {
        float maxBias = mMaxBias;
        if (maxBias == 100.0f)
            maxBias = (Float)FifamPlayerLevel::GetPlayerLevel12(this, mMainPosition, mPlayingStyle);
        for (UInt i = 0; i < 18; i++)
            positionBias12[i] = (UChar)(mPositionBias[i] * (maxBias / 100.0f));
    }
    writer.WriteStartIndex(L"PLAYER");
    if (writer.IsVersionGreaterOrEqual(0x2012, 0x01)) {
        writer.WriteLine(mPersonType);
        Array<String, 5> namesArray = { mFirstName, mLastName, mNickname, mPseudonym, L"0" };
        writer.WriteLineArray(namesArray, '|');
        writer.WriteLineArray(mNationality);
        writer.WriteLineArray(mLanguages);
        writer.WriteLine(mBirthday);
        if (writer.IsVersionGreaterOrEqual(0x2013, 0x04)) {
            UChar leadership = 1;
            // convert leadership from [0;99] to [1;10]
            if (mAttributes.Leadership >= 90)
                leadership = 10;
            else if (mAttributes.Leadership >= 85)
                leadership = 9;
            else if (mAttributes.Leadership >= 80)
                leadership = 8;
            else if (mAttributes.Leadership >= 75)
                leadership = 7;
            else if (mAttributes.Leadership >= 70)
                leadership = 6;
            else if (mAttributes.Leadership >= 60)
                leadership = 5;
            else if (mAttributes.Leadership >= 50)
                leadership = 4;
            else if (mAttributes.Leadership >= 40)
                leadership = 3;
            else if (mAttributes.Leadership >= 30)
                leadership = 2;
            //
            writer.WriteLine(mTalent, mTacticalEducation, leadership, mGeneralExperience, mMainPosition);
        }
        else
            writer.WriteLine(mTalent, mTacticalEducation, mGeneralExperience, mMainPosition);
        if (writer.IsVersionGreaterOrEqual(0x2013, 0x01)) {
            Array<UChar, 14> positionBias = {
                (UChar)mPositionBias[FifamPlayerPosition::None],
                (UChar)mPositionBias[FifamPlayerPosition::GK],
                (UChar)mPositionBias[FifamPlayerPosition::RB],
                (UChar)mPositionBias[FifamPlayerPosition::LB],
                (UChar)mPositionBias[FifamPlayerPosition::CB],
                (UChar)mPositionBias[FifamPlayerPosition::DM],
                (UChar)mPositionBias[FifamPlayerPosition::RM],
                (UChar)mPositionBias[FifamPlayerPosition::LM],
                (UChar)mPositionBias[FifamPlayerPosition::CM],
                (UChar)mPositionBias[FifamPlayerPosition::RW],
                (UChar)mPositionBias[FifamPlayerPosition::LW],
                (UChar)mPositionBias[FifamPlayerPosition::AM],
                (UChar)mPositionBias[FifamPlayerPosition::CF],
                (UChar)mPositionBias[FifamPlayerPosition::ST]
            };
            if ((UChar)mPositionBias[FifamPlayerPosition::SW] == 100)
                positionBias[4] = 100;
            if ((UChar)mPositionBias[FifamPlayerPosition::RWB] == 100)
                positionBias[2] = 100;
            if ((UChar)mPositionBias[FifamPlayerPosition::LWB] == 100)
                positionBias[3] = 100;
            if ((UChar)mPositionBias[FifamPlayerPosition::ANC] == 100)
                positionBias[5] = 100;
            writer.WriteLineArray(positionBias);
        }
        else
            writer.WriteLineArray(positionBias12);
        mAttributes.Write(writer);
        writer.WriteLine(mInReserveTeam);
        UInt flags = 0;
        Utils::SetFlag(flags, 0x1, mIsNaturalised);
        Utils::SetFlag(flags, 0x2, mIsBasque);
        Utils::SetFlag(flags, 0x4, mIsRealPlayer);
        Utils::SetFlag(flags, 0x8, mInYouthTeam);
        Utils::SetFlag(flags, 0x10, mRetiredFromNationalTeam);
        Utils::SetFlag(flags, 0x20, mCurrentlyInNationalTeam);
        if (writer.GetGameId() > 12) {
            Utils::SetFlag(flags, 0x40, mNoContractExtension);
            Utils::SetFlag(flags, 0x80, mIsCaptain);
        }
        writer.WriteLine(flags);
        writer.WriteLine((mRightFoot & 0xF) | ((mLeftFoot & 0xF) << 4));
        writer.WriteLine(mHeroStatus);
        writer.WriteLine(playingStyle);
        if (writer.IsVersionGreaterOrEqual(0x2013, 0x03)) {
            auto characterFlagas = mCharacter.ToInt() & 0x7FFFFFFFFF;
            if (mCharacter.Check(FifamPlayerCharacter::Diva))
                characterFlagas |= 1;
            if (mCharacter.Check(FifamPlayerCharacter::LifestyleIcon))
                characterFlagas |= 2;
            writer.WriteLine(characterFlagas);
        }
        else
            writer.WriteLine(mCharacter.ToInt() & 0xFFFFFF);
        writer.WriteLine(mAppearance.mGenericFace,
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
        writer.WriteLine(mSpecialFace, mEmpicsId, mHeight, mWeight, mShirtNumberFirstTeam, mShirtNumberReserveTeam);
        writer.WriteLine(mPlayerAgent);
        writer.WriteLine(mNationalTeamMatches, mNationalTeamGoals);
        writer.WriteLine(FifamUtils::GetWriteableID(mFavouriteClub));
        writer.WriteLine(FifamUtils::GetWriteableID(mWouldnSignFor));
        if (writer.IsVersionGreaterOrEqual(0x2013, 0x02)) {
            auto transferRumors = FifamUtils::MakeWriteableIDsList(mTransferRumors);
            for (UInt i = 0; i < 3; i++)
                writer.WriteLine(transferRumors[i]);
        }
        mStartingConditions.Write(writer);
        mHistory.Write(writer);
        mContract.Write(writer);
        writer.WriteLine(mManagerMotivationSkills, mManagerCoachingSkills, mManagerGoalkeepersTraining, mManagerNegotiationSkills);
        Int customFormation = GetProperty<Int>(L"custom_formation", -1);
        if (customFormation != -1)
            writer.WriteLine(customFormation);
        else
            writer.WriteLine(mManagerFavouriteFormation);
        writer.WriteLine(mChairmanStability);
        writer.WriteLine(mComment);
    }
    else {
        writer.WriteLine(mFirstName);
        writer.WriteLine(mLastName);
        writer.WriteLine(mNickname);
        writer.WriteLine(mPseudonym);
        writer.WriteLine(mPersonType);
        writer.WriteLine(mInReserveTeam);
        writer.WriteLine(mNationality[0]);
        writer.WriteLine(mNationality[1]);
        UChar playerBasicFlags = 0;
        Utils::SetFlag(playerBasicFlags, 1, mIsNaturalised);
        Utils::SetFlag(playerBasicFlags, 2, mIsBasque);
        Utils::SetFlag(playerBasicFlags, 4, mIsRealPlayer);
        if (writer.IsVersionGreaterOrEqual(0x2009, 0x02))
            Utils::SetFlag(playerBasicFlags, 8, mInYouthTeam);
        writer.WriteLine(playerBasicFlags);
        writer.WriteLine(mBirthday);
        if (writer.IsVersionGreaterOrEqual(0x2009, 0x02)) {
            writer.WriteLine(mTalent);
            writer.WriteLine((mRightFoot & 0xF) | ((mLeftFoot & 0xF) << 4));
        }
        else {
            UChar talent = mTalent / 2;
            UChar footPrefs = 0;
            if (mRightFoot >= 2 && mLeftFoot >= 2) {
                if (mLeftFoot > mRightFoot)
                    footPrefs = 3;
                else
                    footPrefs = 2;
            }
            else {
                if (mLeftFoot > mRightFoot)
                    footPrefs = 1;
                else
                    footPrefs = 0;
            }
            UChar flags = (talent & 7) | ((footPrefs & 7) << 3);
            Utils::SetFlag(flags, 0x40, mInYouthTeam);
            writer.WriteLine(flags);
        }
        writer.WriteLine(mHeroStatus);

        if (writer.IsVersionGreaterOrEqual(0x2011, 0x04)) {
            for (UInt i = 0; i < 18; i++)
                writer.WriteLine(positionBias12[i]);
            writer.WriteLine(mMainPosition);
        }
        else {
            Vector<UInt> PreferredPositions;
            Vector<UInt> SecondaryPositions;
            PreferredPositions.push_back(mMainPosition.ToInt());
            using PositionPair = Pair<Float, UInt>;
            Array<PositionPair, 18> bestPositions;
            for (UInt i = 0; i < 18; i++) {
                bestPositions[i].first = mPositionBias[i];
                bestPositions[i].second = i;
            }
            bestPositions[0].first = 0.0f;
            std::sort(bestPositions.begin(), bestPositions.end(), [](PositionPair const &a, PositionPair const &b) {
                return a.first > b.first;
            });
            // preferred
            for (UInt i = 0; i < 18; i++) {
                if (bestPositions[i].first >= 98.0f) {
                    if (PreferredPositions.size() < 3 && !Utils::Contains(PreferredPositions, bestPositions[i].second))
                        PreferredPositions.push_back(bestPositions[i].second);
                }
                else
                    break;
            }
            // secondary
            for (UInt i = 0; i < 18; i++) {
                if (bestPositions[i].first >= 96.0f) {
                    if (SecondaryPositions.size() < 3 && !Utils::Contains(PreferredPositions, bestPositions[i].second)
                        && !Utils::Contains(SecondaryPositions, bestPositions[i].second)) // edited
                    {
                        SecondaryPositions.push_back(bestPositions[i].second);
                    }
                }
                else
                    break;
            }
            for (UInt i = 0; i < 3; i++) {
                if (i < PreferredPositions.size())
                    writer.WriteLine(PreferredPositions[i]);
                else
                    writer.WriteLine(0);
            }
            for (UInt i = 0; i < 3; i++) {
                if (i < SecondaryPositions.size())
                    writer.WriteLine(SecondaryPositions[i]);
                else
                    writer.WriteLine(0);
            }
        }
        mAttributes.Write(writer);
        writer.WriteLine(playingStyle);
        if (!writer.IsVersionGreaterOrEqual(0x2011, 0x04))
            writer.WriteLine(mPotential);
        writer.WriteLine(mCharacter.ToInt() & 0x7FFFFF);
        if (writer.IsVersionGreaterOrEqual(0x2011, 0x01)) {
            writer.WriteLine(mAppearance.mGenericFace);
            writer.WriteLine(mAppearance.mHairStyle);
            writer.WriteLine(mAppearance.mHairColor);
            writer.WriteLine(mAppearance.mBeardType);
            writer.WriteLine(mAppearance.mSideburns);
            writer.WriteLine(mAppearance.mBeardColor);
            writer.WriteLine(mAppearance.mSkinColor);
            writer.WriteLine(mAppearance.mFaceVariation);
            writer.WriteLine(mAppearance.mEyeColour);
        }
        else {
            auto info07 = mAppearance.Get07AppearanceInfo();
            writer.WriteLine(info07.hairColor);
            writer.WriteLine(info07.faceId);
            writer.WriteLine(info07.hairId);
            writer.WriteLine(info07.beardType);
        }
        writer.WriteLine(mHeight);
        writer.WriteLine(mWeight);
        if (writer.IsVersionGreaterOrEqual(0x2011, 0x04))
            writer.WriteLine(mGeneralExperience);
        else {
            writer.WriteLine(mNationalExperience);
            writer.WriteLine(mInternationalExperience);
        }
        writer.WriteLine(mShirtNumberFirstTeam);
        writer.WriteLine(mShirtNumberReserveTeam);
        writer.WriteLine(mSpecialFace);
        writer.WriteLine(mEmpicsId);
        if (!writer.IsVersionGreaterOrEqual(0x2007, 0x16)) {
            auto firstClubId = FifamUtils::GetWriteableID(mFirstClub);
            writer.WriteLine(FifamUtils::GetCountryIDFromClubID(firstClubId));
            writer.WriteLine(firstClubId);
            auto previousClubId = FifamUtils::GetWriteableID(mPreviousClub);
            writer.WriteLine(FifamUtils::GetCountryIDFromClubID(previousClubId));
            writer.WriteLine(previousClubId);
        }
        writer.WriteLine(mContract.mJoined.year);
        writer.WriteLine(mContract.mValidUntil.year);
        writer.WriteLine(mContract.mBasicSalary);
        writer.WriteLine(mContract.mFixTransferFee);
        writer.WriteLine(mCurrentEstimatedMarketValue);
        writer.WriteLine(mNationalTeamMatches);
        writer.WriteLine(mNationalTeamGoals);
        writer.WriteLine(mManagerMotivationSkills);
        writer.WriteLine(mManagerCoachingSkills);
        writer.WriteLine(mManagerGoalkeepersTraining);
        writer.WriteLine(mManagerNegotiationSkills);
        writer.WriteLine(mLanguages[0]);
        writer.WriteLine(mLanguages[1]);
        writer.WriteLine(mLanguages[2]);
        writer.WriteLine(mLanguages[3]);
        if (writer.IsVersionGreaterOrEqual(0x2009, 0x0A)) {
            writer.WriteLine(FifamUtils::GetWriteableID(mFavouriteClub));
            writer.WriteLine(FifamUtils::GetWriteableID(mWouldnSignFor));
            if (FifamUtils::GetWriteableID(mManagerFavouritePlayer))
                writer.WriteLine(FifamUtils::GetWriteableID(mManagerFavouritePlayer->mClub));
            else
                writer.WriteLine(0);
        }
        else {
            auto favouriteClubId = FifamUtils::GetWriteableID(mFavouriteClub);
            writer.WriteLine(FifamUtils::GetCountryIDFromClubID(favouriteClubId));
            writer.WriteLine(favouriteClubId);
            if (writer.IsVersionGreaterOrEqual(0x2007, 0x0E))
                writer.WriteLine(FifamUtils::GetWriteableID(mWouldnSignFor));
            if (FifamUtils::GetWriteableID(mManagerFavouritePlayer)) {
                auto favouritePlayerClubId = FifamUtils::GetWriteableID(mManagerFavouritePlayer->mClub);
                writer.WriteLine(FifamUtils::GetCountryIDFromClubID(favouritePlayerClubId));
                writer.WriteLine(favouritePlayerClubId);
            }
            else {
                writer.WriteLine(0);
                writer.WriteLine(0);
            }
        }
        if (FifamUtils::GetWriteableID(mManagerFavouritePlayer) && FifamUtils::GetWriteableID(mManagerFavouritePlayer->mClub))
            writer.WriteLine(FifamUtils::GetWriteableID(mManagerFavouritePlayer));
        else
            writer.WriteLine(0);
        Int customFormation = GetProperty<Int>(L"custom_formation", -1);
        if (customFormation != -1)
            writer.WriteLine(customFormation);
        else
            writer.WriteLine(mManagerFavouriteFormation);
        writer.WriteLine(mChairmanStability);
        writer.WriteLine(mTacticalEducation);
        UInt contractFlags = 0;
        Utils::SetFlag(contractFlags, 0x1, mRetiredFromNationalTeam);
        Utils::SetFlag(contractFlags, 0x2, mCurrentlyInNationalTeam);
        if (writer.IsVersionGreaterOrEqual(0x2009, 0x0A)) {
            Utils::SetFlag(contractFlags, 0x100, mContract.mExtendAvoidRelegation);
            Utils::SetFlag(contractFlags, 0x400, mContract.mClauseNoInternational.mEnabled);
            Utils::SetFlag(contractFlags, 0x800, mContractIsCancelledIfRelegated);
            Utils::SetFlag(contractFlags, 0x1000, mContract.mClauseRelegation.mEnabled);
            Utils::SetFlag(contractFlags, 0x2000, mContract.mClauseNoPromotion.mEnabled);
            Utils::SetFlag(contractFlags, 0x4000, mContract.mAutoExtend != 0);
        }
        if (writer.IsVersionGreaterOrEqual(0x2009, 0x07)) {
            Utils::SetFlag(contractFlags, 0x200, mContract.mOptionClub != 0);
            Utils::SetFlag(contractFlags, 0x8000, mContract.mOptionPlayer != 0);
            writer.WriteLine(contractFlags);
            writer.WriteLine(mContract.mOptionPlayer);
            writer.WriteLine(mContract.mOptionClub);
            writer.WriteLine(mContract.mAutoExtend);
            writer.WriteLine(mContract.mClauseNoInternational.mValue);
            writer.WriteLine(mContract.mClauseRelegation.mValue);
            writer.WriteLine(mContract.mClauseNoPromotion.mValue);
        }
        else {
            Utils::SetFlag(contractFlags, 0x40, mContract.mOptionClub != 0);
            Utils::SetFlag(contractFlags, 0x80, mContract.mOptionPlayer != 0);
            writer.WriteLine(contractFlags);
        }
        mStartingConditions.Write(writer);
        writer.WriteLine(mShoeType);
        writer.WriteLine(mLongSleeves);
        mHistory.Write(writer);
        writer.WriteLine(mComment);
        if (writer.IsVersionGreaterOrEqual(0x2011, 0x01))
            writer.WriteLine(mPlayerAgent);
        else
            writer.WriteLine(Unknown._1);
    }
    writer.WriteEndIndex(L"PLAYER");
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

Bool FifamPlayer::SortPlayersByLevel(FifamPlayer *player1, FifamPlayer *player2) {
    return player1->GetLevel() > player2->GetLevel();
}

void FifamPlayer::ForAllAttributes(Function<void(UChar &, FifamPlayerAbilityID const &)> callback) {
    callback(mAttributes.BallControl, FifamPlayerAbilityID::BallControl);
    callback(mAttributes.Volleys, FifamPlayerAbilityID::Volleys);
    callback(mAttributes.Dribbling, FifamPlayerAbilityID::Dribbling);
    callback(mAttributes.Finishing, FifamPlayerAbilityID::Finishing);
    callback(mAttributes.ShotPower, FifamPlayerAbilityID::ShotPower);
    callback(mAttributes.LongShots, FifamPlayerAbilityID::LongShots);
    callback(mAttributes.Crossing, FifamPlayerAbilityID::Crossing);
    callback(mAttributes.Passing, FifamPlayerAbilityID::Passing);
    callback(mAttributes.LongPassing, FifamPlayerAbilityID::LongPassing);
    callback(mAttributes.Header, FifamPlayerAbilityID::Header);
    callback(mAttributes.TacklingGeneral, FifamPlayerAbilityID::Tackling);
    callback(mAttributes.TackleStanding, FifamPlayerAbilityID::TackleStanding);
    callback(mAttributes.TackleSliding, FifamPlayerAbilityID::TackleSliding);
    callback(mAttributes.ManMarking, FifamPlayerAbilityID::ManMarking);
    callback(mAttributes.Technique, FifamPlayerAbilityID::Technique);
    callback(mAttributes.Creativity, FifamPlayerAbilityID::Creativity);
    callback(mAttributes.Flair, FifamPlayerAbilityID::Flair);
    callback(mAttributes.Touch, FifamPlayerAbilityID::Touch);
    callback(mAttributes.Balance, FifamPlayerAbilityID::Balance);
    callback(mAttributes.Acceleration, FifamPlayerAbilityID::Acceleration);
    callback(mAttributes.Pace, FifamPlayerAbilityID::Pace);
    callback(mAttributes.Agility, FifamPlayerAbilityID::Agility);
    callback(mAttributes.Jumping, FifamPlayerAbilityID::Jumping);
    callback(mAttributes.Strength, FifamPlayerAbilityID::Strength);
    callback(mAttributes.Stamina, FifamPlayerAbilityID::Stamina);
    callback(mAttributes.WorkRate, FifamPlayerAbilityID::WorkRate);
    callback(mAttributes.ForwardRuns, FifamPlayerAbilityID::ForwardRuns);
    callback(mAttributes.PosOffensive, FifamPlayerAbilityID::PosOffensive);
    callback(mAttributes.PosDefensive, FifamPlayerAbilityID::PosDefensive);
    callback(mAttributes.Vision, FifamPlayerAbilityID::Vision);
    callback(mAttributes.Reactions, FifamPlayerAbilityID::Reactions);
    callback(mAttributes.TacticAwareness, FifamPlayerAbilityID::TacticAwareness);
    callback(mAttributes.Aggression, FifamPlayerAbilityID::Aggression);
    callback(mAttributes.Composure, FifamPlayerAbilityID::Composure);
    callback(mAttributes.Consistency, FifamPlayerAbilityID::Consistency);
    callback(mAttributes.Leadership, FifamPlayerAbilityID::Leadership);
    callback(mAttributes.Anticipation, FifamPlayerAbilityID::Anticipation);
    callback(mAttributes.Concentration, FifamPlayerAbilityID::Concentration);
    callback(mAttributes.Decision, FifamPlayerAbilityID::Decision);
    callback(mAttributes.Determination, FifamPlayerAbilityID::Determination);
    callback(mAttributes.TeamWork, FifamPlayerAbilityID::TeamWork);
    callback(mAttributes.Intimidation, FifamPlayerAbilityID::Intimidation);
    callback(mAttributes.FreeKicks, FifamPlayerAbilityID::FreeKicks);
    callback(mAttributes.Corners, FifamPlayerAbilityID::Corners);
    callback(mAttributes.PenaltyShot, FifamPlayerAbilityID::PenaltyShot);
    callback(mAttributes.Diving, FifamPlayerAbilityID::Diving);
    callback(mAttributes.Reflexes, FifamPlayerAbilityID::Reflexes);
    callback(mAttributes.Handling, FifamPlayerAbilityID::Handling);
    callback(mAttributes.Positioning, FifamPlayerAbilityID::Positioning);
    callback(mAttributes.OneOnOne, FifamPlayerAbilityID::OneOnOne);
    callback(mAttributes.Kicking, FifamPlayerAbilityID::GoalKicks);
    callback(mAttributes.Punching, FifamPlayerAbilityID::Punching);
    callback(mAttributes.ShotStopping, FifamPlayerAbilityID::ShotStopping);
    callback(mAttributes.Throwing, FifamPlayerAbilityID::Throwing);
    callback(mAttributes.GkCrosses, FifamPlayerAbilityID::GkCrosses);
}

String FifamPlayer::GetStringUniqueId(UInt gameId, Bool includeEmpicsId) {
    return FifamNames::GetPersonStringId(gameId, mFirstName, mLastName, mPseudonym, mBirthday, includeEmpicsId ? mEmpicsId : 0);
}

// must be called when: player age is known; player position is known; player history is known
void FifamPlayer::ValidateExperience(FifamDate const &currentDate) {
    struct ExperienceCalcEntry {
        UChar exp0to6, minAge, maxAge;
        UShort minRating, maxRating;
    };

    // experience table, sorted by experience value (0-6)
    static ExperienceCalcEntry expCalcTable[] = {
        { 0,17,19,0,10 },{ 0,20,21,0,15 },{ 0,22,22,0,5 },
        { 1,17,17,10,9999 },{ 1,18,19,5,9999 },{ 1,20,22,5,75 },{ 1,23,24,0,75 },{ 1,25,25,0,50 },
        { 2,20,20,75,9999 },{ 2,21,22,50,9999 },{ 2,23,24,50,150 },{ 2,25,26,0,150 },{ 2,27,27,0,100 },
        { 3,22,22,200,9999 },{ 3,23,24,100,9999 },{ 3,25,27,100,250 },{ 3,28,29,0,250 },{ 3,30,30,0,200 },
        { 4,25,25,250,9999 },{ 4,26,27,200,9999 },{ 4,28,30,200,400 },{ 4,31,32,0,400 },{ 4,33,33,0,300 },
        { 5,28,28,400,9999 },{ 5,29,30,300,9999 },{ 5,31,33,300,650 },{ 5,34,35,0,650 },{ 5,36,36,0,500 },
        { 6,31,31,650,9999 },{ 6,32,33,500,9999 },{ 6,34,34,400,9999 },{ 6,35,99,0,9999 }
    };

    // gather player info
    UChar age = GetAge(currentDate);
    Bool isGK = mMainPosition == FifamPlayerPosition::GK;
    UInt rating = mNationalTeamMatches * 2;
    for (auto const &h : mHistory.mEntries) {
        rating += h.mMatches;
        rating += h.mReserveMatches / 2;
    }

    // find table entries
    ExperienceCalcEntry *firstEntry = nullptr;
    ExperienceCalcEntry *lastEntry = nullptr;

    for (UInt i = 0; i < std::size(expCalcTable); i++) {
        if (age >= expCalcTable[i].minAge && age <= expCalcTable[i].maxAge && rating <= expCalcTable[i].maxRating) {
            UInt minRating = expCalcTable[i].minRating;
            if (isGK)
                minRating /= 2;
            if (rating >= minRating) {
                if (!firstEntry)
                    firstEntry = &expCalcTable[i];
                lastEntry = &expCalcTable[i];
            }
        }
    }

    // set new experience if needed
    if (firstEntry) {
        UChar currExpLevel = mGeneralExperience / 3;
        if (currExpLevel < firstEntry->exp0to6)
            mGeneralExperience = firstEntry->exp0to6 * 3;
        else if (currExpLevel > lastEntry->exp0to6) {
            mGeneralExperience = lastEntry->exp0to6 * 3 + 2;
            if (mGeneralExperience > 18)
                mGeneralExperience = 18;
        }
    }
}
