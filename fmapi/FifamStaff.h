#pragma once

#include "FifamTypes.h"
#include "FifamNation.h"
#include "FifamFormation.h"

class FifamClub;

// @since FM07
class FifamStaff {
public:
    // @since FM07
    Int mID = 0;

    // @since FM07
    // @maxsize 15
    String mFirstName;

    // @since FM07
    // @maxsize 19
    String mLastName;

    // @since FM09
    // @maxsize 19
    String mPseudonym;

    // @since FM09
    // @maxsize 19
    String mNickname;

    // @since FM07
    enum class Type : Char {
        Player,
        Manager,
        Chairman,
        President,
        AssistantCoach,
        GoalkeeperCoach,
        AmateurCoach,
        YouthCoach,
        Scout,
        TeamDoctor,
        SpecialistKnee,
        SpecialistAnkle,
        SpecialistMuscle,
        Masseur,
        Psychologist,
        ManagingDirector,
        MarketingGuy,
        StadiumManager,
        FanRepresentative
    } mType = Type::Manager;

    // @since FM07
    FifamDate mBirthdate;

    // @[1]since FM07
    // @[2]since FM09
    FifamNation mNationality[2];

    // @since FM07
    // @range 0-4
    Char mExperience = 0;

    // @since FM07
    // @range 0-15
    Char mMotivationSkills = 0;

    // @since FM07
    // @range 0-15
    Char mCoachingSkills = 0;

    // @since FM07
    // @range 0-15
    Char mGoalkeepersTraining = 0;

    // @since FM07
    // @range 0-15
    Char mNegotiationSkills = 0;

    // @since FM07
    enum class CharacterFocus : Char {
        None,
        Offensive,
        Defensive,
        Discipline,
        PlayerCharacter,
        Fitness,
        TacticalEducation,
        Language,
        LongContracts,
        YoungPlayers,
        ExperiencedPlayers,
        // @since FM08
        KeepsTeamTogether
    } mCharacterFocus = CharacterFocus::None;

    // @since FM07
    FifamLanguage mLanguage[4] = { FifamLanguage::None, FifamLanguage::None, FifamLanguage::None, FifamLanguage::None };

    // @since FM07
    FifamFormation mFavouriteFormation;

    // @since FM07
    // @range 0-4
    // 0 - less patient, 4 - more patient, can be set to negative value
    Char mStabilityOfBoardOfDirectors = 4;

    // @since FM09
    FifamClub *mFavouriteClub = nullptr;

    // @since FM09
    FifamClub *mWouldNeverWorkForClub = nullptr;

    struct {
        // @since FM07
        Char _1 = 0; // 0,1,2, default 0

        // @since FM07
        Char _2 = 0; // 0-7, default 0
    } Unknown;

    FifamStaff() {}

    void Read(FifamReader &reader, bool readId = false) {
        if (readId)
            reader.ReadLine(mID);
        reader.ReadStartIndex(L"STAFF");
        reader.ReadLine(mFirstName);
        reader.ReadLine(mLastName);
        reader.ReadLine(mType);
        reader.ReadLine(mBirthdate);
        reader.ReadLine(mNationality[0]);
        reader.ReadLine(Unknown._1);
        reader.ReadLine(mExperience);
        if (reader.IsVersionGreaterOrEqual(0x2007, 0x1E)) {
            reader.ReadLine(mNickname);
            reader.ReadLine(mPseudonym);
            reader.ReadLine(mNationality[1]);
            Int clubId;
            reader.ReadLine(clubId);
            // mFavouriteClub = FifamClub::GetClub(clubId);
            reader.ReadLine(clubId);
            // mWouldNeverWorkForClub = FifamClub::GetClub(clubId);
        }
        reader.ReadLine(mMotivationSkills);
        reader.ReadLine(mCoachingSkills);
        reader.ReadLine(mGoalkeepersTraining);
        reader.ReadLine(mNegotiationSkills);
        reader.ReadLine(mCharacterFocus);
        reader.ReadLine(mLanguage[0]);
        reader.ReadLine(mLanguage[1]);
        reader.ReadLine(mLanguage[2]);
        reader.ReadLine(mLanguage[3]);
        reader.ReadLine(mFavouriteFormation);
        reader.ReadLine(Unknown._2);
        reader.ReadLine(mStabilityOfBoardOfDirectors);
        reader.ReadEndIndex(L"STAFF");
    }

    void Write(FifamWriter &writer, bool writeId = false) {
        if (writeId)
            writer.WriteLine(mID);
        writer.WriteStartIndex(L"STAFF");
        writer.WriteLine(mFirstName);
        writer.WriteLine(mLastName);
        writer.WriteLine(mType);
        writer.WriteLine(mBirthdate);
        writer.WriteLine(mNationality);
        writer.WriteLine(Unknown._1);
        writer.WriteLine(mExperience);
        if (writer.IsVersionGreaterOrEqual(0x2007, 0x1E)) {
            writer.WriteLine(mNickname);
            writer.WriteLine(mPseudonym);
            writer.WriteLine(mNationality[1]);
            writer.WriteLine(0);
            //writer.WriteLine(FifamClub::GetID(mFavouriteClub));
            writer.WriteLine(0);
            //writer.WriteLine(FifamClub::GetID(mWouldNeverWorkForClub));
        }
        writer.WriteLine(mMotivationSkills);
        writer.WriteLine(mCoachingSkills);
        writer.WriteLine(mGoalkeepersTraining);
        writer.WriteLine(mNegotiationSkills);
        if (writer.GetGameId() < 8 && mCharacterFocus == CharacterFocus::KeepsTeamTogether)
            writer.WriteLine(0);
        else
            writer.WriteLine(mCharacterFocus);
        writer.WriteLine(mLanguage[0]);
        writer.WriteLine(mLanguage[1]);
        writer.WriteLine(mLanguage[2]);
        writer.WriteLine(mLanguage[3]);
        writer.WriteLine(mFavouriteFormation);
        writer.WriteLine(Unknown._2);
        writer.WriteLine(mStabilityOfBoardOfDirectors);
        writer.WriteEndIndex(L"STAFF");
    }
};
