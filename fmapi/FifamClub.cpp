#include "FifamClub.h"
#include "FifamCountry.h"
#include "FifamDatabase.h"

void FifamClub::Read(FifamReader &reader, UInt id) {
    String clubSectionName = Utils::Format(L"CLUB%u", id);
    if (reader.ReadStartIndex(clubSectionName)) {
        reader.ReadVersion();
        if (reader.IsVersionGreaterOrEqual(0x2007, 0x12))
            reader.ReadLine(mUniqueID);
        else
            mUniqueID = id | (mCountry->mId << 16);
        reader.ReadLineTranslationArray(mName);
        reader.ReadLineTranslationArray(mName2);
        reader.ReadLineTranslationArray(mShortName);
        reader.ReadLineTranslationArray(mShortName2);
        reader.ReadLineTranslationArray(mAbbreviationArticle);
        reader.ReadLineTranslationArray(mPlayerInTextArticle);
        if (reader.IsVersionGreaterOrEqual(0x2011, 0x05)) {
            reader.ReadLineTranslationArray(mTermForTeam1Article);
            reader.ReadLineTranslationArray(mTermForTeam2Article);
            reader.ReadLineTranslationArray(mFanName1Article);
            reader.ReadLineTranslationArray(mFanName2Article);
        }
        else
            reader.ReadLineTranslationArray(mFanName1Article);
        reader.ReadLineTranslationArray(mAbbreviation);
        reader.ReadLineTranslationArray(mClubNameUsageInPhrase);
        reader.ReadLineTranslationArray(mClubNameUsageInPhrase2);
        reader.ReadLineTranslationArray(mCityName);
        if (reader.IsVersionGreaterOrEqual(0x2011, 0x09)) {
            reader.ReadLineTranslationArray(mPlayerInText);
            reader.ReadLineTranslationArray(mTermForTeam1);
            reader.ReadLineTranslationArray(mTermForTeam2);
            reader.ReadLineTranslationArray(mFanName1);
            reader.ReadLineTranslationArray(mFanName2);
        }
        else {
            if (reader.IsVersionGreaterOrEqual(0x2011, 0x06)) {
                reader.ReadLine(mPlayerInText[0]);
                reader.ReadLine(mTermForTeam1[0]);
                reader.ReadLine(mTermForTeam2[0]);
                reader.ReadLine(mFanName1[0]);
                reader.ReadLine(mFanName2[0]);
            }
            else {
                reader.ReadLine(mPlayerInText[0]);
                reader.ReadLine(mFanName1[0]);
            }
            for (UInt i = 1; i < FifamTranslation::NUM_TRANSLATIONS; i++) {
                mPlayerInText[i] = mPlayerInText[0];
                mTermForTeam1[i] = mTermForTeam1[0];
                mTermForTeam2[i] = mTermForTeam2[0];
                mFanName1[i] = mFanName1[0];
                mFanName2[i] = mFanName2[0];
            }
        }
        reader.ReadLine(mPartnershipClub);
        for (UInt i = 0; i < 4; i++)
            reader.ReadLine(mRivalClubs[i]);
        reader.ReadLine(mYearOfFoundation);
        reader.ReadFullLine(mAddress);
        reader.ReadFullLine(mTelephone);
        reader.ReadFullLine(mWebsiteAndMail);
        reader.ReadFullLine(mFansites);
        reader.ReadFullLine(Unknown._1);
        reader.ReadFullLine(mMascotName);
        reader.ReadLine(mInitialCapital);
        reader.ReadLine(mTransferBudget);
        reader.ReadLine(mJointStockCompany);
        reader.ReadLine(Unknown._2);
        reader.ReadLine(mPenaltyFlags);
        reader.ReadLine(mPenaltyPoints);
        reader.ReadLine(mPotentialFansCount);
        reader.ReadLine(mAverageAttendanceLastSeason);
        reader.ReadLine(mCountOfSoldSeasonTickets);
        UInt numLeagues = 1;
        if (reader.IsVersionGreaterOrEqual(0x2009, 0x06))
            numLeagues = 3;
        else if (reader.IsVersionGreaterOrEqual(0x2007, 0x14))
            numLeagues = 2;
        for (UInt i = 0; i < numLeagues; i++)
            reader.ReadLine(mLeagueTotalPoints[i]);
        for (UInt i = 0; i < numLeagues; i++)
            reader.ReadLine(mLeagueTotalWins[i]);
        for (UInt i = 0; i < numLeagues; i++)
            reader.ReadLine(mLeagueTotalDraws[i]);
        for (UInt i = 0; i < numLeagues; i++)
            reader.ReadLine(mLeagueTotalLoses[i]);
        for (UInt i = 0; i < numLeagues; i++)
            reader.ReadLine(mLeagueTotalGoals[i]);
        for (UInt i = 0; i < numLeagues; i++)
            reader.ReadLine(mLeagueTotalGoalsAgainst[i]);
        for (UInt i = 0; i < numLeagues; i++)
            reader.ReadLine(mLeagueTotalLeadershipsInTable[i]);
        reader.ReadLine(mStadiumVenue);
        reader.ReadLine(mStadiumCapacity);
        reader.ReadLineTranslationArray(mStadiumName);
        reader.ReadLine(mStadiumType);
        reader.ReadLine(mFlags);
        mClubFacilities = Utils::Clamp(reader.ReadLine<UChar>(), 0, 5) + 1;
        mMedicalDepartment = Utils::Clamp(reader.ReadLine<UChar>(), 0, 5);
        mMerchandising = Utils::Clamp(reader.ReadLine<UChar>(), 0, 5);
        UChar youthDepartment = Utils::MapTo(reader.ReadLine<UChar>(), 0, 5, 0, 10);
        mYouthBoardingSchool = youthDepartment;
        mYouthCentre = youthDepartment;
        mTrainingGrounds = Utils::Clamp(reader.ReadLine<UChar>(), 0, 5);
        mMedia = Utils::Clamp(reader.ReadLine<UChar>(), 0, 5);
        mResearch = Utils::Clamp(reader.ReadLine<UChar>(), 0, 5);
        if (reader.IsVersionGreaterOrEqual(0x2007, 0x0E)) {
            reader.ReadLine(mMediaPressure);
            reader.ReadLine(mAdditionalFlags);
        }
        reader.ReadLine(mLastSeasonFlags);
        mBadge.Read(reader);
        mClubColour.SetFromTable(mTeamColorsTable, reader.ReadLine<UChar>());
        reader.ReadLine(Unknown._3);
        mMerchandiseColour.SetFromTable(mMerchandiseColorsTable, reader.ReadLine<UChar>());
        if (reader.IsVersionGreaterOrEqual(0x2007, 0x0E)) {
            mHeaderColour.SetFromTable(m08InterfaceColorsTable, reader.ReadLine<UChar>());
            mBackgroundColour.SetFromTable(m08InterfaceColorsTable, reader.ReadLine<UChar>());
        }
        else {
            mHeaderColour.SetFromTable(m07InterfaceColorsTable, reader.ReadLine<UChar>());
            mBackgroundColour.SetFromTable(m07InterfaceColorsTable, reader.ReadLine<UChar>());
        }
        mHistory.Read(reader);
        mGeoCoords.mLatitude.SetFromInt(reader.ReadLine<UInt>());
        mGeoCoords.mLongitude.SetFromInt(reader.ReadLine<UInt>());
        reader.ReadLine(mNationalPrestige);
        reader.ReadLine(mInternationalPrestige);
        reader.ReadFullLine(Unknown._4);
        reader.ReadLine(mTransfersCountry1);
        reader.ReadLine(mTransfersCountry2);
        reader.ReadLine(mYouthPlayersCountry);
        reader.ReadLine(mFifaID);
        UInt playersCount = reader.ReadLine<UInt>();
        for (UInt i = 0; i < playersCount; i++) {
            UInt id = reader.ReadLine<UInt>();
            auto player = mCountry->mDatabase->CreatePlayer(this, id);
            player->Read(reader);
        }
        UInt staffsCount = reader.ReadLine<UInt>();
        for (UInt i = 0; i < staffsCount; i++) {
            UInt id = reader.ReadLine<UInt>();
            auto staff = mCountry->mDatabase->CreateStaff(this, id);
            staff->Read(reader);
        }
        mKit.Read(reader);
        for (UInt i = 0; i < 4; i++)
            mLowestLeagues[i].SetFromInt(reader.ReadLine<UInt>());

        reader.ReadEndIndex(clubSectionName);

        //CheckEnum(mStadiumType);
        CheckEnum(mAiStrategy);
        CheckEnum(mLandscape);
        CheckEnum(mSettlement);
        CheckEnum(mMediaPressure);
        for (UInt i = 0; i < 4; i++) {
            CheckEnum(mLowestLeagues[i].mRegion);
            CheckEnum(mLowestLeagues[i].mType);
        }
    }
}

void FifamClub::Write(FifamWriter &writer, UInt id) {
    String clubSectionName = Utils::Format(L"CLUB%u", id);
    writer.WriteStartIndex(clubSectionName);
    writer.WriteVersion();
    writer.WriteEndIndex(clubSectionName);
}
