#include "FifamClub.h"
#include "FifamCountry.h"
#include "FifamDatabase.h"
#include "FifamUtils.h"

void FifamClub::ReadClubMembers(FifamReader &reader) {
    UInt playersCount = reader.ReadLine<UInt>();
    for (UInt i = 0; i < playersCount; i++) {
        UInt id = reader.ReadLine<UInt>();
        auto player = mCountry->mDatabase->CreatePlayer(this, id);
        player->Read(reader, mDatabase);
    }
    UInt staffsCount = reader.ReadLine<UInt>();
    for (UInt i = 0; i < staffsCount; i++) {
        UInt id = reader.ReadLine<UInt>();
        auto staff = mCountry->mDatabase->CreateStaff(this, id);
        staff->Read(reader);
    }
}

void FifamClub::WriteClubMembers(FifamWriter &writer) {
    writer.WriteLine(mPlayers.size());
    for (auto player : mPlayers) {
        writer.WriteLine(player->mID);
        player->Write(writer, mDatabase);
    }
    //writer.WriteLine(mStaffs.size());
    //for (auto staff : mStaffs) {
    //    writer.WriteLine(staff->mID);
    //    staff->Write(writer);
    //}
    writer.WriteLine(0);
}

void FifamClub::Read(FifamReader &reader, UInt id) {
    String clubSectionName = Utils::Format(L"CLUB%u", id);
    if (reader.ReadStartIndex(clubSectionName)) {
        reader.ReadVersion();
        UChar clubFlags = 0;
        UShort lastSeasonFlags = 0;
        UInt clubAdditionalFlags = 0;
        if (reader.IsVersionGreaterOrEqual(0x2012, 0x01)) {
            reader.ReadLine(mUniqueID);
            reader.ReadLine(mFifaID);
            reader.ReadLineTranslationArray(mName);
            reader.ReadLineTranslationArray(mName2);
            reader.ReadLineTranslationArray(mShortName);
            reader.ReadLineTranslationArray(mShortName2);
            reader.ReadLineTranslationArray(mAbbreviation);
            reader.ReadLineTranslationArray(mCityName);
            reader.ReadLineTranslationArray(mPlayerInText);
            reader.ReadLineTranslationArray(mTermForTeam1);
            reader.ReadLineTranslationArray(mTermForTeam2);
            reader.ReadLineTranslationArray(mFanName1);
            reader.ReadLineTranslationArray(mFanName2);
            reader.ReadLineTranslationArray(mStadiumName);
            reader.ReadLineTranslationArray(mAbbreviationArticle);
            reader.ReadLineTranslationArray(mPlayerInTextArticle);
            reader.ReadLineTranslationArray(mTermForTeam1Article);
            reader.ReadLineTranslationArray(mTermForTeam2Article);
            reader.ReadLineTranslationArray(mFanName1Article);
            reader.ReadLineTranslationArray(mFanName2Article);
            reader.ReadLineTranslationArray(mClubNameUsageInPhrase);
            reader.ReadLineTranslationArray(mClubNameUsageInPhrase2);
            reader.ReadFullLine(mAddress);
            reader.ReadFullLine(mTelephone);
            reader.ReadFullLine(mWebsiteAndMail);
            reader.ReadFullLine(mFansites);
            reader.ReadFullLine(mNewspaper);
            reader.ReadFullLine(mMascotName);
            mBadge.Read(reader);
            UInt partnershipClub;
            Array<UInt, 4> rivalClubs;
            reader.ReadLine(partnershipClub, rivalClubs[0], rivalClubs[1], rivalClubs[2], rivalClubs[3]);
            FifamUtils::SaveClubIDToClubLink(mPartnershipClub, partnershipClub);
            for (UInt i = 0; i < rivalClubs.size(); i++)
                FifamUtils::SaveClubIDToClubLink(mRivalClubs[i], rivalClubs[i]);
            reader.ReadLine(mNationalPrestige, mInternationalPrestige);
            Int latitude, longitude;
            reader.ReadLine(latitude, longitude);
            mGeoCoords.Set(latitude, longitude);
            reader.ReadLine(mYearOfFoundation);
            reader.ReadLine(mInitialCapital);
            reader.ReadLine(mTransferBudget);
            reader.ReadLine(mJointStockCompany);
            reader.ReadLine(Unknown._2);
            reader.ReadLine(mPenaltyType, mPenaltyPoints);
            reader.ReadLine(mSponsorAmount, mSponsorDuration, mSpecialSponsor);
            reader.ReadLine(mPotentialFansCount, mAverageAttendanceLastSeason, mCountOfSoldSeasonTickets);
            reader.ReadLine(mFanMembers);
            if (reader.IsVersionGreaterOrEqual(0x2013, 0x06))
                reader.ReadLineArray(mPreferredFormations);
            reader.ReadLineArray(mLeagueTotalPoints);
            reader.ReadLineArray(mLeagueTotalWins);
            reader.ReadLineArray(mLeagueTotalDraws);
            reader.ReadLineArray(mLeagueTotalLoses);
            reader.ReadLineArray(mLeagueTotalGoals);
            reader.ReadLineArray(mLeagueTotalGoalsAgainst);
            reader.ReadLineArray(mLeagueTotalLeadershipsInTable);
            reader.ReadLine(mStadiumVenue, mStadiumStandingsCapacity, mStadiumSeatsCapacity, mStadiumVipCapacity, mStadiumType, clubFlags);
            reader.ReadLine(mMediaPressure);
            reader.ReadLine(mIndividualTvMoney);
            reader.ReadLine(clubAdditionalFlags);
            reader.ReadLine(lastSeasonFlags);
            Array<UChar, 5> clubColors;
            reader.ReadLineArray(clubColors);
            mClubColour.SetFromTable(mTeamColorsTable, clubColors[0]);
            mClubColour2.SetFromTable(mTeamColorsTable, clubColors[1]);
            mMerchandiseColour.SetFromTable(mMerchandiseColorsTable, clubColors[2]);
            mHeaderColour.SetFromTable(m08InterfaceColorsTable, clubColors[3]);
            mBackgroundColour.SetFromTable(m08InterfaceColorsTable, clubColors[4]);
            mHistory.Read(reader);
            reader.ReadLineArray(mTransfersCountry);
            reader.ReadLine(mYouthPlayersCountry);
            mKit.Read(reader);
            Array<UInt, 4> lowestLeagues;
            reader.ReadLineArray(lowestLeagues);
            for (UInt i = 0; i < 4; i++)
                mLowestLeagues[i].SetFromInt(lowestLeagues[i]);
            reader.ReadLine(mClubFacilities, mYouthCentre, mYouthBoardingSchool, mAiStrategy, mLandscape, mSettlement);
            mMedicalDepartment = mClubFacilities;
            mMerchandising = mClubFacilities;
            mTrainingGrounds = mClubFacilities;
            mMedia = mClubFacilities;
            mResearch = mClubFacilities;
            ReadClubMembers(reader);
            if (reader.IsVersionGreaterOrEqual(0x2013, 0x0A)) {
                Array<UInt, 3> captainIDs;
                reader.ReadLineArray(captainIDs);
                for (UInt i = 0; i < captainIDs.size(); i++)
                    FifamUtils::SavePlayerIDToPlayerPtr(mCaptains[i], captainIDs[i]);
            }
        }
        else {
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
            FifamUtils::SaveClubIDToClubLink(mPartnershipClub, reader.ReadLine<UInt>());
            for (UInt i = 0; i < 4; i++)
                FifamUtils::SaveClubIDToClubLink(mRivalClubs[i], reader.ReadLine<UInt>());
            reader.ReadLine(mYearOfFoundation);
            reader.ReadFullLine(mAddress);
            reader.ReadFullLine(mTelephone);
            reader.ReadFullLine(mWebsiteAndMail);
            reader.ReadFullLine(mFansites);
            reader.ReadFullLine(mNewspaper);
            reader.ReadFullLine(mMascotName);
            reader.ReadLine(mInitialCapital);
            reader.ReadLine(mTransferBudget);
            reader.ReadLine(mJointStockCompany);
            reader.ReadLine(Unknown._2);
            reader.ReadLine(mPenaltyType);
            if (reader.IsVersionGreaterOrEqual(0x2011, 0x0B)) {
                reader.ReadLine(mSponsorAmount);
                reader.ReadLine(mSponsorDuration);
                reader.ReadLine(mSpecialSponsor);
            }
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
            reader.ReadLine(mStadiumSeatsCapacity);
            reader.ReadLineTranslationArray(mStadiumName);
            reader.ReadLine(mStadiumType);
            reader.ReadLine(clubFlags);
            if (!reader.IsVersionGreaterOrEqual(0x2011, 0x0D)) {
                mClubFacilities = Utils::Clamp(reader.ReadLine<UChar>(), 0, 5) + 1;
                mMedicalDepartment = Utils::Clamp(reader.ReadLine<UChar>(), 0, 5);
                mMerchandising = Utils::Clamp(reader.ReadLine<UChar>(), 0, 5);
                UChar youthDepartment = reader.ReadLine<UChar>();
                mYouthBoardingSchool = Utils::MapTo(youthDepartment, 0, 5, 0, 10);
                mYouthCentre = Utils::MapTo(youthDepartment, 0, 5, 1, 10);
                mTrainingGrounds = Utils::Clamp(reader.ReadLine<UChar>(), 0, 5);
                mMedia = Utils::Clamp(reader.ReadLine<UChar>(), 0, 5);
                mResearch = Utils::Clamp(reader.ReadLine<UChar>(), 0, 5);
                if (mStadiumSeatsCapacity <= 50000) {
                    if (mStadiumSeatsCapacity > 20000)
                        mSettlement = FifamClubSettlement::Town;
                    else
                        mSettlement = FifamClubSettlement::Village;
                }
                else
                    mSettlement = FifamClubSettlement::City;
            }
            if (reader.IsVersionGreaterOrEqual(0x2007, 0x0E)) {
                reader.ReadLine(mMediaPressure);
                if (reader.IsVersionGreaterOrEqual(0x2011, 0x0C))
                    reader.ReadLine(mIndividualTvMoney);
                reader.ReadLine(clubAdditionalFlags);
            }
            reader.ReadLine(lastSeasonFlags);
            mBadge.Read(reader);
            mClubColour.SetFromTable(mTeamColorsTable, reader.ReadLine<UChar>());
            mClubColour2.SetFromTable(mTeamColorsTable, reader.ReadLine<UChar>());
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
            reader.ReadLine(mTransfersCountry[0]);
            reader.ReadLine(mTransfersCountry[1]);
            reader.ReadLine(mYouthPlayersCountry);
            reader.ReadLine(mFifaID);
            ReadClubMembers(reader);
            mKit.Read(reader);
            for (UInt i = 0; i < 4; i++)
                mLowestLeagues[i].SetFromInt(reader.ReadLine<UInt>());
            if (reader.IsVersionGreaterOrEqual(0x2011, 0x0D)) {
                reader.ReadLine(mClubFacilities);
                reader.ReadLine(mYouthCentre);
                reader.ReadLine(mYouthBoardingSchool);
                reader.ReadLine(mAiStrategy);
                reader.ReadLine(mLandscape);
                reader.ReadLine(mSettlement);
                mMedicalDepartment = mClubFacilities;
                mMerchandising = mClubFacilities;
                mTrainingGrounds = mClubFacilities;
                mMedia = mClubFacilities;
                mResearch = mClubFacilities;
            }
        }
        reader.ReadEndIndex(clubSectionName);

        if (clubFlags & 1)
            mStadiumBigMainStand = true;
        if (clubFlags & 2)
            Unknown.flags._2 = true;
        if (clubFlags & 4)
            Unknown.flags._4 = true;
        if (clubFlags & 8)
            Unknown.flags._8 = true;
        if (clubFlags & 16)
            Unknown.flags._16 = true;
        if (clubFlags & 32)
            Unknown.flags._32 = true;
        if (clubFlags & 64)
            Unknown.flags._64 = true;
        if (clubFlags & 128)
            mCanBeDeletedInEditor = true;

        if (clubAdditionalFlags & 0x4000)
            mTraditionalClub = true;
        if (clubAdditionalFlags & 0x100000)
            mCantPlayInLeague = true;
        if (clubAdditionalFlags & 0x400000)
            mRichGuyControlled = true;
        if (clubAdditionalFlags & 0x1000000)
            mYouthPlayersAreBasques = true;

        if (lastSeasonFlags & 1)
            mFirstTeamLastSeasonInfo.mLeague = FifamClubLastSeasonLeague::Promoted;
        else if (lastSeasonFlags & 4)
            mFirstTeamLastSeasonInfo.mLeague = FifamClubLastSeasonLeague::Relegated;
        if (lastSeasonFlags & 16)
            mFirstTeamLastSeasonInfo.mCup = FifamClubLastSeasonCup::Winner;
        else if (lastSeasonFlags & 64)
            mFirstTeamLastSeasonInfo.mCup = FifamClubLastSeasonCup::RunnerUp;
        if (lastSeasonFlags & 2)
            mReserveTeamLastSeasonInfo.mLeague = FifamClubLastSeasonLeague::Promoted;
        else if (lastSeasonFlags & 8)
            mReserveTeamLastSeasonInfo.mLeague = FifamClubLastSeasonLeague::Relegated;
        if (lastSeasonFlags & 32)
            mReserveTeamLastSeasonInfo.mCup = FifamClubLastSeasonCup::Winner;
        else if (lastSeasonFlags & 128)
            mReserveTeamLastSeasonInfo.mCup = FifamClubLastSeasonCup::RunnerUp;
        if (lastSeasonFlags & 256)
            mDoesNotTakePartInITCup = true;

        //FifamCheckEnum(mStadiumType);
        FifamCheckEnum(mAiStrategy);
        FifamCheckEnum(mLandscape);
        FifamCheckEnum(mSettlement);
        FifamCheckEnum(mMediaPressure);
        for (UInt i = 0; i < 4; i++) {
            FifamCheckEnum(mLowestLeagues[i].mRegion);
            FifamCheckEnum(mLowestLeagues[i].mType);
        }
        //FifamCheckEnum(mPenaltyType);
    }
}

void FifamClub::Write(FifamWriter &writer, UInt id) {
    UChar clubFlags = 0;
    if (mStadiumBigMainStand)
        clubFlags |= 1;
    if (Unknown.flags._2)
        clubFlags |= 2;
    if (Unknown.flags._4)
        clubFlags |= 4;
    if (Unknown.flags._8)
        clubFlags |= 8;
    if (Unknown.flags._16)
        clubFlags |= 16;
    if (Unknown.flags._32)
        clubFlags |= 32;
    if (Unknown.flags._64)
        clubFlags |= 64;
    if (mCanBeDeletedInEditor)
        clubFlags |= 128;

    UInt clubAdditionalFlags = 0;
    if (mTraditionalClub)
        clubAdditionalFlags |= 0x4000;
    if (mCantPlayInLeague)
        clubAdditionalFlags |= 0x100000;
    if (mRichGuyControlled)
        clubAdditionalFlags |= 0x400000;
    if (mYouthPlayersAreBasques)
        clubAdditionalFlags |= 0x1000000;

    UShort lastSeasonFlags = 0;
    if (mFirstTeamLastSeasonInfo.mLeague == FifamClubLastSeasonLeague::Promoted)
        lastSeasonFlags |= 1;
    else if (mFirstTeamLastSeasonInfo.mLeague == FifamClubLastSeasonLeague::Relegated)
        lastSeasonFlags |= 4;
    if (mFirstTeamLastSeasonInfo.mCup == FifamClubLastSeasonCup::Winner)
        lastSeasonFlags |= 16;
    else if (mFirstTeamLastSeasonInfo.mCup == FifamClubLastSeasonCup::RunnerUp)
        lastSeasonFlags |= 64;
    if (mReserveTeamLastSeasonInfo.mLeague == FifamClubLastSeasonLeague::Promoted)
        lastSeasonFlags |= 2;
    else if (mReserveTeamLastSeasonInfo.mLeague == FifamClubLastSeasonLeague::Relegated)
        lastSeasonFlags |= 8;
    if (mReserveTeamLastSeasonInfo.mCup == FifamClubLastSeasonCup::Winner)
        lastSeasonFlags |= 32;
    else if (mReserveTeamLastSeasonInfo.mCup == FifamClubLastSeasonCup::RunnerUp)
        lastSeasonFlags |= 128;
    if (mDoesNotTakePartInITCup)
        lastSeasonFlags |= 256;

    String clubSectionName = Utils::Format(L"CLUB%u", id);
    writer.WriteStartIndex(clubSectionName);
    writer.WriteVersion();
    if (writer.IsVersionGreaterOrEqual(0x2012, 0x01)) {
        writer.WriteLine(FifamUtils::DBTranslateClubID(mDatabase, mUniqueID, writer.GetGameId()));
        writer.WriteLine(mFifaID);
        writer.WriteLineTranslationArray(mName);
        writer.WriteLineTranslationArray(mName2);
        writer.WriteLineTranslationArray(mShortName);
        writer.WriteLineTranslationArray(mShortName2);
        writer.WriteLineTranslationArray(mAbbreviation);
        writer.WriteLineTranslationArray(mCityName);
        writer.WriteLineTranslationArray(mPlayerInText);
        writer.WriteLineTranslationArray(mTermForTeam1);
        writer.WriteLineTranslationArray(mTermForTeam2);
        writer.WriteLineTranslationArray(mFanName1);
        writer.WriteLineTranslationArray(mFanName2);
        writer.WriteLineTranslationArray(mStadiumName);
        writer.WriteLineTranslationArray(mAbbreviationArticle);
        writer.WriteLineTranslationArray(mPlayerInTextArticle);
        writer.WriteLineTranslationArray(mTermForTeam1Article);
        writer.WriteLineTranslationArray(mTermForTeam2Article);
        writer.WriteLineTranslationArray(mFanName1Article);
        writer.WriteLineTranslationArray(mFanName2Article);
        writer.WriteLineTranslationArray(mClubNameUsageInPhrase);
        writer.WriteLineTranslationArray(mClubNameUsageInPhrase2);
        writer.WriteLine(mAddress);
        writer.WriteLine(mTelephone);
        writer.WriteLine(mWebsiteAndMail);
        writer.WriteLine(mFansites);
        writer.WriteLine(mNewspaper);
        writer.WriteLine(mMascotName);
        mBadge.Write(writer);
        writer.WriteLine(FifamUtils::DBClubLinkToID(mDatabase, mPartnershipClub, writer.GetGameId()),
            FifamUtils::DBClubLinkToID(mDatabase, mRivalClubs[0], writer.GetGameId()),
            FifamUtils::DBClubLinkToID(mDatabase, mRivalClubs[1], writer.GetGameId()),
            FifamUtils::DBClubLinkToID(mDatabase, mRivalClubs[2], writer.GetGameId()),
            FifamUtils::DBClubLinkToID(mDatabase, mRivalClubs[3], writer.GetGameId()));
        writer.WriteLine(mNationalPrestige, mInternationalPrestige);
        writer.WriteLine(mGeoCoords.mLatitude.ToInt(), mGeoCoords.mLongitude.ToInt());
        writer.WriteLine(mYearOfFoundation);
        writer.WriteLine(mInitialCapital);
        writer.WriteLine(mTransferBudget);
        writer.WriteLine(mJointStockCompany);
        writer.WriteLine(Unknown._2);
        writer.WriteLine(mPenaltyType, mPenaltyPoints);
        writer.WriteLine(mSponsorAmount, mSponsorDuration, mSpecialSponsor);
        writer.WriteLine(mPotentialFansCount, mAverageAttendanceLastSeason, mCountOfSoldSeasonTickets);
        writer.WriteLine(mFanMembers);
        if (writer.IsVersionGreaterOrEqual(0x2013, 0x06))
            writer.WriteLineArray(mPreferredFormations);
        writer.WriteLineArray(mLeagueTotalPoints);
        writer.WriteLineArray(mLeagueTotalWins);
        writer.WriteLineArray(mLeagueTotalDraws);
        writer.WriteLineArray(mLeagueTotalLoses);
        writer.WriteLineArray(mLeagueTotalGoals);
        writer.WriteLineArray(mLeagueTotalGoalsAgainst);
        writer.WriteLineArray(mLeagueTotalLeadershipsInTable);
        writer.WriteLine(mStadiumVenue, mStadiumStandingsCapacity, mStadiumSeatsCapacity, mStadiumVipCapacity, mStadiumType, clubFlags);
        writer.WriteLine(mMediaPressure);
        writer.WriteLine(mIndividualTvMoney);
        writer.WriteLine(clubAdditionalFlags);
        writer.WriteLine(lastSeasonFlags);
        writer.WriteLine(
            mClubColour.FindIndexInTable(mTeamColorsTable),
            mClubColour2.FindIndexInTable(mTeamColorsTable),
            mMerchandiseColour.FindIndexInTable(mMerchandiseColorsTable),
            mHeaderColour.FindIndexInTable(m08InterfaceColorsTable),
            mBackgroundColour.FindIndexInTable(m08InterfaceColorsTable));
        mHistory.Write(writer);
        writer.WriteLineArray(mTransfersCountry);
        writer.WriteLine(mYouthPlayersCountry);
        mKit.Write(writer);
        writer.WriteLine(
            mLowestLeagues[0].ToInt(),
            mLowestLeagues[1].ToInt(),
            mLowestLeagues[2].ToInt(),
            mLowestLeagues[3].ToInt());
        writer.WriteLine(mClubFacilities, mYouthCentre, mYouthBoardingSchool, mAiStrategy, mLandscape, mSettlement);
        WriteClubMembers(writer);
        if (writer.IsVersionGreaterOrEqual(0x2013, 0x0A)) {
            Array<UInt, 3> captainIDs;
            for (UInt i = 0; i < 3; i++)
                captainIDs[i] = mCaptains[i] ? mCaptains[i]->mID : 0;
            writer.WriteLineArray(captainIDs);
        }
    }
    else {
        if (writer.IsVersionGreaterOrEqual(0x2007, 0x12))
            writer.WriteLine(mUniqueID);
        writer.WriteLineTranslationArray(mName);
        writer.WriteLineTranslationArray(mName2);
        writer.WriteLineTranslationArray(mShortName);
        writer.WriteLineTranslationArray(mShortName2);
        writer.WriteLineTranslationArray(mAbbreviationArticle);
        writer.WriteLineTranslationArray(mPlayerInTextArticle);
        if (writer.IsVersionGreaterOrEqual(0x2011, 0x05)) {
            writer.WriteLineTranslationArray(mTermForTeam1Article);
            writer.WriteLineTranslationArray(mTermForTeam2Article);
            writer.WriteLineTranslationArray(mFanName1Article);
            writer.WriteLineTranslationArray(mFanName2Article);
        }
        else
            writer.WriteLineTranslationArray(mFanName1Article);
        writer.WriteLineTranslationArray(mAbbreviation);
        writer.WriteLineTranslationArray(mClubNameUsageInPhrase);
        writer.WriteLineTranslationArray(mClubNameUsageInPhrase2);
        writer.WriteLineTranslationArray(mCityName);
        if (writer.IsVersionGreaterOrEqual(0x2011, 0x09)) {
            writer.WriteLineTranslationArray(mPlayerInText);
            writer.WriteLineTranslationArray(mTermForTeam1);
            writer.WriteLineTranslationArray(mTermForTeam2);
            writer.WriteLineTranslationArray(mFanName1);
            writer.WriteLineTranslationArray(mFanName2);
        }
        else {
            if (writer.IsVersionGreaterOrEqual(0x2011, 0x06)) {
                writer.WriteLine(mPlayerInText[0]);
                writer.WriteLine(mTermForTeam1[0]);
                writer.WriteLine(mTermForTeam2[0]);
                writer.WriteLine(mFanName1[0]);
                writer.WriteLine(mFanName2[0]);
            }
            else {
                writer.WriteLine(mPlayerInText[0]);
                writer.WriteLine(mFanName1[0]);
            }
        }
        writer.WriteLine(FifamUtils::DBClubLinkToID(mDatabase, mPartnershipClub, writer.GetGameId()));
        for (UInt i = 0; i < 4; i++)
            writer.WriteLine(FifamUtils::DBClubLinkToID(mDatabase, mRivalClubs[i], writer.GetGameId()));
        writer.WriteLine(mYearOfFoundation);
        writer.WriteLine(mAddress);
        writer.WriteLine(mTelephone);
        writer.WriteLine(mWebsiteAndMail);
        writer.WriteLine(mFansites);
        writer.WriteLine(mNewspaper);
        writer.WriteLine(mMascotName);
        writer.WriteLine(mInitialCapital);
        writer.WriteLine(mTransferBudget);
        writer.WriteLine(mJointStockCompany);
        writer.WriteLine(Unknown._2);
        writer.WriteLine(mPenaltyType);
        if (writer.IsVersionGreaterOrEqual(0x2011, 0x0B)) {
            writer.WriteLine(mSponsorAmount);
            writer.WriteLine(mSponsorDuration);
            writer.WriteLine(mSpecialSponsor);
        }
        writer.WriteLine(mPenaltyPoints);
        writer.WriteLine(mPotentialFansCount);
        writer.WriteLine(mAverageAttendanceLastSeason);
        writer.WriteLine(mCountOfSoldSeasonTickets);
        UInt numLeagues = 1;
        if (writer.IsVersionGreaterOrEqual(0x2009, 0x06))
            numLeagues = 3;
        else if (writer.IsVersionGreaterOrEqual(0x2007, 0x14))
            numLeagues = 2;
        for (UInt i = 0; i < numLeagues; i++)
            writer.WriteLine(mLeagueTotalPoints[i]);
        for (UInt i = 0; i < numLeagues; i++)
            writer.WriteLine(mLeagueTotalWins[i]);
        for (UInt i = 0; i < numLeagues; i++)
            writer.WriteLine(mLeagueTotalDraws[i]);
        for (UInt i = 0; i < numLeagues; i++)
            writer.WriteLine(mLeagueTotalLoses[i]);
        for (UInt i = 0; i < numLeagues; i++)
            writer.WriteLine(mLeagueTotalGoals[i]);
        for (UInt i = 0; i < numLeagues; i++)
            writer.WriteLine(mLeagueTotalGoalsAgainst[i]);
        for (UInt i = 0; i < numLeagues; i++)
            writer.WriteLine(mLeagueTotalLeadershipsInTable[i]);
        writer.WriteLine(mStadiumVenue);
        writer.WriteLine(mStadiumSeatsCapacity);
        writer.WriteLineTranslationArray(mStadiumName);
        writer.WriteLine(mStadiumType);
        writer.WriteLine(clubFlags);
        if (!writer.IsVersionGreaterOrEqual(0x2011, 0x0D)) {
            writer.WriteLine(Utils::MapTo(mClubFacilities, 0, 6, 0, 5));
            writer.WriteLine(mMedicalDepartment);
            writer.WriteLine(mMerchandising);
            writer.WriteLine(Utils::MapTo(mYouthCentre, 1, 10, 0, 5));
            writer.WriteLine(mTrainingGrounds);
            writer.WriteLine(mMedia);
            writer.WriteLine(mResearch);
        }
        if (writer.IsVersionGreaterOrEqual(0x2007, 0x0E)) {
            writer.WriteLine(mMediaPressure);
            if (writer.IsVersionGreaterOrEqual(0x2011, 0x0C))
                writer.WriteLine(mIndividualTvMoney);
            writer.WriteLine(clubAdditionalFlags);
        }
        writer.WriteLine(lastSeasonFlags);
        mBadge.Write(writer);
        writer.WriteLine(mClubColour.FindIndexInTable(mTeamColorsTable));
        writer.WriteLine(mClubColour2.FindIndexInTable(mTeamColorsTable));
        writer.WriteLine(mMerchandiseColour.FindIndexInTable(mMerchandiseColorsTable));
        if (writer.IsVersionGreaterOrEqual(0x2007, 0x0E)) {
            writer.WriteLine(mHeaderColour.FindIndexInTable(m08InterfaceColorsTable));
            writer.WriteLine(mBackgroundColour.FindIndexInTable(m08InterfaceColorsTable));
        }
        else {
            writer.WriteLine(mHeaderColour.FindIndexInTable(m07InterfaceColorsTable));
            writer.WriteLine(mBackgroundColour.FindIndexInTable(m07InterfaceColorsTable));
        }
        mHistory.Write(writer);
        writer.WriteLine(mGeoCoords.mLatitude.ToInt());
        writer.WriteLine(mGeoCoords.mLongitude.ToInt());
        writer.WriteLine(mNationalPrestige);
        writer.WriteLine(mInternationalPrestige);
        writer.WriteLine(Unknown._4);
        writer.WriteLine(mTransfersCountry[0]);
        writer.WriteLine(mTransfersCountry[1]);
        writer.WriteLine(mYouthPlayersCountry);
        writer.WriteLine(mFifaID);
        WriteClubMembers(writer);
        mKit.Write(writer);
        for (UInt i = 0; i < 4; i++)
            writer.WriteLine(mLowestLeagues[i].ToInt());
        if (writer.IsVersionGreaterOrEqual(0x2011, 0x0D)) {
            writer.WriteLine(mClubFacilities);
            writer.WriteLine(mYouthCentre);
            writer.WriteLine(mYouthBoardingSchool);
            writer.WriteLine(mAiStrategy);
            writer.WriteLine(mLandscape);
            writer.WriteLine(mSettlement);
        }
    }
    writer.WriteEndIndex(clubSectionName);
}
