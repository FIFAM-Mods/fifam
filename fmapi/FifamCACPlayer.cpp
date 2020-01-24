#include "FifamCACPlayer.h"
#include "FifamUtils.h"
#include "Random.h"

void FifamCACPlayer::Read(FifamReader &reader) {
    if (reader.ReadStartIndex(L"PL_POOL")) {
        reader.ReadLine(mFirstName);
        reader.ReadLine(mLastName);
        reader.ReadLine(mPseudonym);
        reader.ReadLine(mNationality);
        reader.ReadLine(mBestAbility);
        Int teamID = reader.ReadLine<Int>();
        if (teamID < 0)
            mPreferredCACTeam = true;
        else if (teamID != 0)
            FifamUtils::SaveClubIDToClubLink(mPreferredClub, teamID);
        if (reader.IsVersionGreaterOrEqual(0x2004, 0))
            reader.ReadLine(mPositionRole);
        else
            mPositionRole.SetFromInt(Random::Select({ FifamPlayerPositionRole::Goalkeeper, FifamPlayerPositionRole::Defender, FifamPlayerPositionRole::Midfielder, FifamPlayerPositionRole::Attacker }));
        if (reader.IsVersionGreaterOrEqual(0x2007, 0)) {
            reader.ReadLine(mBirthdate);
            reader.ReadLine(mTalent);
            UChar footPrefs = reader.ReadLine<UChar>();
            if (reader.IsVersionGreaterOrEqual(0x2009, 0x02)) {
                mRightFoot = footPrefs & 0xF;
                mLeftFoot = (footPrefs >> 4) & 0xF;
            }
            else {
                mTalent = mTalent * 2 + 1;
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
            reader.ReadLine(mCharacterType);
            if (reader.IsVersionGreaterOrEqual(0x2011, 0x00)) {
                reader.ReadLine(mAppearance.mGenericFace);
                reader.ReadLine(mAppearance.mHairStyle);
                reader.ReadLine(mAppearance.mHairColor);
                reader.ReadLine(mAppearance.mSideburns);
                reader.ReadLine(mAppearance.mBeardType);
                reader.ReadLine(mAppearance.mBeardColor);
                reader.ReadLine(mAppearance.mSkinColor);
                reader.ReadLine(mAppearance.mFaceVariation);
                reader.ReadLine(mAppearance.mEyeColour);
            }
            else {
                FifamPlayerAppearance::AppearanceInfo07 info07;
                reader.ReadLine(info07.faceId);
                reader.ReadLine(info07.hairColor);
                reader.ReadLine(info07.hairId);
                reader.ReadLine(info07.beardType);
                mAppearance.SetFrom07AppearanceInfo(info07);
            }
            reader.ReadLine(mShoeType);
        }
        else {
            mBirthdate.Set(0, 0, 0);
            mTalent = Random::Get(5, 8);
            if (Random::Get(0, 1) == 0) {
                mRightFoot = 4;
                mLeftFoot = Random::Get(0, 2);
            }
            else {
                mLeftFoot = 4;
                mRightFoot = Random::Get(0, 2);
            }
            FifamPlayerAppearance::AppearanceInfo07 info07;
            info07.faceId = 0;
            info07.hairColor = 0;
            info07.hairId = 0;
            info07.beardType = 1;
            mAppearance. SetFrom07AppearanceInfo(info07);
            mShoeType = FifamShoeType::Black;
        }
        reader.ReadEndIndex(L"PL_POOL");
    }
}

void FifamCACPlayer::Write(FifamWriter &writer) {
    writer.WriteStartIndex(L"PL_POOL");
    writer.WriteLine(mFirstName);
    writer.WriteLine(mLastName);
    writer.WriteLine(mPseudonym);
    writer.WriteLine(mNationality);
    writer.WriteLine(mBestAbility);
    if (mPreferredCACTeam)
        writer.WriteLine(-1);
    else
        writer.WriteLine(FifamUtils::GetWriteableID(mPreferredClub));
    if (writer.IsVersionGreaterOrEqual(0x2004, 0))
        writer.WriteLine(mPositionRole);
    if (writer.IsVersionGreaterOrEqual(0x2007, 0)) {
        writer.WriteLine(mBirthdate);
        if (writer.IsVersionGreaterOrEqual(0x2009, 0x02)) {
            writer.WriteLine(mTalent);
            writer.WriteLine((mRightFoot & 0xF) | ((mLeftFoot & 0xF) << 4));
        }
        else {
            writer.WriteLine(mTalent / 2);
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
            writer.WriteLine(footPrefs);
        }
        writer.WriteLine(mCharacterType);
        if (writer.IsVersionGreaterOrEqual(0x2011, 0x00)) {
            writer.WriteLine(mAppearance.mGenericFace);
            writer.WriteLine(mAppearance.mHairStyle);
            writer.WriteLine(mAppearance.mHairColor);
            writer.WriteLine(mAppearance.mSideburns);
            writer.WriteLine(mAppearance.mBeardType);
            writer.WriteLine(mAppearance.mBeardColor);
            writer.WriteLine(mAppearance.mSkinColor);
            writer.WriteLine(mAppearance.mFaceVariation);
            writer.WriteLine(mAppearance.mEyeColour);
        }
        else {
            auto info07 = mAppearance.Get07AppearanceInfo();
            writer.WriteLine(info07.faceId);
            writer.WriteLine(info07.hairColor);
            writer.WriteLine(info07.hairId);
            writer.WriteLine(info07.beardType);
        }
        writer.WriteLine(mShoeType);
    }
    writer.WriteEndIndex(L"PL_POOL");
}
