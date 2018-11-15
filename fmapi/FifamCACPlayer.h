#pragma once
#include "FifamPropertied.h"
#include "FifamTypes.h"
#include "FifamNation.h"
#include "FifamClubLink.h"
#include "FifamPlayerPositionRole.h"
#include "FifamPlayerAppearance.h"
#include "FifamShoeType.h"
#include "FifamReadWrite.h"
#include "FifamPlayerCharacter.h"
#include "FifamPlayerAbilityID.h"
#include "FifamPlayerCharacterID.h"

// @since FM07
class FifamCACPlayer : public FifamPropertied {
public:
    // @since FM07
    String mFirstName;
    // @since FM07
    String mLastName;
    // @since FM07
    String mPseudonym;
    // @since FM07
    FifamNation mNationality;
    // @since FM07
    FifamPlayerAbilityID mBestAbility;
    // @since FM07
    FifamClubLink mPreferredClub;
    // @since FM07
    Bool mPreferredCACTeam = false;
    // @since FM07
    FifamPlayerPositionRole mPositionRole;
    // @since FM07
    FifamDate mBirthdate;
    // @since FM07
    // @range 0-9
    UChar mTalent = 0;
    // @since FM07
    // @range [0;4]
    UChar mLeftFoot = 0;
    // @since FM07
    // @range [0;4]
    UChar mRightFoot = 0;
    // @since FM07
    FifamPlayerCharacterID mCharacterType;
    // @since FM07
    FifamPlayerAppearance mAppearance;
    // @since FM07
    FifamShoeType mShoeType;

    void Read(FifamReader &reader);
    void Write(FifamWriter &writer);
};
