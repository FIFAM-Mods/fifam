#pragma once
#include "FifamStaff.h"
#include "FifamStaffSkillID.h"

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

void SetStaffLevel(FifamStaff *staff, FifamClubStaffPosition position, Int level, Bool decreaseOnlyImportantAttributes = false,
    Set<FifamStaffSkillID> skillsToChange = Set<FifamStaffSkillID>()) {
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
                if (skillsToChange.empty() || skillsToChange.contains(skillID)) {
                    if (!increase) {
                        if (!decreaseOnlyImportantAttributes || weight > 0.0f)
                            value = Utils::Clamp(value - 1, 1, 99);
                    }
                    else if (weight > 0.0f)
                        value = Utils::Clamp((Int)value + roundf((Float)iterations * weight), 1, 99);
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

class StaffTest {
public:
    StaffTest() {
        FifamStaff _staff;
        FifamStaff *staff = &_staff;

        staff->mClubPosition = FifamClubStaffPosition::AssistantCoach;
        staff->mSkills.Tactics = 83;
        staff->mSkills.FieldPlayerTraining = 82;
        staff->mSkills.GoalkeeperTraining = 15;
        staff->mSkills.FitnessTraining = 87;
        staff->mSkills.SkillEstimation = 50;
        staff->mSkills.TeamEstimation = 50;
        staff->mSkills.MotivationAbility = 69;
        staff->mSkills.BoneInjury = 58;
        staff->mSkills.KneeInjury = 67;
        staff->mSkills.MuscleInjury = 55;
        staff->mSkills.InjuryPrevention = 63;
        staff->mSkills.RegenerationAbility = 36;
        staff->mSkills.Arbitrate = 2;
        staff->mSkills.Negotiation = 39;
        staff->mSkills.Marketing = 1;
        staff->mSkills.Sponsoring = 41;
        staff->mSkills.Construction = 2;
        staff->mSkills.PR = 1;
        staff->mSkills.FanContact = 2;
        staff->mSkills.SportsLaw = 2;
        staff->mSkills.LaborLaw = 2;
        staff->mSkills.GeneralScouting = 79;
        staff->mSkills.TalentEstimation = 76;
        staff->mSkills.FieldSkillsEstimation = 86;
        staff->mSkills.GoalkeeperSkillsEstimation = 74;
        staff->mSkills.MentalSkillsEstimation = 88;
        staff->mSkills.PhysicalSkillsEstimation = 72;
        staff->mSkills.Networking = 79;

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

        auto FixMotivationFitness = [](FifamStaff *s, UChar targetLevel) {
            UChar target = (targetLevel > 5) ? Utils::Clamp(targetLevel - 5, 1, 99) : 1;
            if (s->mClubPosition != FifamClubStaffPosition::Psychologist && s->mSkills.MotivationAbility > target)
                s->mSkills.MotivationAbility = target;
            if (s->mClubPosition != FifamClubStaffPosition::FitnessCoach && s->mSkills.FitnessTraining > target)
                s->mSkills.FitnessTraining = target;
        };

        auto skillsBefore = staff->mSkills;
        auto bestPosBefore = GetBestPositions(staff);
        UChar targetLevel = GetStaffLevelFromCA(147);
        FixMotivationFitness(staff, targetLevel);
        SetStaffLevel(staff, staff->mClubPosition, targetLevel);

        static Map<FifamClubStaffPosition, Pair<Set<FifamClubStaffPosition>, Set<FifamClubStaffPosition>>> skillUpdateTable = {
        { FifamClubStaffPosition::AssistantCoach,      { { FifamClubStaffPosition::TeamDoctor, FifamClubStaffPosition::SpecialistBone, FifamClubStaffPosition::SpecialistKnee, FifamClubStaffPosition::SpecialistMuscle, FifamClubStaffPosition::Masseur, FifamClubStaffPosition::GeneralManager, FifamClubStaffPosition::MarketingManager, FifamClubStaffPosition::ConstructionManager, FifamClubStaffPosition::SportsDirector, FifamClubStaffPosition::FanRepresentative, FifamClubStaffPosition::Spokesperson, FifamClubStaffPosition::Lawyer, FifamClubStaffPosition::GeneralScout }, {  } } },
        { FifamClubStaffPosition::AmateurCoach,        { { FifamClubStaffPosition::TeamDoctor, FifamClubStaffPosition::SpecialistBone, FifamClubStaffPosition::SpecialistKnee, FifamClubStaffPosition::SpecialistMuscle, FifamClubStaffPosition::Masseur, FifamClubStaffPosition::GeneralManager, FifamClubStaffPosition::MarketingManager, FifamClubStaffPosition::ConstructionManager, FifamClubStaffPosition::SportsDirector, FifamClubStaffPosition::FanRepresentative, FifamClubStaffPosition::Spokesperson, FifamClubStaffPosition::Lawyer, FifamClubStaffPosition::GeneralScout }, {  } } },
        { FifamClubStaffPosition::YouthCoach,          { { FifamClubStaffPosition::TeamDoctor, FifamClubStaffPosition::SpecialistBone, FifamClubStaffPosition::SpecialistKnee, FifamClubStaffPosition::SpecialistMuscle, FifamClubStaffPosition::Masseur, FifamClubStaffPosition::GeneralManager, FifamClubStaffPosition::MarketingManager, FifamClubStaffPosition::ConstructionManager, FifamClubStaffPosition::SportsDirector, FifamClubStaffPosition::FanRepresentative, FifamClubStaffPosition::Spokesperson, FifamClubStaffPosition::Lawyer, FifamClubStaffPosition::GeneralScout }, {  } } },
        { FifamClubStaffPosition::FitnessCoach,        { { FifamClubStaffPosition::TeamDoctor, FifamClubStaffPosition::SpecialistBone, FifamClubStaffPosition::SpecialistKnee, FifamClubStaffPosition::SpecialistMuscle, FifamClubStaffPosition::Masseur, FifamClubStaffPosition::GeneralManager, FifamClubStaffPosition::MarketingManager, FifamClubStaffPosition::ConstructionManager, FifamClubStaffPosition::SportsDirector, FifamClubStaffPosition::FanRepresentative, FifamClubStaffPosition::Spokesperson, FifamClubStaffPosition::Lawyer }, { FifamClubStaffPosition::AssistantCoach, FifamClubStaffPosition::AmateurCoach, FifamClubStaffPosition::YouthCoach, FifamClubStaffPosition::GoalkeeperCoach } } },
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
                    if (weight > 0.0f && !mySkills.contains(skillID))
                        skillsToChange.insert(skillID);
                    });
                if (!skillsToChange.empty())
                    SetStaffLevel(staff, pos, maxLevelForImportantPos, true, skillsToChange);
            }
        }
        FixMotivationFitness(staff, targetLevel);
        SetStaffLevel(staff, staff->mClubPosition, targetLevel);
        FixMotivationFitness(staff, targetLevel);

        auto bestPosAfter = GetBestPositions(staff);
        std::wcout << std::endl <<
            L"StaffPosition: " << staff->mClubPosition.ToStr() << L" TargetLevel: " << targetLevel << std::endl <<
            std::endl <<
            L"Best 5 Positions Before:" << std::endl <<
            bestPosBefore[0].second << L" - " << bestPosBefore[0].first.ToStr() << std::endl <<
            bestPosBefore[1].second << L" - " << bestPosBefore[1].first.ToStr() << std::endl <<
            bestPosBefore[2].second << L" - " << bestPosBefore[2].first.ToStr() << std::endl <<
            bestPosBefore[3].second << L" - " << bestPosBefore[3].first.ToStr() << std::endl <<
            bestPosBefore[4].second << L" - " << bestPosBefore[4].first.ToStr() << std::endl <<
            std::endl <<
            L"Best 5 Positions After:" << std::endl <<
            bestPosAfter[0].second << L" - " << bestPosAfter[0].first.ToStr() << std::endl <<
            bestPosAfter[1].second << L" - " << bestPosAfter[1].first.ToStr() << std::endl <<
            bestPosAfter[2].second << L" - " << bestPosAfter[2].first.ToStr() << std::endl <<
            bestPosAfter[3].second << L" - " << bestPosAfter[3].first.ToStr() << std::endl <<
            bestPosAfter[4].second << L" - " << bestPosAfter[4].first.ToStr() << std::endl;

        auto Diff = [](Int before, Int after) {
            String result;
            if (before != after) {
                if (before > after)
                    result = L" (-" + to_wstring(before - after) + L")";
                else
                    result = L" (+" + to_wstring(after - before) + L")";
            }
            return result;
        };

        std::wcout << std::endl <<
            L"Tactics: " << staff->mSkills.Tactics << Diff(skillsBefore.Tactics, staff->mSkills.Tactics) << std::endl <<
            L"FieldPlayerTraining: " << staff->mSkills.FieldPlayerTraining << Diff(skillsBefore.FieldPlayerTraining, staff->mSkills.FieldPlayerTraining) << std::endl <<
            L"GoalkeeperTraining: " << staff->mSkills.GoalkeeperTraining << Diff(skillsBefore.GoalkeeperTraining, staff->mSkills.GoalkeeperTraining) << std::endl <<
            L"FitnessTraining: " << staff->mSkills.FitnessTraining << Diff(skillsBefore.FitnessTraining, staff->mSkills.FitnessTraining) << std::endl <<
            L"SkillEstimation: " << staff->mSkills.SkillEstimation << Diff(skillsBefore.SkillEstimation, staff->mSkills.SkillEstimation) << std::endl <<
            L"TeamEstimation: " << staff->mSkills.TeamEstimation << Diff(skillsBefore.TeamEstimation, staff->mSkills.TeamEstimation) << std::endl <<
            L"MotivationAbility: " << staff->mSkills.MotivationAbility << Diff(skillsBefore.MotivationAbility, staff->mSkills.MotivationAbility) << std::endl <<
            L"BoneInjury: " << staff->mSkills.BoneInjury << Diff(skillsBefore.BoneInjury, staff->mSkills.BoneInjury) << std::endl <<
            L"KneeInjury: " << staff->mSkills.KneeInjury << Diff(skillsBefore.KneeInjury, staff->mSkills.KneeInjury) << std::endl <<
            L"MuscleInjury: " << staff->mSkills.MuscleInjury << Diff(skillsBefore.MuscleInjury, staff->mSkills.MuscleInjury) << std::endl <<
            L"InjuryPrevention: " << staff->mSkills.InjuryPrevention << Diff(skillsBefore.InjuryPrevention, staff->mSkills.InjuryPrevention) << std::endl <<
            L"RegenerationAbility: " << staff->mSkills.RegenerationAbility << Diff(skillsBefore.RegenerationAbility, staff->mSkills.RegenerationAbility) << std::endl <<
            L"Arbitrate: " << staff->mSkills.Arbitrate << Diff(skillsBefore.Arbitrate, staff->mSkills.Arbitrate) << std::endl <<
            L"Negotiation: " << staff->mSkills.Negotiation << Diff(skillsBefore.Negotiation, staff->mSkills.Negotiation) << std::endl <<
            L"Marketing: " << staff->mSkills.Marketing << Diff(skillsBefore.Marketing, staff->mSkills.Marketing) << std::endl <<
            L"Sponsoring: " << staff->mSkills.Sponsoring << Diff(skillsBefore.Sponsoring, staff->mSkills.Sponsoring) << std::endl <<
            L"Construction: " << staff->mSkills.Construction << Diff(skillsBefore.Construction, staff->mSkills.Construction) << std::endl <<
            L"PR: " << staff->mSkills.PR << Diff(skillsBefore.PR, staff->mSkills.PR) << std::endl <<
            L"FanContact: " << staff->mSkills.FanContact << Diff(skillsBefore.FanContact, staff->mSkills.FanContact) << std::endl <<
            L"SportsLaw: " << staff->mSkills.SportsLaw << Diff(skillsBefore.SportsLaw, staff->mSkills.SportsLaw) << std::endl <<
            L"LaborLaw: " << staff->mSkills.LaborLaw << Diff(skillsBefore.LaborLaw, staff->mSkills.LaborLaw) << std::endl <<
            L"GeneralScouting: " << staff->mSkills.GeneralScouting << Diff(skillsBefore.GeneralScouting, staff->mSkills.GeneralScouting) << std::endl <<
            L"TalentEstimation: " << staff->mSkills.TalentEstimation << Diff(skillsBefore.TalentEstimation, staff->mSkills.TalentEstimation) << std::endl <<
            L"FieldSkillsEstimation: " << staff->mSkills.FieldSkillsEstimation << Diff(skillsBefore.FieldSkillsEstimation, staff->mSkills.FieldSkillsEstimation) << std::endl <<
            L"GoalkeeperSkillsEstimation: " << staff->mSkills.GoalkeeperSkillsEstimation << Diff(skillsBefore.GoalkeeperSkillsEstimation, staff->mSkills.GoalkeeperSkillsEstimation) << std::endl <<
            L"MentalSkillsEstimation: " << staff->mSkills.MentalSkillsEstimation << Diff(skillsBefore.MentalSkillsEstimation, staff->mSkills.MentalSkillsEstimation) << std::endl <<
            L"PhysicalSkillsEstimation: " << staff->mSkills.PhysicalSkillsEstimation << Diff(skillsBefore.PhysicalSkillsEstimation, staff->mSkills.PhysicalSkillsEstimation) << std::endl <<
            L"Networking: " << staff->mSkills.Networking << Diff(skillsBefore.Networking, staff->mSkills.Networking);
    }
};
