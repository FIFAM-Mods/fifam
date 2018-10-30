#pragma once
#include "FifamPerson.h"
#include "FifamManagerFocus.h"
#include "FifamClubStaffPosition.h"
#include "FifamCoachPlayingOrientation.h"

class FifamPlayer;

// @since FM07
class FifamStaff : public FifamPerson {
public:
    // @since FM07
    FifamClubStaffPosition mClubPosition;
    // @since FM07
    // @range 0-4
    UChar mExperience = 0;
    // @since FM07
    FifamManagerFocus mManagerFocus;
    // @since FM09
    struct PersonalityAttributes {
        // @since FM09
        UChar WillingnessToLearn = 0;
        // @since FM09
        UChar Resilence = 0;
        // @since FM09
        UChar Effort = 0;
    } mPersonalityAttributes;
    // @since FM09
    struct Skills {
        UChar Tactics = 0;
        UChar FieldPlayerTraining = 0;
        UChar GoalkeeperTraining = 0;
        UChar FitnessTraining = 0;
        UChar SkillEstimation = 0;
        UChar TeamEstimation = 0;
        UChar MotivationAbility = 0;
        UChar BoneInjury = 0;
        UChar KneeInjury = 0;
        UChar MuscleInjury = 0;
        UChar InjuryPrevention = 0;
        UChar RegenerationAbility = 0;
        UChar Arbitrate = 0;
        UChar Negotiation = 0;
        UChar Marketing = 0;
        UChar Sponsoring = 0;
        UChar Construction = 0;
        UChar PR = 0;
        UChar FanContact = 0;
        UChar SportsLaw = 0;
        UChar LaborLaw = 0;
        UChar GeneralScouting = 0;
        UChar TalentEstimation = 0;
        UChar FieldSkillsEstimation = 0;
        UChar GoalkeeperSkillsEstimation = 0;
        UChar MentalSkillsEstimation = 0;
        UChar PhysicalSkillsEstimation = 0;
        UChar Networking = 0;
    } mSkills;
    // @since FM09
    FifamCoachPlayingOrientation mCoachPlayingOrientation;
    // @since FM09
    Vector<FifamNation> mScoutPreferredCountries;
    // @since FM09
    Bool mHasNoneJobData = true;
    // @since FM09
    Bool mHasCoachJobData = true;
    // @since FM09
    Bool mHasMedicineJobData = true;
    // @since FM09
    Bool mHasManagerJobData = true;
    // @since FM09
    Bool mHasPRJobData = true;
    // @since FM09
    Bool mHasLawyerJobData = true;
    // @since FM09
    Bool mHasScoutJobData = true;
    // @since FM11
    FifamDate mJoinedClubDate;

    struct {
        // @since FM07
        Char _1 = 0; // 0,1,2, default 0
        // @since FM07
        Char _2 = 0; // 0-7, default 0
    } Unknown;

    FifamNation mLinkedCountry;

    FifamStaff();
    void Read(FifamReader &reader);
    void ReadWorker(FifamReader &reader);
    void ReadManager(FifamReader &reader);
    void ReadFromPlayer(FifamReader &reader);
    void Write(FifamWriter &writer);
    void WriteManager(FifamWriter &writer);
    void WriteWorker(FifamWriter &writer);
    void WriteToPlayer(FifamWriter &writer);
    UChar GetLevel();
    UChar GetLevel(FifamClubStaffPosition position);
    String GetStringUniqueId(UInt gameId);
};
