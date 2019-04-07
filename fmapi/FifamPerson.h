#pragma once
#include "FifamDbWriteable.h"
#include "FifamPropertied.h"
#include "FifamPersonType.h"
#include "FifamDate.h"
#include "FifamNation.h"
#include "FifamLanguage.h"
#include "FifamFormation.h"
#include "FifamChairmanStability.h"
#include "FifamClubLink.h"

class FifamPlayer;
class FifamStaff;

class FifamPerson : public FifamDbWriteable, public FifamPropertied {
public:
    // @since FM07
    UInt mID = 0;
    // @since FM07
    FifamPersonType mPersonType;
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
    // @range [0;9]
    UChar mTalent = 0;
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
    // @since FM07
    FifamClubLink mFavouriteClub;
    // @since FM07
    FifamClubLink mWouldnSignFor;
    // @since FM07
    FifamPlayer *mManagerFavouritePlayer = nullptr;

    FifamClub *mClub = nullptr;

    String mWriteableStringID;

    FifamPlayer *AsPlayer();
    FifamStaff *AsStaff();
    String GetName() const;
    UInt GetAge(FifamDate const &currentDate);
};
