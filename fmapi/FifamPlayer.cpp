#include "FifamPlayer.h"

FifamPlayer::FifamPlayer() {
    mPersonType = FifamPersonType::Player;
}

String FifamPlayer::GetName() const {
    if (!mPseudonym.empty())
        return mPseudonym;
    if (!mFirstName.empty())
        return mFirstName + L" " + mLastName;
    return mLastName;
}

void FifamPlayer::Read(FifamReader &reader, FifamDatabase *database) {
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

            mHistory.Read(reader, database);
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
            if (playerBasicFlags & 1)
                mIsNaturalised = true;
            if (playerBasicFlags & 2)
                mIsBasque = true;
            if (playerBasicFlags & 4)
                mIsRealPlayer = true;
            reader.ReadLine(mBirthday);
            if (reader.IsVersionGreaterOrEqual(0x2009, 0x02)) {
                reader.ReadLine(mTalent);
                UChar footPrefs = reader.ReadLine<UChar>();
                mRightFoot = footPrefs & 0xF;
                mLeftFoot = (footPrefs >> 4) & 0xF;
            }
            else {
                UChar flags = reader.ReadLine<UChar>();
                UChar footPrefs = (flags >> 3) & 7;
                mTalent = (flags & 7) * 2 + 1;
                mInYouthTeam = (flags & 0x40) == 0x40;
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
            reader.ReadLine(mHeroStatus);

            mHistory.Read(reader, database);


            if (playerBasicFlags >= 8)
                Error("playerBasicFlags: unknown value (%u)", playerBasicFlags);
        }
        reader.ReadEndIndex(L"PLAYER");

        FifamCheckEnum(mPersonType);
        FifamCheckEnum(mNationality[0]);
        FifamCheckEnum(mNationality[1]);
        if (mLeftFoot > 4)
            Error("mLeftFoot: unknown value (%u)", mLeftFoot);
        if (mRightFoot > 4)
            Error("mRightFoot: unknown value (%u)", mRightFoot);
    }
}

void FifamPlayer::Write(FifamWriter &writer, FifamDatabase *database) {

}
