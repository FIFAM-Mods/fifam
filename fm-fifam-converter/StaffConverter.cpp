#include "Converter.h"
#include "ConverterUtil.h"

UChar GetManagerLevelFromCA(Int ca) {
    static Pair<UChar, UChar> managerCAtoLvlAry[15] = {
        { 15,  185 },
        { 14,  170 },
        { 13,  160 },
        { 12,  151 },
        { 11,  140 },
        { 10,  130 },
        { 9,   120 },
        { 8,   110 },
        { 7,   100 },
        { 6,    90 },
        { 5,    80 },
        { 4,    70 },
        { 3,    50 },
        { 2,    25 },
        { 1,     0 }
    };
    for (UInt i = 0; i < 15; i++) {
        if (ca >= managerCAtoLvlAry[i].second)
            return managerCAtoLvlAry[i].first;
    }
    return 1;
}

UChar GetStaffLevelFromCA(Int ca) {
    static Pair<UChar, UChar> staffCAtoLvlAry[99] = {
        { 99, 190 },
        { 98, 185 },
        { 97, 180 },
        { 96, 175 },
        { 95, 170 },
        { 94, 167 },
        { 93, 164 },
        { 92, 161 },
        { 91, 158 },
        { 90, 155 },
        { 89, 152 },
        { 88, 149 },
        { 87, 146 },
        { 86, 143 },
        { 85, 140 },
        { 84, 138 },
        { 83, 136 },
        { 82, 134 },
        { 81, 132 },
        { 80, 130 },
        { 79, 128 },
        { 78, 126 },
        { 77, 124 },
        { 76, 122 },
        { 75, 120 },
        { 74, 118 },
        { 73, 116 },
        { 72, 114 },
        { 71, 112 },
        { 70, 110 },
        { 69, 108 },
        { 68, 106 },
        { 67, 104 },
        { 66, 102 },
        { 65, 100 },
        { 64,  98 },
        { 63,  96 },
        { 62,  94 },
        { 61,  92 },
        { 60,  90 },
        { 59,  88 },
        { 58,  86 },
        { 57,  84 },
        { 56,  82 },
        { 55,  80 },
        { 54,  78 },
        { 53,  76 },
        { 52,  74 },
        { 51,  72 },
        { 50,  70 },
        { 49,  68 },
        { 48,  66 },
        { 47,  64 },
        { 46,  62 },
        { 45,  60 },
        { 44,  58 },
        { 43,  56 },
        { 42,  54 },
        { 41,  52 },
        { 40,  50 },
        { 39,  48 },
        { 38,  46 },
        { 37,  44 },
        { 36,  42 },
        { 35,  40 },
        { 34,  38 },
        { 33,  36 },
        { 32,  34 },
        { 31,  32 },
        { 30,  30 },
        { 29,  29 },
        { 28,  28 },
        { 27,  27 },
        { 26,  26 },
        { 25,  25 },
        { 24,  24 },
        { 23,  23 },
        { 22,  22 },
        { 21,  21 },
        { 20,  20 },
        { 19,  19 },
        { 18,  18 },
        { 17,  17 },
        { 16,  16 },
        { 15,  15 },
        { 14,  14 },
        { 13,  13 },
        { 12,  12 },
        { 11,  11 },
        { 10,  10 },
        { 9,    9 },
        { 8,    8 },
        { 7,    7 },
        { 6,    6 },
        { 5,    5 },
        { 4,    4 },
        { 3,    3 },
        { 2,    2 },
        { 1,    1 }
    };
    for (UInt i = 0; i < 99; i++) {
        if (ca >= staffCAtoLvlAry[i].second)
            return staffCAtoLvlAry[i].first;
    }
    return 1;
}

Int ConvertStaffAttribute_20_15(Int attr) {
    if (attr <= 0)
        attr = 1;
    if (attr > 20)
        attr = 20;
    static UChar fmRatingAry[20] = {
        1, // 1
        2, // 2
        3, // 3
        4, // 4
        4, // 5
        5, // 6
        5, // 7
        6, // 8
        6, // 9
        7, // 10
        8, // 11
        9, // 12
        10, // 13
        11, // 14
        12, // 15
        13, // 16
        14, // 17
        14, // 18
        15, // 19
        15, // 20
    };
    return fmRatingAry[attr - 1];
}

Int ConvertStaffAttribute_20_100(Int attr) {
    if (attr <= 0)
        attr = 1;
    if (attr > 20)
        attr = 20;
    static Pair<Int, Int> fmRatingAry[20] = {
        { 5, 29 }, // 1
        { 30, 39 }, // 2
        { 40, 49 }, // 3
        { 50, 54 }, // 4
        { 55, 59 }, // 5
        { 60, 62 }, // 6
        { 63, 64 }, // 7
        { 64, 65 }, // 8
        { 66, 67 }, // 9
        { 68, 69 }, // 10
        { 70, 71 }, // 11
        { 72, 73 }, // 12
        { 74, 76 }, // 13
        { 77, 79 }, // 14
        { 82, 83 }, // 15
        { 84, 85 }, // 16
        { 86, 88 }, // 17
        { 89, 91 }, // 18
        { 92, 95 }, // 19
        { 96, 99 }  // 20
    };
    if (fmRatingAry[attr - 1].first == fmRatingAry[attr - 1].second)
        return fmRatingAry[attr - 1].first;
    return Random::Get(fmRatingAry[attr - 1].first, fmRatingAry[attr - 1].second);
}

Int ConvertStaffAttribute(Int attr) {
    return ConvertStaffAttribute_20_100(OriginalAttrValue(attr));
}

Int ConvertStaffAttribute_100_15(Int attr) {
    return ConvertStaffAttribute_20_15(OriginalAttrValue(attr));
}

FifamStaff *Converter::CreateAndConvertStaff(foom::non_player * p, FifamClub * club, FifamClubStaffPosition position) {
    if (!p->mNation) {
        Error(L"Staff without nation\nSaffId: %d\nStaffName: %s", p->mID, p->mFullName.c_str());
        return nullptr;
    }
    FifamCountry *staffCountry = mFifamDatabase->GetCountry(p->mNation->mConverterData.mFIFAManagerReplacementID);
    if (!staffCountry) {
        Error(L"Staff without associated country\nSaffId: %d\nStaffName: %s", p->mID, p->mFullName.c_str());
        return nullptr;
    }
    FifamStaff *staff = mFifamDatabase->CreateStaff(club, mPersonIdCounter++);

    ConvertPersonAttributes(staff, p);
    staff->SetProperty(L"foom::non_player", p);

    staff->mLinkedCountry = staff->mNationality[0];

    // experience
    UInt age = staff->GetAge(GetCurrentDate());
    if (age >= 60)
        staff->mExperience = 4;
    else if (age >= 50)
        staff->mExperience = 3;
    else if (age >= 40)
        staff->mExperience = 2;
    else if (age >= 30)
        staff->mExperience = 1;
    else
        staff->mExperience = 0;

    // contract
    if (club) {
        if (club->mIsNationalTeam) {
            if (p->mNationContract.mDateJoined > FmEmptyDate())
                staff->mJoinedClubDate = p->mNationContract.mDateJoined;
            else
                staff->mJoinedClubDate.Set(1, 7, CURRENT_YEAR);
        }
        else {
            if (p->mClubContract.mDateJoined > FmEmptyDate())
                staff->mJoinedClubDate = p->mClubContract.mDateJoined;
            else
                staff->mJoinedClubDate.Set(1, 7, CURRENT_YEAR);
        }

    }
    else
        staff->mJoinedClubDate.Set(1, 7, CURRENT_YEAR);

    if (staff->mJoinedClubDate.year > CURRENT_YEAR)
        staff->mJoinedClubDate.year = CURRENT_YEAR;

    staff->mClubPosition = position;

    if (staff->mClubPosition == FifamClubStaffPosition::Manager)
        staff->mPersonType = FifamPersonType::Manager;
    else if (staff->mClubPosition == FifamClubStaffPosition::ChiefExec)
        staff->mPersonType = FifamPersonType::Chairman;
    else if (staff->mClubPosition == FifamClubStaffPosition::President)
        staff->mPersonType = FifamPersonType::President;
    else
        staff->mPersonType = FifamPersonType::Staff;

    // talent
    Int potential = p->mPotentialAbility;
    if (potential == 180 && p->mOriginalPA != 180)
        potential = p->mCurrentAbility;

    //static UChar potantialAbilityRanges[9] = { 35, 55, 75, 100, 115, 130, 145, 160, 175 };
    static UChar potantialAbilityRanges[9] = { 30, 50, 70, 90, 110, 130, 140, 160, 180 };
    staff->mTalent = 0;
    for (UInt i = 0; i < 9; i++) {
        if (potential >= potantialAbilityRanges[9 - 1 - i]) {
            staff->mTalent = 9 - i;
            break;
        }
    }
    // stability of board of directors
    if (p->mPatience >= 18)
        staff->mChairmanStability = FifamChairmanStability::LongTermPlaning;
    else if (p->mPatience >= 15)
        staff->mChairmanStability = FifamChairmanStability::Patient;
    else if (p->mPatience >= 7)
        staff->mChairmanStability = FifamChairmanStability::Average;
    else if (p->mPatience >= 4)
        staff->mChairmanStability = FifamChairmanStability::Impatient;
    else
        staff->mChairmanStability = FifamChairmanStability::Insane;

    // formation
    if (p->mPreferredFormation > 0)
        staff->mManagerFavouriteFormation = ConvertFormationId(p->mPreferredFormation);
    else
        staff->mManagerFavouriteFormation = FifamFormation::None;

    Int customFormation = ConvertFormationIdToCustom(p->mPreferredFormation);
    if (customFormation != 0)
        staff->SetProperty<Int>(L"custom_formation", customFormation);

    // preferred orientation
    if (p->mExpectAttackingFootball && (!p->mWillLookToPlayOutOfDefence || (p->mExpectAttackingFootball / 3) > p->mWillLookToPlayOutOfDefence)) {
        if (p->mExpectAttackingFootball >= 15)
            staff->mCoachPlayingOrientation = FifamCoachPlayingOrientation::VeryOffensive;
        else
            staff->mCoachPlayingOrientation = FifamCoachPlayingOrientation::Offensive;
    }
    else if (p->mWillLookToPlayOutOfDefence && (!p->mExpectAttackingFootball || (p->mWillLookToPlayOutOfDefence / 3) > p->mExpectAttackingFootball)) {
        if (p->mWillLookToPlayOutOfDefence >= 15)
            staff->mCoachPlayingOrientation = FifamCoachPlayingOrientation::VeryDefensive;
        else
            staff->mCoachPlayingOrientation = FifamCoachPlayingOrientation::Defensive;
    }
    else
        staff->mCoachPlayingOrientation = FifamCoachPlayingOrientation::Normal;

    // coaching style
    Int bestAttacking = Utils::Min(p->mAttacking, p->mExpectAttackingFootball) * 5;
    Int bestDefending = p->mWillLookToPlayOutOfDefence * 5;
    Int bestTactical = BestFrom_Avg<Int>({ p->mTacticalKnowledge, p->mWillMakeEarlyTacticalChanges * 5, p->mWillFitPlayersIntoPreferredTactic * 5 }, 1);
    Int bestYouth = BestFrom_Avg<Int>({ p->mSignsYoungPlayersForTheFirstTeam, p->mSignsALotOfYouthPlayers, p->mExpectYoungSigningsForTheFirstTeam }, 1) * 5;
    auto bestStyle = Utils::GetMaxElementId<Int, FifamManagerFocus>({
        { bestAttacking > bestDefending ? (bestAttacking - bestDefending) : 0, FifamManagerFocus::Offensive },
        { bestDefending > bestAttacking ? (bestDefending - bestAttacking) : 0, FifamManagerFocus::Defensive },
        { bestTactical, FifamManagerFocus::TacticalEducation },
        { p->mLevelOfDiscipline * 5, FifamManagerFocus::Discipline },
        { (p->mCoachingMental + p->mMotivating) / 2, FifamManagerFocus::PlayerCharacter },
        { p->mCoachingFitness * 5, FifamManagerFocus::Fitness },
        { bestYouth, FifamManagerFocus::YoungPlayers }
        });

    if (bestStyle.first > 50)
        staff->mManagerFocus = bestStyle.second;
    else {
        // UPDATE: update this if languages count was changed
        if (staff->mLanguages[3] != FifamLanguage::None)
            staff->mManagerFocus = FifamManagerFocus::Language;
        else {
            Int randVal = Random::Get(0, 100);
            if (randVal < 35)
                staff->mManagerFocus = FifamManagerFocus::None;
            else if (randVal < 45)
                staff->mManagerFocus = FifamManagerFocus::ExperiencedPlayers;
            else if (randVal < 55)
                staff->mManagerFocus = FifamManagerFocus::KeepsTeamTogether;
            else if (randVal < 65)
                staff->mManagerFocus = FifamManagerFocus::LongContracts;
            else
                staff->mManagerFocus.SetFromInt(Random::Get(1, 11));
        }
    }

    // manager skills
    UInt overallSkillPart = 0;
    if (p->mCurrentAbility >= 170)
        overallSkillPart = 8;
    else if (p->mCurrentAbility >= 160)
        overallSkillPart = 7;
    else if (p->mCurrentAbility >= 150)
        overallSkillPart = 6;
    else if (p->mCurrentAbility >= 140)
        overallSkillPart = 5;
    else if (p->mCurrentAbility >= 130)
        overallSkillPart = 4;
    else if (p->mCurrentAbility >= 120)
        overallSkillPart = 3;
    else if (p->mCurrentAbility >= 110)
        overallSkillPart = 2;
    else if (p->mCurrentAbility >= 100)
        overallSkillPart = 1;

    UChar staffLevel = (UChar)ConvertStaffAttribute(p->mCurrentAbility / 2);
    Int staffFitness = Utils::Clamp((Int)((Float)p->mCoachingFitness * (Float)p->mJobFitnessCoach / 20.0f), 1, 20) * 5;

    // manager skills
    auto motivationSkillPart = Utils::Clamp(ConvertStaffAttribute_100_15(p->mMotivating) / 2, 1, 7);
    staff->mManagerMotivationSkills = overallSkillPart + motivationSkillPart;

    Vector<Int> coachingSkills = {
        p->mCoachingAttacking, p->mCoachingDefending, staffFitness, p->mCoachingMental, p->mCoachingTactical, p->mCoachingTechnical
    };
    std::sort(coachingSkills.begin(), coachingSkills.end(), std::greater<Int>());
    auto coachingSkillsPart = Utils::Clamp(ConvertStaffAttribute_100_15((coachingSkills[0] + coachingSkills[1] + coachingSkills[2]) / 3) / 2, 1, 7);
    staff->mManagerCoachingSkills = overallSkillPart + coachingSkillsPart;

    auto gkSkillPart = Utils::GetMaxElement<Int>({ p->mCoachingGKDistribution, p->mCoachingGKHandling, p->mCoachingGKShotStopping });
    gkSkillPart = Utils::Clamp(ConvertStaffAttribute_100_15(gkSkillPart) / 2, 1, 7);
    staff->mManagerGoalkeepersTraining = overallSkillPart + gkSkillPart;

    auto negotiationSkillPart = Utils::GetMaxElement<Int>({ p->mBuyingPlayers, p->mBusiness });
    negotiationSkillPart = Utils::Clamp(ConvertStaffAttribute_20_15(negotiationSkillPart) / 2, 1, 7);
    staff->mManagerNegotiationSkills = overallSkillPart + negotiationSkillPart;

    // fix overall rating
    Int  overallRating = (staff->mManagerMotivationSkills + staff->mManagerCoachingSkills + staff->mManagerNegotiationSkills) / 3;
    if (overallRating > 1 && (staff->mManagerGoalkeepersTraining + 1) < overallRating) {
        staff->mManagerGoalkeepersTraining = overallRating - 1;
    }

    overallRating = staff->GetManagerLevel();
    Int desiredOverall = GetManagerLevelFromCA(p->mCurrentAbility);
    Bool dir = desiredOverall > overallRating;
    while ((dir && overallRating < desiredOverall) || (!dir && overallRating > desiredOverall)) {
        Int ratingDiff = dir ? 1 : -1;
        staff->mManagerMotivationSkills = Utils::Clamp(staff->mManagerMotivationSkills + ratingDiff, 1, 15);
        staff->mManagerCoachingSkills = Utils::Clamp(staff->mManagerCoachingSkills + ratingDiff, 1, 15);
        staff->mManagerNegotiationSkills = Utils::Clamp(staff->mManagerNegotiationSkills + ratingDiff, 1, 15);
        staff->mManagerGoalkeepersTraining = Utils::Clamp(staff->mManagerGoalkeepersTraining + ratingDiff, 1, 15);
        overallRating = staff->GetManagerLevel();
    }

    // personality attributes
    staff->mPersonalityAttributes.WillingnessToLearn = ConvertStaffAttribute_20_100(p->mAmbition);
    staff->mPersonalityAttributes.Effort = ConvertStaffAttribute(p->mDetermination);
    staff->mPersonalityAttributes.Resilence = ConvertStaffAttribute(p->mVersatility);

    staff->mSkills.Tactics = ConvertStaffAttribute(p->mTacticalKnowledge);

    staff->mSkills.FieldPlayerTraining = ConvertStaffAttribute(
        BestFrom_Avg<Int>(
            { p->mCoachingAttacking, p->mCoachingDefending, p->mCoachingMental, p->mCoachingTactical, p->mCoachingTechnical }, 2, true)
    );

    staff->mSkills.GoalkeeperTraining = ConvertStaffAttribute(
        BestFrom_Avg<Int>(
            { p->mCoachingGKDistribution, p->mCoachingGKHandling, p->mCoachingGKShotStopping }, 3, true)
    );

    staff->mSkills.FitnessTraining = Utils::Clamp((Int)((Float)ConvertStaffAttribute(staffFitness) * (Float)staffLevel * 1.1f / 100.0f), 1, 99);
    staff->mSkills.MotivationAbility = ConvertStaffAttribute(p->mMotivating);

    staff->mSkills.RegenerationAbility = ConvertStaffAttribute(p->mPhysiotherapy);

    staff->mSkills.InjuryPrevention = ConvertStaffAttribute(p->mSportsScience);
    staff->mSkills.BoneInjury = Utils::Clamp(staff->mSkills.InjuryPrevention - 10 + Random::Get(0, 20), 5, 99);
    staff->mSkills.KneeInjury = Utils::Clamp(staff->mSkills.InjuryPrevention - 10 + Random::Get(0, 20), 5, 99);
    staff->mSkills.MuscleInjury = Utils::Clamp(staff->mSkills.InjuryPrevention - 10 + Random::Get(0, 20), 5, 99);

    staff->mSkills.Negotiation = ConvertStaffAttribute(p->mBuyingPlayers);
    staff->mSkills.Sponsoring = ConvertStaffAttribute(p->mBusiness);

    staff->mSkills.GeneralScouting = ConvertStaffAttribute(Utils::Max(p->mJudgingPlayerAbility, p->mJudgingTeamData));
    staff->mSkills.TalentEstimation = ConvertStaffAttribute(p->mJudgingPlayerPotential);

    staff->mSkills.FieldSkillsEstimation = Utils::Clamp(staff->mSkills.GeneralScouting - 10 + Random::Get(0, 20), 5, 99);
    staff->mSkills.GoalkeeperSkillsEstimation = Utils::Clamp(staff->mSkills.GeneralScouting - 10 + Random::Get(0, 20), 5, 99);
    staff->mSkills.MentalSkillsEstimation = Utils::Clamp(staff->mSkills.GeneralScouting - 10 + Random::Get(0, 20), 5, 99);
    staff->mSkills.PhysicalSkillsEstimation = Utils::Clamp(staff->mSkills.GeneralScouting - 10 + Random::Get(0, 20), 5, 99);
    staff->mSkills.Networking = ConvertStaffAttribute(
        BestFrom_Avg<Int>(
            { p->mPresentingData, p->mJudgingPlayerData, p->mJudgingTeamData }, 1, true)
    );

    if (staff->mClubPosition == FifamClubStaffPosition::SportsDirector) {
        staff->mSkills.SportsLaw = Random::Get(10, 25);
        staff->mSkills.LaborLaw = Random::Get(10, 25);
        staff->mSkills.PR = Random::Get(10, 25);
        staff->mSkills.FanContact = Random::Get(10, 25);
        staff->mSkills.Arbitrate = Random::Get(10, 25);
        staff->mSkills.Marketing = Random::Get(10, 25);
        staff->mSkills.Construction = Random::Get(10, 25);
    }
    else if (staff->mClubPosition == FifamClubStaffPosition::GeneralManager) {
        staff->mSkills.SportsLaw = Random::Get(15, 30);
        staff->mSkills.LaborLaw = Random::Get(15, 30);
        staff->mSkills.PR = Random::Get(15, 30);
        staff->mSkills.FanContact = Random::Get(15, 30);
        staff->mSkills.Arbitrate = Random::Get(15, 30);
        staff->mSkills.Marketing = Random::Get(15, 30);
        staff->mSkills.Construction = Random::Get(15, 30);
    }
    else {
        staff->mSkills.SportsLaw = Random::Get(5, 20);
        staff->mSkills.LaborLaw = Random::Get(5, 20);
        staff->mSkills.PR = Random::Get(5, 20);
        staff->mSkills.FanContact = Random::Get(5, 20);
        staff->mSkills.Arbitrate = Random::Get(5, 20);
        staff->mSkills.Marketing = Random::Get(5, 20);
        staff->mSkills.Construction = Random::Get(5, 20);
    }

    staff->ForAllAttributes([=](UChar & attr, Float weight) {
        float c = 0.0f;
        if (weight >= 1.0f)
            c = 1.0f;
        else if (weight >= 0.75f) {
            if (staff->mClubPosition == FifamClubStaffPosition::TeamDoctor)
                c = 1.0f;
            else
                c = 0.95f;
        }
        else if (weight >= 0.5f) {
            if (staff->mClubPosition == FifamClubStaffPosition::TeamDoctor)
                c = 1.0f;
            else
                c = 0.9f;
        }
        else if (weight >= 0.25f)
            c = 0.8f;
        if (c > 0.0f) {
            Int newAttrValue = Utils::Clamp((Int)((Float)staffLevel * c) + 10 - Random::Get(0, 20), 1, 99);
            if (newAttrValue > (Int)attr)
                attr = (UChar)newAttrValue;
        }
        });

    Int staffCurrLevel = staff->GetStaffLevel();
    Int staffDesiredLevel = GetStaffLevelFromCA(p->mCurrentAbility);
    if (staffCurrLevel > 0 && staffCurrLevel != staffDesiredLevel) {
        Bool dir = staffDesiredLevel > staffCurrLevel;
        while ((dir && staffCurrLevel < staffDesiredLevel) || (!dir && staffCurrLevel > staffDesiredLevel)) {
            staff->ForAllAttributes([=](UChar & attr, Float weight) {
                if (weight > 0.0f) {
                    if (dir) {
                        if (attr < 99)
                            attr++;
                    }
                    else {
                        if (attr > 1)
                            attr--;
                    }
                }
                });
            staffCurrLevel = staff->GetStaffLevel();
        }
    }

    return staff;
}

void Converter::CreateStaffMembersForClub(UInt gameId, foom::team *team, FifamClub *dst, Bool isNationalTeam) {
    std::sort(team->mVecContractedNonPlayers.begin(), team->mVecContractedNonPlayers.end(),
        [](foom::non_player * a, foom::non_player * b) {
            return a->mCurrentAbility > b->mCurrentAbility;
        });

    foom::non_player *staffManager = nullptr;
    foom::non_player *staffOwner = nullptr;
    foom::non_player *staffChairman = nullptr;
    foom::non_player *staffDirector = nullptr;
    foom::non_player *staffManagingDirector = nullptr;
    foom::non_player *staffSportsDirector = nullptr;
    foom::non_player *staffReserveCoach = nullptr;
    foom::non_player *staffYouthCoach = nullptr;
    foom::non_player *staffGoalkeeperCoach = nullptr;
    foom::non_player *staffFitnessCoach = nullptr;
    foom::non_player *staffTeamDoctor = nullptr;
    foom::non_player *staffPhysio = nullptr;

    Vector<foom::non_player *> staffAssistantCoaches;
    Vector<foom::non_player *> staffCoaches;
    Vector<foom::non_player *> staffChiefScouts;
    Vector<foom::non_player *> staffScouts;

    UInt maxAssistantCoaches = 1;
    UInt maxScouts = 1;

    UChar maxReserveCoachAgeGroup = 0;
    UChar maxYouthCoachAgeGroup = 0;

    Bool hasHeadPhysio = false;
    Bool hasChiefDoctor = false;
    Bool hasOwner = false;

    if (gameId >= 9) {
        maxAssistantCoaches = 2;
        maxScouts = 6;
    }

    // reserve coach
    if (!isNationalTeam) {
        foom::club *reserveClub = ((foom::club *)team)->mConverterData.mMainChildClubInDB;
        if (reserveClub) {
            for (auto &p : reserveClub->mVecContractedNonPlayers) {
                if (IsConvertable(p, gameId) && p->mClubContract.mJob == 5) {
                    staffReserveCoach = p;
                    maxReserveCoachAgeGroup = 23;
                    break;
                }
            }
        }
    }

    for (auto &p : team->mVecContractedNonPlayers) {
        Int j = -1;
        if (isNationalTeam) {
            if (p->mClubContract.mClub)
                continue;
            j = p->mNationContract.mJob;
        }
        else
            j = p->mClubContract.mJob;

        if (j == 157) { // owner
            if (!hasOwner) {
                staffOwner = p;
                hasOwner = IsConvertable(p, gameId);
            }
        }
        else if (IsConvertable(p, gameId)) {
            switch (j) {
            case 5: // used as manager
                if (!staffManager)
                    staffManager = p;
                break;
            case 1: // used as president or chief-exec
                if (!staffChairman)
                    staffChairman = p;
                break;
            case 38: // used as chief-exec
                if (!staffDirector)
                    staffDirector = p;
                break;
            case 2: // used as general manager
                if (!staffManagingDirector)
                    staffManagingDirector = p;
                break;
            case 4: // used as director of football (sports director)
                if (!staffSportsDirector)
                    staffSportsDirector = p;
                break;
            case 6: // assistant manager first team
                staffAssistantCoaches.push_back(p);
                break;
            case 8: // coach first team
                staffCoaches.push_back(p);
                break;
            case 34: // chief scout
                staffChiefScouts.push_back(p);
                break;
            case 9: // scout
                staffScouts.push_back(p);
                break;
            case 26: // gk coach
                if (!staffGoalkeeperCoach)
                    staffGoalkeeperCoach = p;
                break;
            case 55: // fitness coach
                if (!staffFitnessCoach)
                    staffFitnessCoach = p;
                break;
            case 36: // head physio
                if (!hasHeadPhysio) {
                    staffPhysio = p;
                    hasHeadPhysio = true;
                }
                break;
            case 10: // physio
                if (!staffPhysio)
                    staffPhysio = p;
                break;
            case 172: // chief doctor
                if (!hasChiefDoctor) {
                    staffTeamDoctor = p;
                    hasChiefDoctor = true;
                }
                break;
            case 178: // doctor
                if (!staffTeamDoctor)
                    staffTeamDoctor = p;
                break;
            case 101: // U23 manager
                if (!staffReserveCoach || maxReserveCoachAgeGroup < 23) {
                    staffReserveCoach = p;
                    maxReserveCoachAgeGroup = 23;
                }
                break;
            case 107: // U21 manager
                if (!staffReserveCoach || maxReserveCoachAgeGroup < 21) {
                    staffReserveCoach = p;
                    maxReserveCoachAgeGroup = 21;
                }
                break;
            case 113: // U20 manager
                if (!staffReserveCoach || maxReserveCoachAgeGroup < 20) {
                    staffReserveCoach = p;
                    maxReserveCoachAgeGroup = 20;
                }
                break;
            case 119: // U19 manager
                if (!staffYouthCoach || maxYouthCoachAgeGroup < 19) {
                    staffYouthCoach = p;
                    maxYouthCoachAgeGroup = 19;
                }
                break;
            case 24: // U18 manager
                if (!staffYouthCoach || maxYouthCoachAgeGroup < 18) {
                    staffYouthCoach = p;
                    maxYouthCoachAgeGroup = 18;
                }
                break;
            }
        }
    }

    if (!staffCoaches.empty())
        staffAssistantCoaches.insert(staffAssistantCoaches.end(), staffCoaches.begin(), staffCoaches.end());
    if (!staffScouts.empty())
        staffChiefScouts.insert(staffChiefScouts.end(), staffScouts.begin(), staffScouts.end());

    if (staffManager)
        CreateAndConvertStaff(staffManager, dst, FifamClubStaffPosition::Manager);
    if (staffOwner) {
        if (IsConvertable(staffOwner, gameId))
            CreateAndConvertStaff(staffOwner, dst, FifamClubStaffPosition::President);
        if (staffChairman)
            CreateAndConvertStaff(staffChairman, dst, FifamClubStaffPosition::ChiefExec);
        else if (staffDirector)
            CreateAndConvertStaff(staffDirector, dst, FifamClubStaffPosition::ChiefExec);
    }
    else {
        if (staffChairman)
            CreateAndConvertStaff(staffChairman, dst, FifamClubStaffPosition::President);
        if (staffDirector)
            CreateAndConvertStaff(staffDirector, dst, FifamClubStaffPosition::ChiefExec);
    }
    if (staffManagingDirector)
        CreateAndConvertStaff(staffManagingDirector, dst, FifamClubStaffPosition::GeneralManager);
    if (gameId >= 9) {
        if (staffSportsDirector)
            CreateAndConvertStaff(staffSportsDirector, dst, FifamClubStaffPosition::SportsDirector);
    }
    for (UInt i = 0; i < Utils::Min(maxAssistantCoaches, staffAssistantCoaches.size()); i++)
        CreateAndConvertStaff(staffAssistantCoaches[i], dst, FifamClubStaffPosition::AssistantCoach);
    for (UInt i = 0; i < Utils::Min(maxScouts, staffChiefScouts.size()); i++)
        CreateAndConvertStaff(staffChiefScouts[i], dst, FifamClubStaffPosition::GeneralScout);
    if (staffReserveCoach)
        CreateAndConvertStaff(staffReserveCoach, dst, FifamClubStaffPosition::AmateurCoach);
    if (staffYouthCoach)
        CreateAndConvertStaff(staffYouthCoach, dst, FifamClubStaffPosition::YouthCoach);
    if (staffGoalkeeperCoach)
        CreateAndConvertStaff(staffGoalkeeperCoach, dst, FifamClubStaffPosition::GoalkeeperCoach);
    if (gameId >= 9) {
        if (staffFitnessCoach)
            CreateAndConvertStaff(staffFitnessCoach, dst, FifamClubStaffPosition::FitnessCoach);
    }
    if (staffPhysio)
        CreateAndConvertStaff(staffPhysio, dst, FifamClubStaffPosition::Masseur);
    if (staffTeamDoctor)
        CreateAndConvertStaff(staffTeamDoctor, dst, FifamClubStaffPosition::TeamDoctor);
}
