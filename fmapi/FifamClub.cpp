#include "FifamClub.h"
#include "FifamCountry.h"
#include "FifamDatabase.h"
#include "FifamUtils.h"
#include "FifamNames.h"

void FifamClub::ReadClubMembers(FifamReader &reader) {
    UInt playersCount = reader.ReadLine<UInt>();
    for (UInt i = 0; i < playersCount; i++) {
        UInt id = reader.ReadLine<UInt>();
        if (FifamDatabase::mReadingOptions.mReadPersons) {
            auto player = mCountry->mDatabase->CreatePlayer(this, id);
            player->Read(reader);
        }
        else {
            FifamPlayer dummyPlayer;
            dummyPlayer.Read(reader);
        }
    }
    if (reader.IsVersionGreaterOrEqual(0x2006, 1)) {
        UInt staffsCount = reader.ReadLine<UInt>();
        for (UInt i = 0; i < staffsCount; i++) {
            UInt id = reader.ReadLine<UInt>();
            if (FifamDatabase::mReadingOptions.mReadPersons) {
                auto staff = mCountry->mDatabase->CreateStaff(this, id);
                staff->Read(reader);
            }
            else {
                FifamStaff dummyStaff;
                dummyStaff.Read(reader);
            }
        }
    }
}

void FifamClub::WriteClubMembers(FifamWriter &writer) {
    Vector<FifamPlayer *> players;
    for (auto player : mPlayers) {
        if (player->GetIsWriteable())
            players.push_back(player);
    }
    writer.WriteLine(players.size());
    for (auto player : players) {
        writer.WriteLine(FifamUtils::GetWriteableID(player));
        player->Write(writer);
    }
    if (writer.IsVersionGreaterOrEqual(0x2006, 1)) {
        Vector<FifamStaff *> staffs;
        for (auto staff : mStaffs) {
            if (staff->GetIsWriteable())
                staffs.push_back(staff);
        }
        writer.WriteLine(staffs.size());
        for (auto staff : staffs) {
            writer.WriteLine(FifamUtils::GetWriteableID(staff));
            staff->Write(writer);
        }
    }
}

void FifamClub::Read(FifamReader &reader, UInt id) {
    String clubSectionName = Utils::Format(L"CLUB%u", id);
    if (reader.ReadStartIndex(clubSectionName)) {
        if (!FifamDatabase::mReadingOptions.mReadClubs && id != 0xFFFF) {
            reader.ReadEndIndex(clubSectionName);
            return;
        }
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
            Vector<UInt> rivalClubs(4);
            reader.ReadLine(partnershipClub, rivalClubs[0], rivalClubs[1], rivalClubs[2], rivalClubs[3]);
            FifamUtils::SaveClubIDToClubLink(mPartnershipClub, partnershipClub);
            FifamUtils::SaveIDsToClubLinkList(rivalClubs, mRivalClubs);
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
            if (reader.IsVersionGreaterOrEqual(0x2013, 0x06)) {
                if (FifamDatabase::mReadingOptions.mUseCustomFormations) {
                    mPreferredFormations[0] = FifamFormation::None;
                    mPreferredFormations[1] = FifamFormation::None;
                    Array<Int, 2> customFormations = {};
                    reader.ReadLineArray(customFormations);
                    SetProperty<Int>(L"custom_formation_1", customFormations[0]);
                    SetProperty<Int>(L"custom_formation_2", customFormations[1]);
                }
                else
                    reader.ReadLineArray(mPreferredFormations); 
            }
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
            auto lowestLeagues = reader.ReadLineArray<UInt>();
            FifamUtils::SaveIDsToCompetitionList(lowestLeagues, mLowestLeagues);
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
                    FifamUtils::SavePlayerIDToPtr(mCaptains[i], captainIDs[i]);
            }
        }
        else {
            if (reader.IsVersionGreaterOrEqual(0x2007, 0x12))
                reader.ReadLine(mUniqueID);
            else
                mUniqueID = id | (mCountry->mId << 16);
            reader.ReadLineTranslationArray(mName);
            if (reader.IsVersionGreaterOrEqual(0x2006, 0x3))
                reader.ReadLineTranslationArray(mName2);
            else
                mName2 = mName;
            reader.ReadLineTranslationArray(mShortName);
            if (reader.IsVersionGreaterOrEqual(0x2006, 0x3))
                reader.ReadLineTranslationArray(mShortName2);
            else
                mShortName2 = mShortName;
            if (reader.IsVersionGreaterOrEqual(0x2005, 0x0) && !reader.IsVersionGreaterOrEqual(0x2006, 0x0))
                reader.ReadLineTranslationArray(mAbbreviation5Letters);
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
            if (reader.IsVersionGreaterOrEqual(0x2006, 0x3))
                reader.ReadLineTranslationArray(mClubNameUsageInPhrase2);
            else
                mClubNameUsageInPhrase2 = mClubNameUsageInPhrase;
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
            mRivalClubs.resize(2);
            for (UInt i = 0; i < 2; i++)
                FifamUtils::SaveClubIDToClubLink(mRivalClubs[i], reader.ReadLine<UInt>());
            if (reader.IsVersionGreaterOrEqual(0x2006, 1)) {
                mRivalClubs.resize(mRivalClubs.size() + 1);
                FifamUtils::SaveClubIDToClubLink(mRivalClubs[mRivalClubs.size() - 1], reader.ReadLine<UInt>());
            }
            if (reader.IsVersionGreaterOrEqual(0x2006, 3)) {
                mRivalClubs.resize(mRivalClubs.size() + 1);
                FifamUtils::SaveClubIDToClubLink(mRivalClubs[mRivalClubs.size() - 1], reader.ReadLine<UInt>());
            }
            reader.ReadLine(mYearOfFoundation);
            reader.ReadFullLine(mAddress);
            reader.ReadFullLine(mTelephone);
            reader.ReadFullLine(mWebsiteAndMail);
            reader.ReadFullLine(mFansites);
            reader.ReadFullLine(mNewspaper);
            reader.ReadFullLine(mMascotName);
            reader.ReadLine(mInitialCapital);
            if (reader.IsVersionGreaterOrEqual(0x2006, 0x01))
                reader.ReadLine(mTransferBudget);
            reader.ReadLine(mJointStockCompany);
            if (reader.IsVersionGreaterOrEqual(0x2005, 0x04))
                reader.ReadLine(Unknown._2);
            if (reader.IsVersionGreaterOrEqual(0x2007, 0x09))
                reader.ReadLine(mPenaltyType);
            else {
                reader.ReadLine(mPenaltyPoints);
                if (mPenaltyPoints != 0) {
                    mPenaltyPoints = -mPenaltyPoints;
                    mPenaltyType = FifamClubPenaltyType::Points;
                }
            }
            if (reader.IsVersionGreaterOrEqual(0x2011, 0x0B)) {
                reader.ReadLine(mSponsorAmount);
                reader.ReadLine(mSponsorDuration);
                reader.ReadLine(mSpecialSponsor);
            }
            if (reader.IsVersionGreaterOrEqual(0x2007, 0x09))
                reader.ReadLine(mPenaltyPoints);
            if (!reader.IsVersionGreaterOrEqual(0x2005, 0x00))
                reader.SkipLines(2);
            if (reader.GetGameId() >= 9)
                reader.ReadLine(mPotentialFansCount);
            else
                reader.ReadLine(mMaxAttendance);
            reader.ReadLine(mAverageAttendanceLastSeason);
            reader.ReadLine(mCountOfSoldSeasonTickets);
            UInt numLeagues = 1;
            if (reader.IsVersionGreaterOrEqual(0x2009, 0x06))
                numLeagues = 3;
            else if (reader.IsVersionGreaterOrEqual(0x2007, 0x14))
                numLeagues = 2;
            for (UInt i = 0; i < numLeagues; i++)
                reader.ReadLine(mLeagueTotalPoints[i]);
            if (reader.IsVersionGreaterOrEqual(0x2007, 0x06)) {
                for (UInt i = 0; i < numLeagues; i++)
                    reader.ReadLine(mLeagueTotalWins[i]);
                for (UInt i = 0; i < numLeagues; i++)
                    reader.ReadLine(mLeagueTotalDraws[i]);
                for (UInt i = 0; i < numLeagues; i++)
                    reader.ReadLine(mLeagueTotalLoses[i]);
            }
            else
                reader.ReadLine(mLeagueTotalMatches);
            for (UInt i = 0; i < numLeagues; i++)
                reader.ReadLine(mLeagueTotalGoals[i]);
            for (UInt i = 0; i < numLeagues; i++)
                reader.ReadLine(mLeagueTotalGoalsAgainst[i]);
            for (UInt i = 0; i < numLeagues; i++)
                reader.ReadLine(mLeagueTotalLeadershipsInTable[i]);
            reader.ReadLine(mStadiumVenue);
            reader.ReadLine(mStadiumSeatsCapacity);
            if (reader.IsVersionGreaterOrEqual(0x2005, 0x03)) {
                reader.ReadLineTranslationArray(mStadiumName);
                reader.ReadLine(mStadiumType);
                reader.ReadLine(clubFlags);
            }
            else {
                mCanBeDeletedInEditor = true;
            }
            if (!reader.IsVersionGreaterOrEqual(0x2011, 0x0D)) {
                mClubFacilities = Utils::MapTo(reader.ReadLine<UChar>(), 0, 5, 0, 6);
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
            if (reader.IsVersionGreaterOrEqual(0x2004, 0)) {
                mClubColour.SetFromTable(mTeamColorsTable, reader.ReadLine<UChar>());
                mClubColour2.SetFromTable(mTeamColorsTable, reader.ReadLine<UChar>());
                mMerchandiseColour.SetFromTable(mMerchandiseColorsTable, reader.ReadLine<UChar>());
                if (reader.IsVersionGreaterOrEqual(0x2007, 0x0E)) {
                    mHeaderColour.SetFromTable(m08InterfaceColorsTable, reader.ReadLine<UChar>());
                    mBackgroundColour.SetFromTable(m08InterfaceColorsTable, reader.ReadLine<UChar>());
                }
                else {
                    if (reader.IsVersionGreaterOrEqual(0x2007, 2)) {
                        mHeaderColour.SetFromTable(m07InterfaceColorsTable, reader.ReadLine<UChar>());
                        mBackgroundColour.SetFromTable(m07InterfaceColorsTable, reader.ReadLine<UChar>());
                    }
                    else {
                        mBackgroundColour = mClubColour.first;
                        mHeaderColour = mClubColour.second;
                    }
                }
            }
            mHistory.Read(reader);
            mGeoCoords.mLatitude.SetFromInt(reader.ReadLine<UInt>());
            mGeoCoords.mLongitude.SetFromInt(reader.ReadLine<UInt>());
            reader.ReadLine(mNationalPrestige);
            if (reader.IsVersionGreaterOrEqual(0x2006, 0x02))
                reader.ReadLine(mInternationalPrestige);
            reader.ReadFullLine(Unknown._4);
            reader.ReadLine(mTransfersCountry[0]);
            reader.ReadLine(mTransfersCountry[1]);
            reader.ReadLine(mYouthPlayersCountry);
            reader.ReadLine(mFifaID);
            if (reader.IsVersionGreaterOrEqual(0x2004, 0x04) && !reader.IsVersionGreaterOrEqual(0x2005, 0x03))
                reader.ReadLine(Unknown._5);
            ReadClubMembers(reader);
            mKit.Read(reader);
            if (reader.IsVersionGreaterOrEqual(0x2007, 2)) {
                mLowestLeagues.resize(4);
                for (UInt i = 0; i < 4; i++)
                    mLowestLeagues[i].SetFromInt(reader.ReadLine<UInt>());
            }
            else if (reader.IsVersionGreaterOrEqual(0x2007, 1)) {
                mLowestLeagues.resize(1);
                mLowestLeagues[0].SetFromInt(reader.ReadLine<UInt>());
            }
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

        if (reader.GetGameId() >= 9) {
            mMaxAttendance = mAverageAttendanceLastSeason * 2;
            UInt stadiumCapacity = mStadiumSeatsCapacity + mStadiumStandingsCapacity + mStadiumVipCapacity;
            if (mMaxAttendance > stadiumCapacity)
                mMaxAttendance = stadiumCapacity;
        }
        else {
            mPotentialFansCount = mAverageAttendanceLastSeason * 2;
            if (mInternationalPrestige >= 15)
                mPotentialFansCount += (mInternationalPrestige - 14) * 25'000;
            if (mPotentialFansCount > 250'000)
                mPotentialFansCount = 250'000;
        }
        if (!reader.IsVersionGreaterOrEqual(0x2005, 0x0) || reader.IsVersionGreaterOrEqual(0x2006, 0x0)) {
            for (UInt i = 1; i < FifamTranslation::NUM_TRANSLATIONS; i++) {
                if (!mShortName[i].empty())
                    mAbbreviation5Letters[i] = FifamNames::LimitName(mShortName[i], 5);
            }
        }

        // TODO: calculate international prestige for clubs

        //FifamCheckEnum(mStadiumType);
        FifamCheckEnum(mAiStrategy);
        FifamCheckEnum(mLandscape);
        FifamCheckEnum(mSettlement);
        //FifamCheckEnum(mMediaPressure);
        if (mMediaPressure.GetWasSetFromUnknown())
            Error(FifamTr(mName));
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
        writer.WriteLine(FifamUtils::GetWriteableUniqueID(this));
        writer.WriteLine(mFifaID);
        writer.WriteLineTranslationArray(FifamNames::TransformTrArray(mName, FifamNames::LimitName, 29));
        writer.WriteLineTranslationArray(FifamNames::TransformTrArray(mName2, FifamNames::LimitName, 29));
        writer.WriteLineTranslationArray(FifamNames::TransformTrArray(mShortName, FifamNames::LimitName, 10));
        writer.WriteLineTranslationArray(FifamNames::TransformTrArray(mShortName2, FifamNames::LimitName, 10));
        writer.WriteLineTranslationArray(FifamNames::TransformTrArray(mAbbreviation, FifamNames::LimitName, 4));
        writer.WriteLineTranslationArray(FifamNames::TransformTrArray(mCityName, FifamNames::LimitName, 29));
        writer.WriteLineTranslationArray(mPlayerInText);
        writer.WriteLineTranslationArray(mTermForTeam1);
        writer.WriteLineTranslationArray(mTermForTeam2);
        writer.WriteLineTranslationArray(mFanName1);
        writer.WriteLineTranslationArray(mFanName2);
        writer.WriteLineTranslationArray(FifamNames::TransformTrArray(mStadiumName, FifamNames::LimitName, 29));
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
        auto rivalClubs = FifamUtils::MakeWriteableIDsList(mRivalClubs);
        writer.WriteLine(FifamUtils::GetWriteableID(mPartnershipClub), rivalClubs[0], rivalClubs[1], rivalClubs[2], rivalClubs[3]);
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
        if (writer.IsVersionGreaterOrEqual(0x2013, 0x06)) {
            Array<Int, 2> customFormations;
            customFormations[0] = GetProperty<Int>(L"custom_formation_1", -1);
            if (customFormations[0] != -1) {
                customFormations[1] = GetProperty<Int>(L"custom_formation_2", 0);
                writer.WriteLineArray(customFormations);
            }
            else
                writer.WriteLineArray(mPreferredFormations);
        }
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
        FifamDbWriteableIDsList lowestLeaguesIDs;
        // TODO: check this one
        for (UInt i = 0; i < mLowestLeagues.size(); i++)
            lowestLeaguesIDs.push_back_unique(FifamUtils::GetWriteableID(mLowestLeagues[i], writer.GetGameId()));
        writer.WriteLineArray(lowestLeaguesIDs.get_array(4));
        writer.WriteLine(mClubFacilities, mYouthCentre, mYouthBoardingSchool, mAiStrategy, mLandscape, mSettlement);
        WriteClubMembers(writer);
        if (writer.IsVersionGreaterOrEqual(0x2013, 0x0A)) {
            Array<UInt, 3> captainIDs;
            for (UInt i = 0; i < 3; i++)
                captainIDs[i] = FifamUtils::GetWriteableID(mCaptains[i]);
            writer.WriteLineArray(captainIDs);
        }
    }
    else {
        if (writer.IsVersionGreaterOrEqual(0x2007, 0x12))
            writer.WriteLine(mUniqueID);
        writer.WriteLineTranslationArray(FifamNames::TransformTrArray(mName, FifamNames::LimitName, 29));
        if (writer.IsVersionGreaterOrEqual(0x2006, 0x3))
            writer.WriteLineTranslationArray(FifamNames::TransformTrArray(mName2, FifamNames::LimitName, 29));
        writer.WriteLineTranslationArray(FifamNames::TransformTrArray(mShortName, FifamNames::LimitName, 10));
        if (writer.IsVersionGreaterOrEqual(0x2006, 0x3))
            writer.WriteLineTranslationArray(FifamNames::TransformTrArray(mShortName2, FifamNames::LimitName, 10));
        if (writer.IsVersionGreaterOrEqual(0x2005, 0x0) && !writer.IsVersionGreaterOrEqual(0x2006, 0x0))
            writer.WriteLineTranslationArray(FifamNames::TransformTrArray(mAbbreviation5Letters, FifamNames::LimitName, 5));
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
        writer.WriteLineTranslationArray(FifamNames::TransformTrArray(mAbbreviation, FifamNames::LimitName, 4));
        writer.WriteLineTranslationArray(mClubNameUsageInPhrase);
        if (writer.IsVersionGreaterOrEqual(0x2006, 0x3))
            writer.WriteLineTranslationArray(mClubNameUsageInPhrase2);
        writer.WriteLineTranslationArray(FifamNames::TransformTrArray(mCityName, FifamNames::LimitName, 29));
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
        writer.WriteLine(FifamUtils::GetWriteableID(mPartnershipClub));
        auto rivalClubs = FifamUtils::MakeWriteableIDsList(mRivalClubs);
        for (UInt i = 0; i < 2; i++)
            writer.WriteLine(rivalClubs[i]);
        if (writer.IsVersionGreaterOrEqual(0x2006, 1))
            writer.WriteLine(rivalClubs[2]);
        if (writer.IsVersionGreaterOrEqual(0x2006, 3))
            writer.WriteLine(rivalClubs[3]);
        writer.WriteLine(mYearOfFoundation);
        writer.WriteLine(mAddress);
        writer.WriteLine(mTelephone);
        writer.WriteLine(mWebsiteAndMail);
        writer.WriteLine(mFansites);
        writer.WriteLine(mNewspaper);
        writer.WriteLine(mMascotName);
        writer.WriteLine(mInitialCapital);
        if (writer.IsVersionGreaterOrEqual(0x2006, 0x01))
            writer.WriteLine(mTransferBudget);
        writer.WriteLine(mJointStockCompany);
        writer.WriteLine(Unknown._2);
        if (writer.IsVersionGreaterOrEqual(0x2007, 0x09))
            writer.WriteLine(mPenaltyType);
        else {
            Char penaltyPoints = mPenaltyPoints;
            if (penaltyPoints != 0) {
                penaltyPoints = -penaltyPoints;
                if (penaltyPoints != 3 && penaltyPoints != 6) {
                    if (penaltyPoints > 6)
                        penaltyPoints = 6;
                    else
                        penaltyPoints = 3;
                }
            }
            writer.WriteLine(penaltyPoints);
        }
        if (writer.IsVersionGreaterOrEqual(0x2011, 0x0B)) {
            writer.WriteLine(mSponsorAmount);
            writer.WriteLine(mSponsorDuration);
            writer.WriteLine(mSpecialSponsor);
        }
        if (writer.IsVersionGreaterOrEqual(0x2007, 0x09))
            writer.WriteLine(mPenaltyPoints);
        if (!writer.IsVersionGreaterOrEqual(0x2005, 0x00)) {
            writer.WriteLine(0);
            writer.WriteLine(0);
        }
        if (writer.GetGameId() >= 9)
            writer.WriteLine(mPotentialFansCount);
        else
            writer.WriteLine(mMaxAttendance);
        writer.WriteLine(mAverageAttendanceLastSeason);
        writer.WriteLine(mCountOfSoldSeasonTickets);
        UInt numLeagues = 1;
        if (writer.IsVersionGreaterOrEqual(0x2009, 0x06))
            numLeagues = 3;
        else if (writer.IsVersionGreaterOrEqual(0x2007, 0x14))
            numLeagues = 2;
        for (UInt i = 0; i < numLeagues; i++)
            writer.WriteLine(mLeagueTotalPoints[i]);
        if (writer.IsVersionGreaterOrEqual(0x2007, 0x06)) {
            for (UInt i = 0; i < numLeagues; i++)
                writer.WriteLine(mLeagueTotalWins[i]);
            for (UInt i = 0; i < numLeagues; i++)
                writer.WriteLine(mLeagueTotalDraws[i]);
            for (UInt i = 0; i < numLeagues; i++)
                writer.WriteLine(mLeagueTotalLoses[i]);
        }
        else
            writer.WriteLine(mLeagueTotalMatches);
        for (UInt i = 0; i < numLeagues; i++)
            writer.WriteLine(mLeagueTotalGoals[i]);
        for (UInt i = 0; i < numLeagues; i++)
            writer.WriteLine(mLeagueTotalGoalsAgainst[i]);
        for (UInt i = 0; i < numLeagues; i++)
            writer.WriteLine(mLeagueTotalLeadershipsInTable[i]);
        writer.WriteLine(mStadiumVenue);
        writer.WriteLine(mStadiumSeatsCapacity);
        writer.WriteLineTranslationArray(FifamNames::TransformTrArray(mStadiumName, FifamNames::LimitName, 29));
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
        if (writer.IsVersionGreaterOrEqual(0x2004, 0x00)) {
            writer.WriteLine(mClubColour.FindIndexInTable(mTeamColorsTable));
            writer.WriteLine(mClubColour2.FindIndexInTable(mTeamColorsTable));
            writer.WriteLine(mMerchandiseColour.FindIndexInTable(mMerchandiseColorsTable));
            if (writer.IsVersionGreaterOrEqual(0x2007, 0x0E)) {
                writer.WriteLine(mHeaderColour.FindIndexInTable(m08InterfaceColorsTable));
                writer.WriteLine(mBackgroundColour.FindIndexInTable(m08InterfaceColorsTable));
            }
            else if (writer.IsVersionGreaterOrEqual(0x2007, 2)) {
                writer.WriteLine(mHeaderColour.FindIndexInTable(m07InterfaceColorsTable));
                writer.WriteLine(mBackgroundColour.FindIndexInTable(m07InterfaceColorsTable));
            }
        }
        mHistory.Write(writer);
        writer.WriteLine(mGeoCoords.mLatitude.ToInt());
        writer.WriteLine(mGeoCoords.mLongitude.ToInt());
        writer.WriteLine(mNationalPrestige);
        if (writer.IsVersionGreaterOrEqual(0x2006, 2))
            writer.WriteLine(mInternationalPrestige);
        writer.WriteLine(Unknown._4);
        writer.WriteLine(mTransfersCountry[0]);
        writer.WriteLine(mTransfersCountry[1]);
        writer.WriteLine(mYouthPlayersCountry);
        writer.WriteLine(mFifaID);
        if (writer.IsVersionGreaterOrEqual(0x2004, 0x04) && !writer.IsVersionGreaterOrEqual(0x2005, 0x03))
            writer.WriteLine(Unknown._5);
        WriteClubMembers(writer);
        mKit.Write(writer);
        if (writer.IsVersionGreaterOrEqual(0x2007, 1)) {
            FifamDbWriteableIDsList lowestLeaguesIDs;
            // TODO: check this one
            for (UInt i = 0; i < mLowestLeagues.size(); i++)
                lowestLeaguesIDs.push_back_unique(FifamUtils::GetWriteableID(mLowestLeagues[i], writer.GetGameId()));
            if (writer.IsVersionGreaterOrEqual(0x2007, 2)) {
                for (UInt i = 0; i < 4; i++)
                    writer.WriteLine(lowestLeaguesIDs[i]);
            }
            else
                writer.WriteLine(lowestLeaguesIDs[0]);
        }
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
