#include "Converter.h"
#include "ConverterUtil.h"

//#define WRITE_STAFF_DEBUG

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

void SetStaffLevel(FifamStaff *staff, FifamClubStaffPosition position, Int level, Bool decreaseOnlyImportantSkills = false,
    Set<FifamStaffSkillID> skillsToChange = Set<FifamStaffSkillID>())
{
    Int currLevel = staff->GetStaffLevel(position);
    if (currLevel != level) {
        Int iterations = 0;
        Bool increase = level > currLevel;
        FifamStaff::Skills savedSkills;
        while (true) {
            iterations++;
            if (increase)
                savedSkills = staff->mSkills;
            staff->ForAllSkills(position, [=](FifamStaffSkillID skillID, UChar &value, Float weight) {
                if (skillsToChange.empty() || Utils::Contains(skillsToChange, skillID)) {
                    if (increase) {
                        if (weight > 0.0f)
                            value = Utils::Clamp((Int)value + roundf((Float)iterations * weight), 1, 99);
                    }
                    else {
                        if (!decreaseOnlyImportantSkills || weight > 0.0f)
                            value = Utils::Clamp(value - 1, 1, 99);
                    }
                }
            });
            currLevel = staff->GetStaffLevel(position);
            if (iterations == 100 || (increase && currLevel >= level) || (!increase && currLevel <= level))
                break;
            if (increase)
                staff->mSkills = savedSkills;
        }
    }
}

FifamStaff *Converter::CreateAndConvertStaff(foom::non_player * p, FifamClub * club, FifamClubStaffPosition position, UInt gameId) {
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

    ConvertPersonAttributes(staff, p, gameId);
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

    staff->SetProperty<UInt>(L"foom::PA", potential);
    staff->SetProperty<UInt>(L"foom::CA", p->mCurrentAbility);

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
    // custom formations are used
    //if (p->mPreferredFormation > 0)
    //    staff->mManagerFavouriteFormation = ConvertFormationId(p->mPreferredFormation);
    //else
        staff->mManagerFavouriteFormation = FifamFormation::None;

    Int customFormation = ConvertFormationIdToCustom(p->mPreferredFormation);
    if (customFormation != 0)
        staff->SetProperty<Int>(L"custom_formation", customFormation);

    UInt expectAttackingFootball = Utils::Max(p->mAttacking, p->mCoachingAttacking);
    // preferred orientation
    if (p->mAttacking && (!p->mWillLookToPlayOutOfDefence || (p->mAttacking / 3) > p->mWillLookToPlayOutOfDefence)) {
        if (p->mAttacking >= 15)
            staff->mCoachPlayingOrientation = FifamCoachPlayingOrientation::VeryOffensive;
        else
            staff->mCoachPlayingOrientation = FifamCoachPlayingOrientation::Offensive;
    }
    else if (p->mWillLookToPlayOutOfDefence && (!p->mAttacking || (p->mWillLookToPlayOutOfDefence / 3) > p->mAttacking)) {
        if (p->mWillLookToPlayOutOfDefence >= 15)
            staff->mCoachPlayingOrientation = FifamCoachPlayingOrientation::VeryDefensive;
        else
            staff->mCoachPlayingOrientation = FifamCoachPlayingOrientation::Defensive;
    }
    else
        staff->mCoachPlayingOrientation = FifamCoachPlayingOrientation::Normal;

    if (staff->mCoachPlayingOrientation == FifamCoachPlayingOrientation::Offensive) {
        UInt rnd;
        Int customFormation = staff->GetProperty<Int>(L"custom_formation", 0);
        if (customFormation > 0) {
            switch (customFormation) {
            case FMF_FORMATION_4_1_2_3_NARROW:
            case FMF_FORMATION_4_1_3_2_NARROW:
            case FMF_FORMATION_4_2_4_WIDE:
            case FMF_FORMATION_4_3_1_2_NARROW:
            case FMF_FORMATION_4_3_3_NARROW:
            case FMF_FORMATION_4_3_3_WIDE:
                staff->mCoachPlayingOrientation = FifamCoachPlayingOrientation::Offensive;
                break;

            case FMF_FORMATION_3_4_1_2:
            case FMF_FORMATION_3_4_2_1:
            case FMF_FORMATION_3_4_3:
            case FMF_FORMATION_3_4_3_DM_WIDE:
            case FMF_FORMATION_3_5_2:
            case FMF_FORMATION_3_5_2_CF:
            case FMF_FORMATION_5_1_2_2_DM_WB:
            case FMF_FORMATION_5_1_2_2_DM_WB_CF:
            case FMF_FORMATION_5_1_2_2_DM_WB_NARROW:
            case FMF_FORMATION_5_1_3_1_DM_WB:
            case FMF_FORMATION_5_2_1_2_WB:
            case FMF_FORMATION_5_2_2_1_WB:
            case FMF_FORMATION_5_2_3_NARROW:
            case FMF_FORMATION_5_3_2_WB:
            case FMF_FORMATION_5_3_2_WB_CF:
            case FMF_FORMATION_5_4_1:
            case FMF_FORMATION_5_4_1_DIAMOND_WB:
            case FMF_FORMATION_5_4_1_WB_WIDE:
                staff->mCoachPlayingOrientation = FifamCoachPlayingOrientation::Defensive;
                break;
            case FMF_FORMATION_5_2_2_1_DM:
            case FMF_FORMATION_5_2_2_1_DM_CF:
            case FMF_FORMATION_5_2_2_1_0_DM:
            case FMF_FORMATION_3_1_3_1_2_DM:
            case FMF_FORMATION_3_1_4_2_DM:
            case FMF_FORMATION_3_2_3_2_DM:
            case FMF_FORMATION_3_4_2_1_DM:
                staff->mCoachPlayingOrientation = FifamCoachPlayingOrientation::VeryDefensive;
                break;
            default:
                rnd = Random::Get(0, 100);
                //std::wcout << rnd << std::endl;
                if (rnd < 33)
                    staff->mCoachPlayingOrientation = FifamCoachPlayingOrientation::Normal;
                else if (rnd < 67)
                    staff->mCoachPlayingOrientation = FifamCoachPlayingOrientation::Defensive;
                else
                    staff->mCoachPlayingOrientation = FifamCoachPlayingOrientation::Offensive;
            }
        }
        else {
            rnd = Random::Get(0, 100);
            //std::wcout << rnd << std::endl;
            if (rnd < 33)
                staff->mCoachPlayingOrientation = FifamCoachPlayingOrientation::Normal;
            else if (rnd < 67)
                staff->mCoachPlayingOrientation = FifamCoachPlayingOrientation::Defensive;
            else
                staff->mCoachPlayingOrientation = FifamCoachPlayingOrientation::Offensive;
        }
    }

    // coaching style 
    // mCoachingStyle : 0 - not set, 1 - general (Simeone, Luis Enrique, Conte), 2 - goalkeeping, 3 - fitness, 4 - attacking, 5 - defending, 6 - technical, 7 - technical, 8 - mental, 9 - set pieces
    if (p->mCoachingStyle == 3) // fitness
        staff->mManagerFocus = FifamManagerFocus::Fitness;
    else if (p->mCoachingStyle == 4) // attacking (Fergusson, Klopp, Inzaghi)
        staff->mManagerFocus = FifamManagerFocus::Offensive;
    else if (p->mCoachingStyle == 5) // defending (Baresi, Deschamps, Maldini)
        staff->mManagerFocus = FifamManagerFocus::Defensive;
    else if (p->mCoachingStyle == 6 || p->mCoachingStyle == 7) // technical (Mourinho, Xavi, Sarri) / technical (Guardiola, Zidane, Arteta)
        staff->mManagerFocus = FifamManagerFocus::TacticalEducation;
    else if (p->mCoachingStyle == 8) // mental (Ancelotti)
        staff->mManagerFocus = FifamManagerFocus::PlayerCharacter;
    else {
        Int bestAttacking = p->mAttacking * 5;
        Int bestDefending = p->mWillLookToPlayOutOfDefence * 5;
        Int bestTactical = BestFrom_Avg<Int>({ p->mTacticalKnowledge, p->mWillMakeEarlyTacticalChanges * 5, p->mWillFitPlayersIntoPreferredTactic * 5 }, 1);
        Int bestYouth = BestFrom_Avg<Int>({ p->mSignsYoungPlayersForTheFirstTeam, p->mSignsALotOfYouthPlayers }, 1) * 5;
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

    UChar staffCA = (UChar)ConvertStaffAttribute(p->mCurrentAbility / 2);
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

    staff->mSkills.FitnessTraining = Utils::Clamp((Int)((Float)ConvertStaffAttribute(staffFitness) * (Float)staffCA * 1.1f / 100.0f), 1, 99);
    staff->mSkills.MotivationAbility = ConvertStaffAttribute(p->mMotivating);

    staff->mSkills.RegenerationAbility = ConvertStaffAttribute(p->mPhysiotherapy);

    staff->mSkills.InjuryPrevention = ConvertStaffAttribute(p->mSportsScience);
    staff->mSkills.BoneInjury = Utils::Clamp(staff->mSkills.InjuryPrevention - 10 + Random::Get(0, 20), 5, 99);
    staff->mSkills.KneeInjury = Utils::Clamp(staff->mSkills.InjuryPrevention - 10 + Random::Get(0, 20), 5, 99);
    staff->mSkills.MuscleInjury = Utils::Clamp(staff->mSkills.InjuryPrevention - 10 + Random::Get(0, 20), 5, 99);

    staff->mSkills.Negotiation = ConvertStaffAttribute(p->mBuyingPlayers);
    staff->mSkills.Sponsoring = ConvertStaffAttribute(p->mBusiness);

    staff->mSkills.GeneralScouting = ConvertStaffAttribute(Utils::Max(p->mJudgingPlayerAbility, p->mAnalysingData));
    staff->mSkills.TalentEstimation = ConvertStaffAttribute(p->mJudgingPlayerPotential);

    staff->mSkills.FieldSkillsEstimation = Utils::Clamp(staff->mSkills.GeneralScouting - 10 + Random::Get(0, 20), 5, 99);
    staff->mSkills.GoalkeeperSkillsEstimation = Utils::Clamp(staff->mSkills.GeneralScouting - 10 + Random::Get(0, 20), 5, 99);
    staff->mSkills.MentalSkillsEstimation = Utils::Clamp(staff->mSkills.GeneralScouting - 10 + Random::Get(0, 20), 5, 99);
    staff->mSkills.PhysicalSkillsEstimation = Utils::Clamp(staff->mSkills.GeneralScouting - 10 + Random::Get(0, 20), 5, 99);
    staff->mSkills.Networking = ConvertStaffAttribute(
        BestFrom_Avg<Int>(
            { p->mAnalysingData, p->mJudgingPlayerPotential, p->mJudgingPlayerAbility }, 1, true)
    );
    staff->mSkills.SportsLaw = 1;
    staff->mSkills.LaborLaw = 1;
    staff->mSkills.PR = 1;
    staff->mSkills.FanContact = 1;
    staff->mSkills.Arbitrate = 1;
    staff->mSkills.Marketing = 1;
    staff->mSkills.Construction = 1;

    static Map<FifamClubStaffPosition, Pair<Set<FifamClubStaffPosition>, Set<FifamClubStaffPosition>>> skillUpdateTable = {
        { FifamClubStaffPosition::AssistantCoach,      { { FifamClubStaffPosition::TeamDoctor, FifamClubStaffPosition::SpecialistBone, FifamClubStaffPosition::SpecialistKnee, FifamClubStaffPosition::SpecialistMuscle, FifamClubStaffPosition::Masseur, FifamClubStaffPosition::GeneralManager, FifamClubStaffPosition::MarketingManager, FifamClubStaffPosition::ConstructionManager, FifamClubStaffPosition::SportsDirector, FifamClubStaffPosition::FanRepresentative, FifamClubStaffPosition::Spokesperson, FifamClubStaffPosition::Lawyer, FifamClubStaffPosition::GeneralScout }, {  } } },
        { FifamClubStaffPosition::AmateurCoach,        { { FifamClubStaffPosition::TeamDoctor, FifamClubStaffPosition::SpecialistBone, FifamClubStaffPosition::SpecialistKnee, FifamClubStaffPosition::SpecialistMuscle, FifamClubStaffPosition::Masseur, FifamClubStaffPosition::GeneralManager, FifamClubStaffPosition::MarketingManager, FifamClubStaffPosition::ConstructionManager, FifamClubStaffPosition::SportsDirector, FifamClubStaffPosition::FanRepresentative, FifamClubStaffPosition::Spokesperson, FifamClubStaffPosition::Lawyer, FifamClubStaffPosition::GeneralScout }, {  } } },
        { FifamClubStaffPosition::YouthCoach,          { { FifamClubStaffPosition::TeamDoctor, FifamClubStaffPosition::SpecialistBone, FifamClubStaffPosition::SpecialistKnee, FifamClubStaffPosition::SpecialistMuscle, FifamClubStaffPosition::Masseur, FifamClubStaffPosition::GeneralManager, FifamClubStaffPosition::MarketingManager, FifamClubStaffPosition::ConstructionManager, FifamClubStaffPosition::SportsDirector, FifamClubStaffPosition::FanRepresentative, FifamClubStaffPosition::Spokesperson, FifamClubStaffPosition::Lawyer, FifamClubStaffPosition::GeneralScout }, {  } } },
        { FifamClubStaffPosition::FitnessCoach,        { { FifamClubStaffPosition::TeamDoctor, FifamClubStaffPosition::SpecialistBone, FifamClubStaffPosition::SpecialistKnee, FifamClubStaffPosition::SpecialistMuscle, FifamClubStaffPosition::Masseur, FifamClubStaffPosition::GeneralManager, FifamClubStaffPosition::MarketingManager, FifamClubStaffPosition::ConstructionManager, FifamClubStaffPosition::SportsDirector, FifamClubStaffPosition::FanRepresentative, FifamClubStaffPosition::Spokesperson, FifamClubStaffPosition::Lawyer, FifamClubStaffPosition::GeneralScout }, { FifamClubStaffPosition::AssistantCoach, FifamClubStaffPosition::AmateurCoach, FifamClubStaffPosition::YouthCoach, FifamClubStaffPosition::GoalkeeperCoach } } },
        { FifamClubStaffPosition::GoalkeeperCoach,     { { FifamClubStaffPosition::TeamDoctor, FifamClubStaffPosition::SpecialistBone, FifamClubStaffPosition::SpecialistKnee, FifamClubStaffPosition::SpecialistMuscle, FifamClubStaffPosition::Masseur, FifamClubStaffPosition::GeneralManager, FifamClubStaffPosition::MarketingManager, FifamClubStaffPosition::ConstructionManager, FifamClubStaffPosition::SportsDirector, FifamClubStaffPosition::FanRepresentative, FifamClubStaffPosition::Spokesperson, FifamClubStaffPosition::Lawyer, FifamClubStaffPosition::GeneralScout }, { FifamClubStaffPosition::AssistantCoach, FifamClubStaffPosition::AmateurCoach, FifamClubStaffPosition::YouthCoach, FifamClubStaffPosition::FitnessCoach } } },
        { FifamClubStaffPosition::TeamDoctor,          { { FifamClubStaffPosition::AssistantCoach, FifamClubStaffPosition::AmateurCoach, FifamClubStaffPosition::YouthCoach, FifamClubStaffPosition::FitnessCoach, FifamClubStaffPosition::GoalkeeperCoach, FifamClubStaffPosition::Masseur, FifamClubStaffPosition::Psychologist, FifamClubStaffPosition::GeneralManager, FifamClubStaffPosition::MarketingManager, FifamClubStaffPosition::ConstructionManager, FifamClubStaffPosition::SportsDirector, FifamClubStaffPosition::FanRepresentative, FifamClubStaffPosition::Spokesperson, FifamClubStaffPosition::Lawyer, FifamClubStaffPosition::GeneralScout }, {  } } },
        { FifamClubStaffPosition::SpecialistBone,      { { FifamClubStaffPosition::AssistantCoach, FifamClubStaffPosition::AmateurCoach, FifamClubStaffPosition::YouthCoach, FifamClubStaffPosition::FitnessCoach, FifamClubStaffPosition::GoalkeeperCoach, FifamClubStaffPosition::Masseur, FifamClubStaffPosition::Psychologist, FifamClubStaffPosition::GeneralManager, FifamClubStaffPosition::MarketingManager, FifamClubStaffPosition::ConstructionManager, FifamClubStaffPosition::SportsDirector, FifamClubStaffPosition::FanRepresentative, FifamClubStaffPosition::Spokesperson, FifamClubStaffPosition::Lawyer, FifamClubStaffPosition::GeneralScout }, {  } } },
        { FifamClubStaffPosition::SpecialistKnee,      { { FifamClubStaffPosition::AssistantCoach, FifamClubStaffPosition::AmateurCoach, FifamClubStaffPosition::YouthCoach, FifamClubStaffPosition::FitnessCoach, FifamClubStaffPosition::GoalkeeperCoach, FifamClubStaffPosition::Masseur, FifamClubStaffPosition::Psychologist, FifamClubStaffPosition::GeneralManager, FifamClubStaffPosition::MarketingManager, FifamClubStaffPosition::ConstructionManager, FifamClubStaffPosition::SportsDirector, FifamClubStaffPosition::FanRepresentative, FifamClubStaffPosition::Spokesperson, FifamClubStaffPosition::Lawyer, FifamClubStaffPosition::GeneralScout }, {  } } },
        { FifamClubStaffPosition::SpecialistMuscle,    { { FifamClubStaffPosition::AssistantCoach, FifamClubStaffPosition::AmateurCoach, FifamClubStaffPosition::YouthCoach, FifamClubStaffPosition::FitnessCoach, FifamClubStaffPosition::GoalkeeperCoach, FifamClubStaffPosition::Masseur, FifamClubStaffPosition::Psychologist, FifamClubStaffPosition::GeneralManager, FifamClubStaffPosition::MarketingManager, FifamClubStaffPosition::ConstructionManager, FifamClubStaffPosition::SportsDirector, FifamClubStaffPosition::FanRepresentative, FifamClubStaffPosition::Spokesperson, FifamClubStaffPosition::Lawyer, FifamClubStaffPosition::GeneralScout }, {  } } },
        { FifamClubStaffPosition::Masseur,             { { FifamClubStaffPosition::AssistantCoach, FifamClubStaffPosition::AmateurCoach, FifamClubStaffPosition::YouthCoach, FifamClubStaffPosition::FitnessCoach, FifamClubStaffPosition::GoalkeeperCoach, FifamClubStaffPosition::TeamDoctor, FifamClubStaffPosition::SpecialistBone, FifamClubStaffPosition::SpecialistKnee, FifamClubStaffPosition::SpecialistMuscle, FifamClubStaffPosition::Psychologist, FifamClubStaffPosition::GeneralManager, FifamClubStaffPosition::MarketingManager, FifamClubStaffPosition::ConstructionManager, FifamClubStaffPosition::SportsDirector, FifamClubStaffPosition::FanRepresentative, FifamClubStaffPosition::Spokesperson, FifamClubStaffPosition::Lawyer, FifamClubStaffPosition::GeneralScout }, {  } } },
        { FifamClubStaffPosition::Psychologist,        { { FifamClubStaffPosition::TeamDoctor, FifamClubStaffPosition::SpecialistBone, FifamClubStaffPosition::SpecialistKnee, FifamClubStaffPosition::SpecialistMuscle, FifamClubStaffPosition::Masseur, FifamClubStaffPosition::GeneralManager, FifamClubStaffPosition::MarketingManager, FifamClubStaffPosition::ConstructionManager, FifamClubStaffPosition::SportsDirector, FifamClubStaffPosition::FanRepresentative, FifamClubStaffPosition::Spokesperson, FifamClubStaffPosition::Lawyer, FifamClubStaffPosition::GeneralScout }, { FifamClubStaffPosition::AssistantCoach, FifamClubStaffPosition::AmateurCoach, FifamClubStaffPosition::YouthCoach, FifamClubStaffPosition::FitnessCoach, FifamClubStaffPosition::GoalkeeperCoach } } },
        { FifamClubStaffPosition::GeneralManager,      { { FifamClubStaffPosition::AssistantCoach, FifamClubStaffPosition::AmateurCoach, FifamClubStaffPosition::YouthCoach, FifamClubStaffPosition::FitnessCoach, FifamClubStaffPosition::GoalkeeperCoach, FifamClubStaffPosition::TeamDoctor, FifamClubStaffPosition::SpecialistBone, FifamClubStaffPosition::SpecialistKnee, FifamClubStaffPosition::SpecialistMuscle, FifamClubStaffPosition::Masseur, FifamClubStaffPosition::Psychologist, FifamClubStaffPosition::MarketingManager, FifamClubStaffPosition::ConstructionManager, FifamClubStaffPosition::FanRepresentative, FifamClubStaffPosition::Spokesperson, FifamClubStaffPosition::Lawyer, FifamClubStaffPosition::GeneralScout }, {  } } },
        { FifamClubStaffPosition::MarketingManager,    { { FifamClubStaffPosition::AssistantCoach, FifamClubStaffPosition::AmateurCoach, FifamClubStaffPosition::YouthCoach, FifamClubStaffPosition::FitnessCoach, FifamClubStaffPosition::GoalkeeperCoach, FifamClubStaffPosition::TeamDoctor, FifamClubStaffPosition::SpecialistBone, FifamClubStaffPosition::SpecialistKnee, FifamClubStaffPosition::SpecialistMuscle, FifamClubStaffPosition::Masseur, FifamClubStaffPosition::Psychologist, FifamClubStaffPosition::GeneralManager, FifamClubStaffPosition::ConstructionManager, FifamClubStaffPosition::SportsDirector, FifamClubStaffPosition::FanRepresentative, FifamClubStaffPosition::Spokesperson, FifamClubStaffPosition::Lawyer, FifamClubStaffPosition::GeneralScout }, {  } } },
        { FifamClubStaffPosition::ConstructionManager, { { FifamClubStaffPosition::AssistantCoach, FifamClubStaffPosition::AmateurCoach, FifamClubStaffPosition::YouthCoach, FifamClubStaffPosition::FitnessCoach, FifamClubStaffPosition::GoalkeeperCoach, FifamClubStaffPosition::TeamDoctor, FifamClubStaffPosition::SpecialistBone, FifamClubStaffPosition::SpecialistKnee, FifamClubStaffPosition::SpecialistMuscle, FifamClubStaffPosition::Masseur, FifamClubStaffPosition::Psychologist, FifamClubStaffPosition::GeneralManager, FifamClubStaffPosition::MarketingManager, FifamClubStaffPosition::SportsDirector, FifamClubStaffPosition::FanRepresentative, FifamClubStaffPosition::Spokesperson, FifamClubStaffPosition::Lawyer, FifamClubStaffPosition::GeneralScout }, {  } } },
        { FifamClubStaffPosition::SportsDirector,      { { FifamClubStaffPosition::AssistantCoach, FifamClubStaffPosition::AmateurCoach, FifamClubStaffPosition::YouthCoach, FifamClubStaffPosition::FitnessCoach, FifamClubStaffPosition::GoalkeeperCoach, FifamClubStaffPosition::TeamDoctor, FifamClubStaffPosition::SpecialistBone, FifamClubStaffPosition::SpecialistKnee, FifamClubStaffPosition::SpecialistMuscle, FifamClubStaffPosition::Masseur, FifamClubStaffPosition::Psychologist, FifamClubStaffPosition::MarketingManager, FifamClubStaffPosition::ConstructionManager, FifamClubStaffPosition::FanRepresentative, FifamClubStaffPosition::Spokesperson, FifamClubStaffPosition::Lawyer, FifamClubStaffPosition::GeneralScout }, {  } } },
        { FifamClubStaffPosition::FanRepresentative,   { { FifamClubStaffPosition::AssistantCoach, FifamClubStaffPosition::AmateurCoach, FifamClubStaffPosition::YouthCoach, FifamClubStaffPosition::FitnessCoach, FifamClubStaffPosition::GoalkeeperCoach, FifamClubStaffPosition::TeamDoctor, FifamClubStaffPosition::SpecialistBone, FifamClubStaffPosition::SpecialistKnee, FifamClubStaffPosition::SpecialistMuscle, FifamClubStaffPosition::Masseur, FifamClubStaffPosition::Psychologist, FifamClubStaffPosition::GeneralManager, FifamClubStaffPosition::MarketingManager, FifamClubStaffPosition::ConstructionManager, FifamClubStaffPosition::SportsDirector, FifamClubStaffPosition::Lawyer, FifamClubStaffPosition::GeneralScout }, { FifamClubStaffPosition::Spokesperson } } },
        { FifamClubStaffPosition::Spokesperson,        { { FifamClubStaffPosition::AssistantCoach, FifamClubStaffPosition::AmateurCoach, FifamClubStaffPosition::YouthCoach, FifamClubStaffPosition::FitnessCoach, FifamClubStaffPosition::GoalkeeperCoach, FifamClubStaffPosition::TeamDoctor, FifamClubStaffPosition::SpecialistBone, FifamClubStaffPosition::SpecialistKnee, FifamClubStaffPosition::SpecialistMuscle, FifamClubStaffPosition::Masseur, FifamClubStaffPosition::Psychologist, FifamClubStaffPosition::GeneralManager, FifamClubStaffPosition::MarketingManager, FifamClubStaffPosition::ConstructionManager, FifamClubStaffPosition::SportsDirector, FifamClubStaffPosition::Lawyer, FifamClubStaffPosition::GeneralScout }, { FifamClubStaffPosition::FanRepresentative } } },
        { FifamClubStaffPosition::Lawyer,              { { FifamClubStaffPosition::AssistantCoach, FifamClubStaffPosition::AmateurCoach, FifamClubStaffPosition::YouthCoach, FifamClubStaffPosition::FitnessCoach, FifamClubStaffPosition::GoalkeeperCoach, FifamClubStaffPosition::TeamDoctor, FifamClubStaffPosition::SpecialistBone, FifamClubStaffPosition::SpecialistKnee, FifamClubStaffPosition::SpecialistMuscle, FifamClubStaffPosition::Masseur, FifamClubStaffPosition::Psychologist, FifamClubStaffPosition::GeneralManager, FifamClubStaffPosition::MarketingManager, FifamClubStaffPosition::ConstructionManager, FifamClubStaffPosition::SportsDirector, FifamClubStaffPosition::FanRepresentative, FifamClubStaffPosition::Spokesperson, FifamClubStaffPosition::GeneralScout }, {  } } },
        { FifamClubStaffPosition::GeneralScout,        { { FifamClubStaffPosition::AssistantCoach, FifamClubStaffPosition::AmateurCoach, FifamClubStaffPosition::YouthCoach, FifamClubStaffPosition::FitnessCoach, FifamClubStaffPosition::GoalkeeperCoach, FifamClubStaffPosition::TeamDoctor, FifamClubStaffPosition::SpecialistBone, FifamClubStaffPosition::SpecialistKnee, FifamClubStaffPosition::SpecialistMuscle, FifamClubStaffPosition::Masseur, FifamClubStaffPosition::Psychologist, FifamClubStaffPosition::GeneralManager, FifamClubStaffPosition::MarketingManager, FifamClubStaffPosition::ConstructionManager, FifamClubStaffPosition::SportsDirector, FifamClubStaffPosition::FanRepresentative, FifamClubStaffPosition::Spokesperson, FifamClubStaffPosition::Lawyer }, {  } } }
    };

    auto FixMotivationFitnessGK = [](FifamStaff *s, UChar targetLevel) {
        const UInt LEVEL_DIFF = 1;
        UChar target = (targetLevel > LEVEL_DIFF) ? Utils::Clamp(targetLevel - LEVEL_DIFF, 1, 99) : 1;
        if (s->mClubPosition != FifamClubStaffPosition::Psychologist && s->mSkills.MotivationAbility > target)
            s->mSkills.MotivationAbility = target;
        if (s->mClubPosition != FifamClubStaffPosition::FitnessCoach && s->mSkills.FitnessTraining > target)
            s->mSkills.FitnessTraining = target;
        if (s->mClubPosition != FifamClubStaffPosition::GoalkeeperCoach && s->mSkills.GoalkeeperTraining > target)
            s->mSkills.GoalkeeperTraining = target;
    };

    UChar targetLevel = GetStaffLevelFromCA(p->mCurrentAbility);

    FixMotivationFitnessGK(staff, targetLevel);
    SetStaffLevel(staff, staff->mClubPosition, targetLevel, false);

    Float attrRating = (Float)targetLevel / 99.0f;
    auto RandomAttribute = [attrRating](Int min, Int max) {
        return (UChar)roundf((Float)Random::Get(min, max) * attrRating);
    };
    if (staff->mClubPosition == FifamClubStaffPosition::SportsDirector || staff->mClubPosition == FifamClubStaffPosition::GeneralManager) {
        staff->mSkills.SportsLaw = RandomAttribute(15, 30);
        staff->mSkills.LaborLaw = RandomAttribute(15, 30);
        staff->mSkills.PR = RandomAttribute(15, 30);
        staff->mSkills.FanContact = RandomAttribute(15, 30);
        staff->mSkills.Arbitrate = RandomAttribute(15, 30);
        staff->mSkills.Marketing = RandomAttribute(15, 30);
        staff->mSkills.Construction = RandomAttribute(15, 30);
    }
    else {
        staff->mSkills.SportsLaw = RandomAttribute(10, 25);
        staff->mSkills.LaborLaw = RandomAttribute(10, 25);
        staff->mSkills.PR = RandomAttribute(10, 25);
        staff->mSkills.FanContact = RandomAttribute(10, 25);
        staff->mSkills.Arbitrate = RandomAttribute(10, 25);
        staff->mSkills.Marketing = RandomAttribute(10, 25);
        staff->mSkills.Construction = RandomAttribute(10, 25);
    }
#ifdef WRITE_STAFF_DEBUG
    static FifamWriter debugWriter(L"Staff Debug.csv");
    static bool headerWritten = false;
    if (!headerWritten) {
        debugWriter.WriteLine(L"ID,Name,MainPos,LevelBefore,LevelAfter,BP1,BL1,BP2,BL2,BP3,BL3,BP4,BL4,BP5,BL5,AP1,AL1,AP2,AL2,AP3,AL3,AP4,AL4,AP5,AL5,WrongBefore,WrongAfter");
        headerWritten = true;
    }
    auto GetBestPositions = [](FifamStaff *s) {
        Array<Pair<FifamClubStaffPosition, UChar>, 5> bestPos {};
        Vector<Pair<FifamClubStaffPosition, UChar>> posValues;
        FifamClubStaffPosition::ForAllValues([&](FifamClubStaffPosition const &pos) {
            posValues.emplace_back(pos, s->GetStaffLevel(pos));
        });
        std::sort(posValues.begin(), posValues.end(), [&](Pair<FifamClubStaffPosition, UChar> const &a, Pair<FifamClubStaffPosition, UChar> const &b) {
            if (a.second > b.second)
                return true;
            if (b.second > a.second)
                return false;
            if (a.first == s->mClubPosition)
                return true;
            if (b.first == s->mClubPosition)
                return false;
            return true;
        });
        for (UInt i = 0; i < std::size(bestPos); i++)
            bestPos[i] = posValues[i];
        return bestPos;
    };
    auto bestPosBefore = GetBestPositions(staff);
#endif
    UChar maxLevelForUnimportantPos = Utils::Clamp((UChar)roundf((Float)targetLevel * 0.5f), 1, 99);
    auto skillUpdate = skillUpdateTable[staff->mClubPosition];
    for (auto const &pos : skillUpdate.first) {
        if (staff->GetStaffLevel(pos) > maxLevelForUnimportantPos)
            SetStaffLevel(staff, pos, maxLevelForUnimportantPos, true);
    }
    UChar maxLevelForImportantPos = Utils::Clamp(targetLevel - 1, 1, 99);
    for (auto const &pos : skillUpdate.second) {
        if (staff->GetStaffLevel(pos) > maxLevelForImportantPos) {
            Set<FifamStaffSkillID> mySkills;
            staff->ForAllSkills([&](FifamStaffSkillID skillID, UChar &value, Float weight) {
                if (weight > 0.0f)
                    mySkills.insert(skillID);
            });
            Set<FifamStaffSkillID> skillsToChange;
            staff->ForAllSkills(pos, [&](FifamStaffSkillID skillID, UChar &value, Float weight) {
                if (weight > 0.0f && !Utils::Contains(mySkills, skillID))
                    skillsToChange.insert(skillID);
            });
            if (!skillsToChange.empty())
                SetStaffLevel(staff, pos, maxLevelForImportantPos, true, skillsToChange);
        }
    }

    FixMotivationFitnessGK(staff, targetLevel);
    SetStaffLevel(staff, staff->mClubPosition, targetLevel);
    FixMotivationFitnessGK(staff, targetLevel);
    if (staff->GetStaffLevel() < targetLevel) {
        Set<FifamStaffSkillID> skillsToIgnore;
        if (staff->mClubPosition != FifamClubStaffPosition::Psychologist)
            skillsToIgnore.insert(FifamStaffSkillID::MotivationAbility);
        if (staff->mClubPosition != FifamClubStaffPosition::FitnessCoach)
            skillsToIgnore.insert(FifamStaffSkillID::FitnessTraining);
        if (staff->mClubPosition != FifamClubStaffPosition::GoalkeeperCoach)
            skillsToIgnore.insert(FifamStaffSkillID::GoalkeeperTraining);
        Set<FifamStaffSkillID> skillsToChange;
        staff->ForAllSkills([&](FifamStaffSkillID skillID, UChar &value, Float weight) {
            if (weight > 0.0f && !Utils::Contains(skillsToIgnore, skillID))
                skillsToChange.insert(skillID);
        });
        SetStaffLevel(staff, staff->mClubPosition, targetLevel, true, skillsToChange);
    }
    Int minSkillValueMax = Utils::Clamp(staff->GetStaffLevel() / 10, 1, 99);
    Int minSkillValueMin = Utils::Clamp(minSkillValueMax - 2, 1, 99);
    staff->ForAllSkills([&](FifamStaffSkillID skillID, UChar &value, Float weight) {
        if (value < minSkillValueMin) {
            if (minSkillValueMin == minSkillValueMax)
                value = minSkillValueMin;
            else
                value = Random::Get(minSkillValueMin, minSkillValueMax);
        }
    });
#ifdef WRITE_STAFF_DEBUG
    auto bestPosAfter = GetBestPositions(staff);
    if (staff->mClubPosition != FifamClubStaffPosition::Manager
        && staff->mClubPosition != FifamClubStaffPosition::ChiefExec
        && staff->mClubPosition != FifamClubStaffPosition::President)
    {
        debugWriter.WriteLine(staff->mFootballManagerID, Quoted(staff->GetName()), staff->mClubPosition.ToStr(), targetLevel, staff->GetStaffLevel(),
            bestPosBefore[0].first.ToStr(), bestPosBefore[0].second,
            bestPosBefore[1].first.ToStr(), bestPosBefore[1].second,
            bestPosBefore[2].first.ToStr(), bestPosBefore[2].second,
            bestPosBefore[3].first.ToStr(), bestPosBefore[3].second,
            bestPosBefore[4].first.ToStr(), bestPosBefore[4].second,
            bestPosAfter[0].first.ToStr(), bestPosAfter[0].second,
            bestPosAfter[1].first.ToStr(), bestPosAfter[1].second,
            bestPosAfter[2].first.ToStr(), bestPosAfter[2].second,
            bestPosAfter[3].first.ToStr(), bestPosAfter[3].second,
            bestPosAfter[4].first.ToStr(), bestPosAfter[4].second,
            bestPosBefore[0].first != staff->mClubPosition, bestPosAfter[0].first != staff->mClubPosition
        );
    }
#endif
    staff->mScoutPreferredCountries.clear();
    if (staff->mClubPosition == FifamClubStaffPosition::GeneralScout) {
        Map<FifamNation, UInt> scoutNations;
        scoutNations[staff->mNationality[0]] = 3;
        if (staff->mNationality[1] != FifamNation::None && staff->mNationality[1] != staff->mNationality[0])
            scoutNations[staff->mNationality[1]] = 2;
        if (club && club->mCountry) {
            FifamNation clubCountry = FifamNation::MakeFromInt(club->mCountry->mId);
            scoutNations[clubCountry] = 1;
        }
        for (auto const &sn : p->mDaysAtClubOrNation) {
            if (sn.mClubOrNation && sn.mDays > 0) {
                foom::nation *n = nullptr;
                if (sn.mClubOrNation->mIsNation)
                    n = reinterpret_cast<foom::nation *>(sn.mClubOrNation);
                else
                    n = reinterpret_cast<foom::club *>(sn.mClubOrNation)->mNation;
                if (n && n->mConverterData.mFifamCountry) {
                    auto clubCountry = FifamNation::MakeFromInt(reinterpret_cast<FifamCountry *>(n->mConverterData.mFifamCountry)->mId);
                    if (clubCountry != FifamNation::None)
                        scoutNations[clubCountry] += sn.mDays;
                }
            }
        }
        Vector<Pair<FifamNation, UInt>> vecScoutNations;
        for (auto const &sn : scoutNations)
            vecScoutNations.push_back(sn);
        std::sort(vecScoutNations.begin(), vecScoutNations.end(), [](Pair<FifamNation, UInt> const &a, Pair<FifamNation, UInt> const &b) {
            return a.second > b.second;
            });
        if (vecScoutNations.size() > 0) {
            staff->mHasScoutJobData = true;
            for (UInt i = 0; i < Utils::Min(3u, vecScoutNations.size()); i++)
                staff->mScoutPreferredCountries.push_back(vecScoutNations[i].first);
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
    Vector<foom::non_player *> staffHeadPerformanceAnalysts;
    Vector<foom::non_player *> staffPerformanceAnalysts;

    UInt maxAssistantCoaches = 1;
    UInt maxScouts = 1;

    UInt reserveCoachPriority = 0;
    UInt youthCoachPriority = 0;

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
            std::sort(reserveClub->mVecContractedNonPlayers.begin(), reserveClub->mVecContractedNonPlayers.end(),
                [](foom::non_player *a, foom::non_player *b) {
                return a->mCurrentAbility > b->mCurrentAbility;
            });
            for (auto &p : reserveClub->mVecContractedNonPlayers) {
                if (IsConvertable(p, gameId)) { 
                    if (p->mClubContract.mJob == 5 && reserveCoachPriority < 5232) {
                        staffReserveCoach = p;
                        reserveCoachPriority = 5232;
                        break;
                    }
                    else if (p->mClubContract.mJob == 6 && reserveCoachPriority < 2242) {
                        staffReserveCoach = p;
                        reserveCoachPriority = 2242;
                    }
                    else if (p->mClubContract.mJob == 8 && reserveCoachPriority < 1242) {
                        staffReserveCoach = p;
                        reserveCoachPriority = 1242;
                    }
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
            case 7: // Manager reserve team
                if (!staffReserveCoach || reserveCoachPriority < 4241) {
                    staffReserveCoach = p;
                    reserveCoachPriority = 4241;
                }
                break;
            case 101: // U23 manager
                if (!staffReserveCoach || reserveCoachPriority < 3231) {
                    staffReserveCoach = p;
                    reserveCoachPriority = 3231;
                }
                break;
            case 221: // U22 manager
                if (!staffReserveCoach || reserveCoachPriority < 3221) {
                    staffReserveCoach = p;
                    reserveCoachPriority = 3221;
                }
                break;
            case 107: // U21 manager
                if (!staffReserveCoach || reserveCoachPriority < 3211) {
                    staffReserveCoach = p;
                    reserveCoachPriority = 3211;
                }
                break;
            case 113: // U20 manager
                if (!staffReserveCoach || reserveCoachPriority < 3201) {
                    staffReserveCoach = p;
                    reserveCoachPriority = 3201;
                }
                break;
            case 159: // assistant manager reserve team
                if (!staffReserveCoach || reserveCoachPriority < 2241) {
                    staffReserveCoach = p;
                    reserveCoachPriority = 2241;
                }
                break;
            case 102: // assistant manager U23
                if (!staffReserveCoach || reserveCoachPriority < 2231) {
                    staffReserveCoach = p;
                    reserveCoachPriority = 2231;
                }
                break;
            case 29: // assistant manager U22
                if (!staffReserveCoach || reserveCoachPriority < 2221) {
                    staffReserveCoach = p;
                    reserveCoachPriority = 2221;
                }
                break;
            case 108: // assistant manager U21
                if (!staffReserveCoach || reserveCoachPriority < 2211) {
                    staffReserveCoach = p;
                    reserveCoachPriority = 2211;
                }
                break;
            case 114: // assistant manager U20
                if (!staffReserveCoach || reserveCoachPriority < 2201) {
                    staffReserveCoach = p;
                    reserveCoachPriority = 2201;
                }
                break;
            case 160: // coach reserve team
                if (!staffReserveCoach || reserveCoachPriority < 1241) {
                    staffReserveCoach = p;
                    reserveCoachPriority = 1241;
                }
                break;
            case 103: // coach U23
                if (!staffReserveCoach || reserveCoachPriority < 1231) {
                    staffReserveCoach = p;
                    reserveCoachPriority = 1231;
                }
                break;
            case 224: // coach U22
                if (!staffReserveCoach || reserveCoachPriority < 1221) {
                    staffReserveCoach = p;
                    reserveCoachPriority = 1221;
                }
                break;
            case 109: // coach U21
                if (!staffReserveCoach || reserveCoachPriority < 1211) {
                    staffReserveCoach = p;
                    reserveCoachPriority = 1211;
                }
                break;
            case 115: // coach U20
                if (!staffReserveCoach || reserveCoachPriority < 1201) {
                    staffReserveCoach = p;
                    reserveCoachPriority = 1201;
                }
                break;
            case 119: // U19 manager
                if (!staffYouthCoach || youthCoachPriority < 3191) {
                    staffYouthCoach = p;
                    youthCoachPriority = 3191;
                }
                break;
            case 24: // U18 manager
                if (!staffYouthCoach || youthCoachPriority < 3181) {
                    staffYouthCoach = p;
                    youthCoachPriority = 3181;
                }
                break;
            case 120: // U19 assistant manager
                if (!staffYouthCoach || youthCoachPriority < 2191) {
                    staffYouthCoach = p;
                    youthCoachPriority = 2191;
                }
                break;
            case 125: // U18 assistant manager
                if (!staffYouthCoach || youthCoachPriority < 2181) {
                    staffYouthCoach = p;
                    youthCoachPriority = 2181;
                }
                break;
            case 121: // U19 coach
                if (!staffYouthCoach || youthCoachPriority < 1191) {
                    staffYouthCoach = p;
                    youthCoachPriority = 1191;
                }
                break;
            case 30: // U18 coach
                if (!staffYouthCoach || youthCoachPriority < 1181) {
                    staffYouthCoach = p;
                    youthCoachPriority = 1181;
                }
                break;
            case 245: // head performance analyst
                staffHeadPerformanceAnalysts.push_back(p);
                break;
            case 247: // performance analyst
                staffPerformanceAnalysts.push_back(p);
                break;
            }
        }
    }

    if (!staffCoaches.empty())
        staffAssistantCoaches.insert(staffAssistantCoaches.end(), staffCoaches.begin(), staffCoaches.end());
    if (!staffScouts.empty())
        staffChiefScouts.insert(staffChiefScouts.end(), staffScouts.begin(), staffScouts.end());
    if (!staffHeadPerformanceAnalysts.empty())
        staffChiefScouts.insert(staffChiefScouts.end(), staffHeadPerformanceAnalysts.begin(), staffHeadPerformanceAnalysts.end());
    if (!staffPerformanceAnalysts.empty())
        staffChiefScouts.insert(staffChiefScouts.end(), staffPerformanceAnalysts.begin(), staffPerformanceAnalysts.end());

    if (staffManager)
        CreateAndConvertStaff(staffManager, dst, FifamClubStaffPosition::Manager, gameId);
    if (staffOwner) {
        if (IsConvertable(staffOwner, gameId))
            CreateAndConvertStaff(staffOwner, dst, FifamClubStaffPosition::President, gameId);
        if (staffChairman)
            CreateAndConvertStaff(staffChairman, dst, FifamClubStaffPosition::ChiefExec, gameId);
        else if (staffDirector)
            CreateAndConvertStaff(staffDirector, dst, FifamClubStaffPosition::ChiefExec, gameId);
    }
    else {
        if (staffChairman)
            CreateAndConvertStaff(staffChairman, dst, FifamClubStaffPosition::President, gameId);
        if (staffDirector)
            CreateAndConvertStaff(staffDirector, dst, FifamClubStaffPosition::ChiefExec, gameId);
    }
    if (staffManagingDirector)
        CreateAndConvertStaff(staffManagingDirector, dst, FifamClubStaffPosition::GeneralManager, gameId);
    if (gameId >= 9) {
        if (staffSportsDirector)
            CreateAndConvertStaff(staffSportsDirector, dst, FifamClubStaffPosition::SportsDirector, gameId);
    }
    for (UInt i = 0; i < Utils::Min(maxAssistantCoaches, staffAssistantCoaches.size()); i++)
        CreateAndConvertStaff(staffAssistantCoaches[i], dst, FifamClubStaffPosition::AssistantCoach, gameId);
    for (UInt i = 0; i < Utils::Min(maxScouts, staffChiefScouts.size()); i++)
        CreateAndConvertStaff(staffChiefScouts[i], dst, FifamClubStaffPosition::GeneralScout, gameId);
    if (staffReserveCoach && !isNationalTeam)
        CreateAndConvertStaff(staffReserveCoach, dst, FifamClubStaffPosition::AmateurCoach, gameId);
    if (staffYouthCoach && !isNationalTeam)
        CreateAndConvertStaff(staffYouthCoach, dst, FifamClubStaffPosition::YouthCoach, gameId);
    if (staffGoalkeeperCoach)
        CreateAndConvertStaff(staffGoalkeeperCoach, dst, FifamClubStaffPosition::GoalkeeperCoach, gameId);
    if (gameId >= 9) {
        if (staffFitnessCoach)
            CreateAndConvertStaff(staffFitnessCoach, dst, FifamClubStaffPosition::FitnessCoach, gameId);
    }
    if (staffPhysio)
        CreateAndConvertStaff(staffPhysio, dst, FifamClubStaffPosition::Masseur, gameId);
    if (staffTeamDoctor)
        CreateAndConvertStaff(staffTeamDoctor, dst, FifamClubStaffPosition::TeamDoctor, gameId);
}
