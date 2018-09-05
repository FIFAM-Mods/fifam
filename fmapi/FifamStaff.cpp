#include "FifamStaff.h"
#include "FifamUtils.h"
#include "FifamPlayer.h"

void FifamStaff::Read(FifamReader &reader, FifamDatabase *database) {
    if (reader.ReadStartIndex(L"STAFF")) {
        if (reader.IsVersionGreaterOrEqual(0x2009, 0x05)) {
            UChar type = reader.ReadLine<UChar>();
            if (type < 4) {
                mClubPosition.SetFromInt(type);
                ReadManager(reader, database);
            }
            else
                ReadWorker(reader, database);
        }
        else
            ReadManager(reader, database);
        reader.ReadEndIndex(L"STAFF");
    }
}

void FifamStaff::ReadWorker(FifamReader &reader, FifamDatabase *database) {
    reader.ReadFullLine(mFirstName);
    reader.ReadFullLine(mLastName);
    reader.ReadFullLine(mNickname);
    reader.ReadFullLine(mPseudonym);
    reader.ReadLine(mBirthdate);
    UChar numNationalities = reader.ReadLine<UChar>();
    for (UInt i = 0; i < numNationalities; i++) {
        if (i < 2)
            reader.ReadLine(mNationality[i]);
        else
            reader.SkipLine();
    }
    FifamUtils::SaveClubIDToClubLink(mFavouriteClub, reader.ReadLine<UInt>());
    FifamUtils::SaveClubIDToClubLink(mWouldNeverWorkForClub, reader.ReadLine<UInt>());
    UChar numPersonalityAttributes = reader.ReadLine<UChar>();
    for (UInt i = 0; i < numPersonalityAttributes; i++) {
        UChar attrId = reader.ReadLine<UChar>();
        if (attrId == 0)
            reader.ReadLine(mPersonalityAttributes.WillingnessToLearn);
        else if (attrId == 1)
            reader.ReadLine(mPersonalityAttributes.Resilence);
        else if (attrId == 2)
            reader.ReadLine(mPersonalityAttributes.Effort);
        else
            reader.SkipLine();
    }
    UChar numSkills = reader.ReadLine<UChar>();
    for (UInt i = 0; i < numSkills; i++) {
        UChar skillId = reader.ReadLine<UChar>();
        if (skillId == 0)
            reader.ReadLine(mTalent);
        else if (skillId == 1)
            reader.ReadLine(mSkills.Tactics);
        else if (skillId == 2)
            reader.ReadLine(mSkills.FieldPlayerTraining);
        else if (skillId == 3)
            reader.ReadLine(mSkills.GoalkeeperTraining);
        else if (skillId == 4)
            reader.ReadLine(mSkills.FitnessTraining);
        else if (skillId == 5)
            reader.ReadLine(mSkills.SkillEstimation);
        else if (skillId == 6)
            reader.ReadLine(mSkills.TeamEstimation);
        else if (skillId == 7)
            reader.ReadLine(mSkills.MotivationAbility);
        else if (skillId == 8)
            reader.ReadLine(mSkills.BoneInjury);
        else if (skillId == 9)
            reader.ReadLine(mSkills.KneeInjury);
        else if (skillId == 10)
            reader.ReadLine(mSkills.MuscleInjury);
        else if (skillId == 11)
            reader.ReadLine(mSkills.InjuryPrevention);
        else if (skillId == 12)
            reader.ReadLine(mSkills.RegenerationAbility);
        else if (skillId == 13)
            reader.ReadLine(mSkills.Arbitrate);
        else if (skillId == 14)
            reader.ReadLine(mSkills.Negotiation);
        else if (skillId == 15)
            reader.ReadLine(mSkills.Marketing);
        else if (skillId == 16)
            reader.ReadLine(mSkills.Sponsoring);
        else if (skillId == 17)
            reader.ReadLine(mSkills.Construction);
        else if (skillId == 18)
            reader.ReadLine(mSkills.PR);
        else if (skillId == 19)
            reader.ReadLine(mSkills.FanContact);
        else if (skillId == 20)
            reader.ReadLine(mSkills.SportsLaw);
        else if (skillId == 21)
            reader.ReadLine(mSkills.LaborLaw);
        else if (skillId == 22)
            reader.ReadLine(mSkills.GeneralScouting);
        else if (skillId == 23)
            reader.ReadLine(mSkills.TalentEstimation);
        else if (skillId == 24)
            reader.ReadLine(mSkills.FieldSkillsEstimation);
        else if (skillId == 25)
            reader.ReadLine(mSkills.GoalkeeperSkillsEstimation);
        else if (skillId == 26)
            reader.ReadLine(mSkills.MentalSkillsEstimation);
        else if (skillId == 27)
            reader.ReadLine(mSkills.PhysicalSkillsEstimation);
        else if (skillId == 28)
            reader.ReadLine(mSkills.Networking);
        else
            reader.SkipLine();
    }
    mHasNoneJobData = false;
    mHasCoachJobData = false;
    mHasMedicineJobData = false;
    mHasManagerJobData = false;
    mHasPRJobData = false;
    mHasLawyerJobData = false;
    mHasScoutJobData = false;
    UChar numJobDatas = reader.ReadLine<UChar>();
    for (UInt i = 0; i < numJobDatas; i++) {
        UChar jobType = reader.ReadLine<UChar>();
        reader.SkipLine();
        if (jobType == 0)
            mHasNoneJobData = true;
        else if (jobType == 1) {
            mHasCoachJobData = true;
            reader.ReadLine(mFavouriteFormation);
            reader.ReadLine(mCoachPlayingOrientation);
        }
        else if (jobType == 2)
            mHasMedicineJobData = true;
        else if (jobType == 3)
            mHasManagerJobData = true;
        else if (jobType == 4)
            mHasPRJobData = true;
        else if (jobType == 5)
            mHasLawyerJobData = true;
        else if (jobType == 6) {
            mHasScoutJobData = true;
            UChar numPreferredCountries = reader.ReadLine<UChar>();
            mScoutPreferredCountries.clear();
            for (UInt i = 0; i < numPreferredCountries; i++)
                mScoutPreferredCountries.push_back(reader.ReadLine<FifamNation>());
        }
    }
    if (reader.IsVersionGreaterOrEqual(0x2009, 0x05))
        reader.ReadLine(mClubPosition);
    else {
        UChar unkCount = reader.ReadLine<UChar>();
        reader.SkipLines(unkCount * 2);
    }
    if (reader.IsVersionGreaterOrEqual(0x2011, 0x0A)) {
        UShort joinedYear = reader.ReadLine<UShort>();
        mJoinedClubDate.Set(1, 7, joinedYear);
    }
    mLinkedCountry = mNationality[0];
}

void FifamStaff::ReadManager(FifamReader &reader, FifamDatabase *database) {
    reader.ReadFullLine(mFirstName);
    reader.ReadFullLine(mLastName);
    if (!reader.IsVersionGreaterOrEqual(0x2009, 0x05))
        reader.ReadLine(mClubPosition);
    reader.ReadLine(mBirthdate);
    reader.ReadLine(mNationality[0]);
    reader.ReadLine(Unknown._1);
    reader.ReadLine(mExperience);
    if (reader.IsVersionGreaterOrEqual(0x2011, 0x0A)) {
        UShort joinedYear = reader.ReadLine<UShort>();
        mJoinedClubDate.Set(1, 7, joinedYear);
    }
    if (reader.IsVersionGreaterOrEqual(0x2007, 0x1E)) {
        reader.ReadFullLine(mNickname);
        reader.ReadFullLine(mPseudonym);
        reader.ReadLine(mNationality[1]);
        FifamUtils::SaveClubIDToClubLink(mFavouriteClub, reader.ReadLine<UInt>());
        FifamUtils::SaveClubIDToClubLink(mWouldNeverWorkForClub, reader.ReadLine<UInt>());
    }
    reader.ReadLine(mManagerMotivationSkills);
    reader.ReadLine(mManagerCoachingSkills);
    reader.ReadLine(mManagerGoalkeepersTraining);
    reader.ReadLine(mManagerNegotiationSkills);
    reader.ReadLine(mManagerFocus);
    reader.ReadLine(mLanguage[0]);
    reader.ReadLine(mLanguage[1]);
    reader.ReadLine(mLanguage[2]);
    reader.ReadLine(mLanguage[3]);
    reader.ReadLine(mFavouriteFormation);
    reader.ReadLine(Unknown._2);
    reader.ReadLine(mChairmanStability);
    mLinkedCountry = mNationality[0];
}

void FifamStaff::ReadFromPlayer(FifamReader &reader, FifamDatabase *database) {
    FifamPlayer player;
    player.Read(reader, database);
    mClubPosition.SetFromInt(player.mPersonType.ToInt());
    mFirstName = player.mFirstName;
    mLastName = player.mLastName;
    mPseudonym = player.mPseudonym;
    mNickname = player.mNickname;
    mBirthdate = player.mBirthday;
    mNationality = player.mNationality;
    mFavouriteClub = player.mFavouriteClub;
    mWouldNeverWorkForClub = player.mWouldnSignFor;
    mFavouritePlayer = player.mManagerFavouritePlayer;
    mFavouriteFormation = player.mManagerFavouriteFormation;
    mChairmanStability = player.mChairmanStability;
    mManagerFocus.SetFromInt((Char)player.mCharacter.ToInt());
    mManagerMotivationSkills = player.mManagerMotivationSkills;
    mManagerCoachingSkills = player.mManagerCoachingSkills;
    mManagerGoalkeepersTraining = player.mManagerGoalkeepersTraining;
    mManagerNegotiationSkills = player.mManagerNegotiationSkills;
    mLanguage = player.mLanguages;
    mTalent = player.mTalent;
    mJoinedClubDate = player.mContract.mJoined;
}

void FifamStaff::Write(FifamWriter &writer, FifamDatabase *database) {
    writer.WriteStartIndex(L"STAFF");
    if (writer.IsVersionGreaterOrEqual(0x2009, 0x05)) {
        if (mClubPosition.ToInt() < 4) {
            writer.WriteLine(mClubPosition.ToInt());
            WriteManager(writer, database);
        }
        else {
            writer.WriteLine(4);
            WriteWorker(writer, database);
        }
    }
    else
        WriteManager(writer, database);
    writer.WriteEndIndex(L"STAFF");
}

void FifamStaff::WriteManager(FifamWriter &writer, FifamDatabase *database) {
    writer.WriteLine(mFirstName);
    writer.WriteLine(mLastName);
    if (!writer.IsVersionGreaterOrEqual(0x2009, 0x05))
        writer.WriteLine(mClubPosition);
    writer.WriteLine(mBirthdate);
    writer.WriteLine(mNationality[0]);
    writer.WriteLine(Unknown._1);
    writer.WriteLine(mExperience);
    if (writer.IsVersionGreaterOrEqual(0x2011, 0x0A))
        writer.WriteLine(mJoinedClubDate.year);
    if (writer.IsVersionGreaterOrEqual(0x2007, 0x1E)) {
        writer.WriteLine(mNickname);
        writer.WriteLine(mPseudonym);
        writer.WriteLine(mNationality[1]);
        writer.WriteLine(FifamUtils::DBClubLinkToID(database, mFavouriteClub, writer.GetGameId()));
        writer.WriteLine(FifamUtils::DBClubLinkToID(database, mWouldNeverWorkForClub, writer.GetGameId()));
    }
    writer.WriteLine(mManagerMotivationSkills);
    writer.WriteLine(mManagerCoachingSkills);
    writer.WriteLine(mManagerGoalkeepersTraining);
    writer.WriteLine(mManagerNegotiationSkills);
    writer.WriteLine(mManagerFocus);
    writer.WriteLine(mLanguage[0]);
    writer.WriteLine(mLanguage[1]);
    writer.WriteLine(mLanguage[2]);
    writer.WriteLine(mLanguage[3]);
    writer.WriteLine(mFavouriteFormation);
    writer.WriteLine(Unknown._2);
    writer.WriteLine(mChairmanStability);
}

void FifamStaff::WriteWorker(FifamWriter &writer, FifamDatabase *database) {
    writer.WriteLine(mFirstName);
    writer.WriteLine(mLastName);
    writer.WriteLine(mNickname);
    writer.WriteLine(mPseudonym);
    writer.WriteLine(mBirthdate);
    writer.WriteLine(2);
    writer.WriteLine(mNationality[0]);
    writer.WriteLine(mNationality[1]);
    writer.WriteLine(FifamUtils::DBClubLinkToID(database, mFavouriteClub, writer.GetGameId()));
    writer.WriteLine(FifamUtils::DBClubLinkToID(database, mWouldNeverWorkForClub, writer.GetGameId()));
    writer.WriteLine(3);
    writer.WriteLine(0);
    writer.WriteLine(mPersonalityAttributes.WillingnessToLearn);
    writer.WriteLine(1);
    writer.WriteLine(mPersonalityAttributes.Resilence);
    writer.WriteLine(2);
    writer.WriteLine(mPersonalityAttributes.Effort);
    writer.WriteLine(29);
    writer.WriteLine(0);
    writer.WriteLine(mTalent);
    writer.WriteLine(1);
    writer.WriteLine(mSkills.Tactics);
    writer.WriteLine(2);
    writer.WriteLine(mSkills.FieldPlayerTraining);
    writer.WriteLine(3);
    writer.WriteLine(mSkills.GoalkeeperTraining);
    writer.WriteLine(4);
    writer.WriteLine(mSkills.FitnessTraining);
    writer.WriteLine(5);
    writer.WriteLine(mSkills.SkillEstimation);
    writer.WriteLine(6);
    writer.WriteLine(mSkills.TeamEstimation);
    writer.WriteLine(7);
    writer.WriteLine(mSkills.MotivationAbility);
    writer.WriteLine(8);
    writer.WriteLine(mSkills.BoneInjury);
    writer.WriteLine(9);
    writer.WriteLine(mSkills.KneeInjury);
    writer.WriteLine(10);
    writer.WriteLine(mSkills.MuscleInjury);
    writer.WriteLine(11);
    writer.WriteLine(mSkills.InjuryPrevention);
    writer.WriteLine(12);
    writer.WriteLine(mSkills.RegenerationAbility);
    writer.WriteLine(13);
    writer.WriteLine(mSkills.Arbitrate);
    writer.WriteLine(14);
    writer.WriteLine(mSkills.Negotiation);
    writer.WriteLine(15);
    writer.WriteLine(mSkills.Marketing);
    writer.WriteLine(16);
    writer.WriteLine(mSkills.Sponsoring);
    writer.WriteLine(17);
    writer.WriteLine(mSkills.Construction);
    writer.WriteLine(18);
    writer.WriteLine(mSkills.PR);
    writer.WriteLine(19);
    writer.WriteLine(mSkills.FanContact);
    writer.WriteLine(20);
    writer.WriteLine(mSkills.SportsLaw);
    writer.WriteLine(21);
    writer.WriteLine(mSkills.LaborLaw);
    writer.WriteLine(22);
    writer.WriteLine(mSkills.GeneralScouting);
    writer.WriteLine(23);
    writer.WriteLine(mSkills.TalentEstimation);
    writer.WriteLine(24);
    writer.WriteLine(mSkills.FieldSkillsEstimation);
    writer.WriteLine(25);
    writer.WriteLine(mSkills.GoalkeeperSkillsEstimation);
    writer.WriteLine(26);
    writer.WriteLine(mSkills.MentalSkillsEstimation);
    writer.WriteLine(27);
    writer.WriteLine(mSkills.PhysicalSkillsEstimation);
    writer.WriteLine(28);
    writer.WriteLine(mSkills.Networking);
    UInt numJobs = (mHasNoneJobData ? 1 : 0)
        + (mHasCoachJobData ? 1 : 0)
        + (mHasMedicineJobData ? 1 : 0)
        + (mHasManagerJobData ? 1 : 0)
        + (mHasPRJobData ? 1 : 0)
        + (mHasLawyerJobData ? 1 : 0)
        + (mHasScoutJobData ? 1 : 0);
    writer.WriteLine(numJobs);
    if (mHasNoneJobData) {
        writer.WriteLine(0);
        writer.WriteLine(0);
    }
    if (mHasCoachJobData) {
        writer.WriteLine(1);
        writer.WriteLine(0);
        writer.WriteLine(mFavouriteFormation);
        writer.WriteLine(mCoachPlayingOrientation);
    }
    if (mHasNoneJobData) {
        writer.WriteLine(2);
        writer.WriteLine(0);
    }
    if (mHasNoneJobData) {
        writer.WriteLine(3);
        writer.WriteLine(0);
    }
    if (mHasNoneJobData) {
        writer.WriteLine(4);
        writer.WriteLine(0);
    }
    if (mHasNoneJobData) {
        writer.WriteLine(5);
        writer.WriteLine(0);
    }
    if (mHasNoneJobData) {
        writer.WriteLine(6);
        writer.WriteLine(0);
        UInt numPreferredCountries = mScoutPreferredCountries.size();
        if (numPreferredCountries > 3)
            numPreferredCountries = 3;
        writer.WriteLine(numPreferredCountries);
        for (UInt i = 0; i < numPreferredCountries; i++)
            writer.WriteLine(mScoutPreferredCountries[i]);
    }
    if (writer.IsVersionGreaterOrEqual(0x2009, 0x05))
        writer.WriteLine(mClubPosition);
    else
        writer.WriteLine(0);
    if (writer.IsVersionGreaterOrEqual(0x2011, 0x0A))
        writer.WriteLine(mJoinedClubDate.year);
}

void FifamStaff::WriteToPlayer(FifamWriter &writer, FifamDatabase *database) {
    FifamPlayer player;
    player.mPersonType.SetFromInt(mClubPosition.ToInt());
    player.mFirstName = mFirstName;
    player.mLastName = mLastName;
    player.mPseudonym = mPseudonym;
    player.mNickname = mNickname;
    player.mBirthday = mBirthdate;
    player.mNationality = mNationality;
    player.mFavouriteClub = mFavouriteClub;
    player.mWouldnSignFor = mWouldNeverWorkForClub;
    player.mManagerFavouritePlayer = mFavouritePlayer;
    player.mManagerFavouriteFormation = mFavouriteFormation;
    player.mChairmanStability = mChairmanStability;
    player.mCharacter.SetFromInt(mManagerFocus.ToInt());
    player.mManagerMotivationSkills = mManagerMotivationSkills;
    player.mManagerCoachingSkills = mManagerCoachingSkills;
    player.mManagerGoalkeepersTraining = mManagerGoalkeepersTraining;
    player.mManagerNegotiationSkills = mManagerNegotiationSkills;
    player.mLanguages = mLanguage;
    player.mTalent = mTalent;
    player.mContract.mJoined = mJoinedClubDate;
    player.mMainPosition = FifamPlayerPosition::GK;
    player.mAttributes.BallControl = 40;
    player.mAttributes.Volleys = 40;
    player.mAttributes.Dribbling = 40;
    player.mAttributes.Finishing = 40;
    player.mAttributes.ShotPower = 40;
    player.mAttributes.LongShots = 40;
    player.mAttributes.Crossing = 40;
    player.mAttributes.Passing = 40;
    player.mAttributes.LongPassing = 40;
    player.mAttributes.Header = 40;
    player.mAttributes.TacklingGeneral = 40;
    player.mAttributes.TackleStanding = 40;
    player.mAttributes.TackleSliding = 40;
    player.mAttributes.ManMarking = 40;
    player.mAttributes.Technique = 40;
    player.mAttributes.Creativity = 40;
    player.mAttributes.Flair = 40;
    player.mAttributes.Touch = 40;
    player.mAttributes.Balance = 40;
    player.mAttributes.Acceleration = 40;
    player.mAttributes.Pace = 40;
    player.mAttributes.Agility = 40;
    player.mAttributes.Jumping = 40;
    player.mAttributes.Strength = 40;
    player.mAttributes.Stamina = 40;
    player.mAttributes.WorkRate = 40;
    player.mAttributes.ForwardRuns = 40;
    player.mAttributes.PosOffensive = 40;
    player.mAttributes.PosDefensive = 40;
    player.mAttributes.Vision = 40;
    player.mAttributes.Reactions = 40;
    player.mAttributes.TacticAwareness = 40;
    player.mAttributes.Aggression = 40;
    player.mAttributes.Composure = 40;
    player.mAttributes.Consistency = 40;
    player.mAttributes.Leadership = 40;
    player.mAttributes.Anticipation = 40;
    player.mAttributes.Concentration = 40;
    player.mAttributes.Decision = 40;
    player.mAttributes.Determination = 40;
    player.mAttributes.TeamWork = 40;
    player.mAttributes.Intimidation = 40;
    player.mAttributes.FreeKicks = 40;
    player.mAttributes.Corners = 40;
    player.mAttributes.PenaltyShot = 40;
    player.mAttributes.Diving = 40;
    player.mAttributes.Reflexes = 40;
    player.mAttributes.Handling = 40;
    player.mAttributes.Positioning = 40;
    player.mAttributes.OneOnOne = 40;
    player.mAttributes.Kicking = 40;
    player.mAttributes.Punching = 40;
    player.mAttributes.ShotStopping = 40;
    player.mAttributes.Throwing = 40;
    player.mAttributes.GkCrosses = 40;
    player.mTalent = 100;
    player.mHeight = 180;
    player.mWeight = 75;
    player.Write(writer, database);
}

UChar FifamStaff::GetLevel() {
    return 0;
}

UChar FifamStaff::GetLevel(FifamClubStaffPosition position) {
    return 0;
}
