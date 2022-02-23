#pragma once
#include "FifamDbEnvironment.h"
#include "FifamPlayerLevel.h"
#include "FifamStaff.h"
#include "FifamNames.h"
#include "FifamCompLeague.h"

String FixPersonName(String const &name, UInt gameId) {
    String result;
    static Set<WideChar> restrictedChars = { L',', L'|', L':', L'<', L'>', L'/', L'\\', L'?', L'*', 0x327, 0x301, 0x308, 0x30C, 0x200E };
    for (auto c : name) {
        if (!restrictedChars.contains(c)) {
            if (c == L'Ị')
                result += L'I';
            else if (c == L'ị')
                result += L'i';
            else if (c == L'Ọ')
                result += L'O';
            else if (c == L'ọ')
                result += L'o';
            else if (c == L'ọ')
                result += L'o';
            else if (c == L'þ')
                result += L"th";
            else if (c == L'Ə')
                result += L'A';
            else if (c == L'ə')
                result += L'a';
            else if (c == L'ü')
                result += L'u';
            else if (c == L'ứ')
                result += L'u';
            else if (c == L'Ș')
                result += L'S';
            else if (c == L'ș')
                result += L's';
            else if (c == L'ë')
                result += L'e';
            else if (c == L'с')
                result += L'c';
            else if (c == L'´')
                result += L'\'';
            else if (gameId >= 13)
                result += c;
            else {
                if (c == L'Ț')
                    result += L"T";
                else if (c == L'ț')
                    result += L"t";
                else if (c == L'ð')
                    result += L'd';
                else
                    result += c;
            }
        }
    }
    return result;
}

class CompareDB {
public:
    static String NationName(FifamDatabase *db, FifamNation const &id) {
        if (id.ToInt() >= 1 && id.ToInt() <= FifamDatabase::NUM_COUNTRIES && db->mCountries[id.ToInt() - 1])
            return FifamTr(db->mCountries[id.ToInt() - 1]->mName);
        return String();
    }

    static String ClubName(FifamClubLink const &c) {
        if (c.IsValid())
            return L"\"" + (c.mPtr->mName[FifamTranslation::English]) + L"\"";
        return L"\"(NONE)\"";
    }

    static UInt ClubID(FifamClubLink const &c) {
        if (c.IsValid())
            return c.mPtr->mUniqueID;
        return 0;
    }

    CompareDB() {
        UInt const gameVersion = 13;
        Path dbPath1 = L"db1";
        Path dbPath2 = L"db2";

        FifamDatabase::mReadingOptions.mUseCustomFormations = true;

        //FifamDatabase::mReadingOptions.mReadClubs = false;
        //FifamDatabase::mReadingOptions.mReadPersons = false;
        //FifamDatabase *db1 = nullptr;
        //FifamDatabase *db2 = new FifamDatabase(gameVersion, dbPath2);
        //FifamWriter wr("relegation_ids.txt");
        //FifamWriter ws("split_ids.txt");
        //for (auto const &[id, comp] : db2->mCompMap) {
        //    if (id.mType == FifamCompType::Relegation) {
        //        Set<Int> leagueSplitCountries = { 131,158,171,167,192,72,76,82,89,95,2,3,4,9,11,12,13,17,22,26,32,35,39,42,43,50,200,75,205 };
        //        if (id.mRegion == FifamCompRegion::United_States || (leagueSplitCountries.contains(id.mRegion.ToInt()) && (id.mIndex == 0 || id.mIndex == 1)))
        //            ws.WriteLine(id.ToHexStr());
        //        else
        //            wr.WriteLine(id.ToHexStr());
        //    }
        //}
        //return;

        FifamDatabase *db1 = new FifamDatabase(gameVersion, dbPath1);
        FifamDatabase *db2 = new FifamDatabase(gameVersion, dbPath2);
        //db1->SetupWriteableStatus(gameVersion);
        //db2->SetupWriteableStatus(gameVersion);

        //Map<UInt, FifamPlayer *> oldPlayers;
        //for (auto const &p : db1->mPlayers) {
        //    if (p->mEmpicsId > 0)
        //        oldPlayers[p->mEmpicsId] = p;
        //}
        //auto PortraitPath = [](Vector<Path> const &dirs, String const &id, UInt empicsId) {
        //    String id1 = id;
        //    if (empicsId > 0) {
        //        for (auto const &dir : dirs) {
        //            Path p = dir / (id1 + L".png");
        //            if (exists(p))
        //                return p;
        //        }
        //        String empicsStr = L"-" + to_wstring(empicsId);
        //        if (id1.ends_with(empicsStr))
        //            id1 = id1.substr(0, id1.size() - empicsStr.size());
        //    }
        //    for (auto const &dir : dirs) {
        //        Path p = dir / (id1 + L".png");
        //        if (exists(p))
        //            return p;
        //    }
        //    return Path();
        //};
        //Vector<Path> pp = {
        //    R"(E:\Projects\fifam\content\fm13\art_02_fifam_style\portraits\club\160x160)",
        //    R"(E:\Projects\fifam\content\fm13\art_03_fifam_style\portraits\club\160x160)",
        //};
        //Vector<Path> ppxxl = {
        //    R"(E:\Projects\fifam\content\fm13\art_11\portraits\club\512x512)",
        //    R"(E:\Projects\fifam\content\fm13\art_12\portraits\club\512x512)",
        //};
        //for (auto const &p : db2->mPlayers) {
        //    if (p->mFootballManagerID > 0 && oldPlayers.contains(p->mFootballManagerID)) {
        //        if (PortraitPath(pp, p->mWriteableStringID, p->mEmpicsId).empty()) {
        //            FifamPlayer *oldp = oldPlayers[p->mFootballManagerID];
        //            Path oldpath = PortraitPath(pp, oldp->mWriteableStringID, oldp->mEmpicsId);
        //            if (!oldpath.empty())
        //                copy(oldpath, L"fix\\" + p->mWriteableStringID + L".png", copy_options::overwrite_existing);
        //        }
        //        if (PortraitPath(ppxxl, p->mWriteableStringID, p->mEmpicsId).empty()) {
        //            FifamPlayer *oldp = oldPlayers[p->mFootballManagerID];
        //            Path oldpath = PortraitPath(ppxxl, oldp->mWriteableStringID, oldp->mEmpicsId);
        //            if (!oldpath.empty())
        //                copy(oldpath, L"fix_xxl\\" + p->mWriteableStringID + L".png", copy_options::overwrite_existing);
        //        }
        //    }
        //}
        //
        //return;
        
        //Set<UInt> usedFoomIDs;
        //for (auto const &p : db2->mPlayers) {
        //    if (p->mFootballManagerID > 0)
        //        usedFoomIDs.insert(p->mFootballManagerID);
        //}
        //for (auto const &s : db2->mStaffs) {
        //    if (s->mFootballManagerID > 0)
        //        usedFoomIDs.insert(s->mFootballManagerID);
        //}
        //for (auto const &r : db2->mReferees) {
        //    if (r->mFootballManagerID > 0)
        //        usedFoomIDs.insert(r->mFootballManagerID);
        //}
        //for (auto const &p : db2->mPlayers) {
        //    if (p->mFootballManagerID <= 0 && p->mEmpicsId != 0 && p->mClub && p->mClub->mCountry->mId == FifamNation::Israel && !usedFoomIDs.contains(p->mEmpicsId)) {
        //        p->mFootballManagerID = p->mEmpicsId;
        //        usedFoomIDs.insert(p->mFootballManagerID);
        //    }
        //}
        //Map<String, Int> staffIDs;
        //for (auto const &s : db1->mStaffs) {
        //    if (s->mFootballManagerID > 0 && s->mClub && s->mClub->mCountry->mId == FifamNation::Israel && !usedFoomIDs.contains(s->mFootballManagerID))
        //        staffIDs[s->mWriteableStringID] = s->mFootballManagerID;
        //}
        //for (auto const &s : db2->mStaffs) {
        //    if (s->mFootballManagerID <= 0 && s->mClub && s->mClub->mCountry->mId == FifamNation::Israel && staffIDs.contains(s->mWriteableStringID) && !usedFoomIDs.contains(staffIDs[s->mWriteableStringID])) {
        //        s->mFootballManagerID = staffIDs[s->mWriteableStringID];
        //        usedFoomIDs.insert(s->mFootballManagerID);
        //    }
        //}
        
        //for (auto const &p : db2->mPlayers) {
        //    if (p->mFootballManagerID <= 0 && p->mEmpicsId != 0 && !p->mClub && !usedFoomIDs.contains(p->mEmpicsId)) {
        //        p->mFootballManagerID = p->mEmpicsId;
        //        usedFoomIDs.insert(p->mFootballManagerID);
        //    }
        //}
        
        //db2->Write(13, FifamDatabase::GetGameDbVersion(13), L"db_new");
        //return;

        FifamWriter fw("loans_fix_info.txt");
        FifamWriter fws("loans_fix_same_club.txt");
        FifamWriter fwh("loans_fix_history.txt");
        fw.WriteLine(L"Type", L"Player", L"FoomID", L"ClubID", L"OldClub", L"NewClub", L"Removed", L"HistoryFix");
        Map<UInt, FifamPlayer *> oldPlayers;
        //Map<UInt, FifamStaff *> oldStaff;
        for (auto const &p : db1->mPlayers) {
            if (p->mFootballManagerID > 0)
                oldPlayers[p->mFootballManagerID] = p;
        }
        //for (auto const &s : db1->mStaffs) {
        //    if (s->mFootballManagerID > 0)
        //        oldStaff[s->mFootballManagerID] = s;
        //}
        auto CopyClub = [&](FifamClubLink &dst, FifamClubLink const &src) {
            if (src.IsValid()) {
                dst.mPtr = db2->GetClubFromUID(src.mPtr->mUniqueID);
                if (!dst.mPtr)
                    dst.mTeamType = FifamClubTeamType::First;
            }
        };
        //for (auto &c : db2->mClubs) {
        //    Set<UInt> clubsToProcess = {
        //        0x401003,0x411007,0x43100B,0x431006,0x431007,0x6A0006,0x701003,0xA10019,0x1C303A,0x7E0002,0x3E0040,0x580006,0x580005,0x283508,0x8B100A,0x8D1103
        //    };
        //    if (clubsToProcess.contains(c->mUniqueID)) {
        //        FifamClub *oldc = db1->GetClubFromUID(c->mUniqueID);
        //        if (oldc) {
        //            c->mName = oldc->mName;
        //            c->mName2 = oldc->mName2;
        //            c->mShortName = oldc->mShortName;
        //            c->mShortName2 = oldc->mShortName2;
        //            c->mAbbreviation = oldc->mAbbreviation;
        //            c->mAbbreviation5Letters = oldc->mAbbreviation5Letters;
        //            c->mCityName = oldc->mCityName;
        //            c->mPlayerInText = oldc->mPlayerInText;
        //            c->mFanName1 = oldc->mFanName1;
        //            c->mAddress = oldc->mAddress;
        //            c->mTelephone = oldc->mTelephone;
        //            c->mWebsiteAndMail = oldc->mWebsiteAndMail;
        //            c->mFansites = oldc->mFansites;
        //            c->mNewspaper = oldc->mNewspaper;
        //            c->mMascotName = oldc->mMascotName;
        //            c->mClubNameUsageInPhrase = oldc->mClubNameUsageInPhrase;
        //            c->mClubNameUsageInPhrase2 = oldc->mClubNameUsageInPhrase2;
        //            c->mYearOfFoundation = oldc->mYearOfFoundation;
        //            c->mInitialCapital = oldc->mInitialCapital;
        //            c->mTransferBudget = oldc->mTransferBudget;
        //            c->mJointStockCompany = oldc->mJointStockCompany;
        //            c->mPenaltyType = oldc->mPenaltyType;
        //            c->mPenaltyPoints = oldc->mPenaltyPoints;
        //            c->mMaxAttendance = oldc->mMaxAttendance;
        //            c->mPotentialFansCount = oldc->mPotentialFansCount;
        //            c->mAverageAttendanceLastSeason = oldc->mAverageAttendanceLastSeason;
        //            c->mCountOfSoldSeasonTickets = oldc->mCountOfSoldSeasonTickets;
        //            c->mLeagueTotalPoints = oldc->mLeagueTotalPoints;
        //            c->mLeagueTotalWins = oldc->mLeagueTotalWins;
        //            c->mLeagueTotalDraws = oldc->mLeagueTotalDraws;
        //            c->mLeagueTotalLoses = oldc->mLeagueTotalLoses;
        //            c->mLeagueTotalGoals = oldc->mLeagueTotalGoals;
        //            c->mLeagueTotalGoalsAgainst = oldc->mLeagueTotalGoalsAgainst;
        //            c->mLeagueTotalLeadershipsInTable = oldc->mLeagueTotalLeadershipsInTable;
        //            c->mLeagueTotalMatches = oldc->mLeagueTotalMatches;
        //            c->mStadiumVenue = oldc->mStadiumVenue;
        //            c->mStadiumStandingsCapacity = oldc->mStadiumStandingsCapacity;
        //            c->mStadiumSeatsCapacity = oldc->mStadiumSeatsCapacity;
        //            c->mStadiumVipCapacity = oldc->mStadiumVipCapacity;
        //            c->mStadiumName = oldc->mStadiumName;
        //            c->mStadiumType = oldc->mStadiumType;
        //            c->mStadiumBigMainStand = oldc->mStadiumBigMainStand;
        //            c->mClubFacilities = oldc->mClubFacilities;
        //            c->mYouthCentre = oldc->mYouthCentre;
        //            c->mYouthBoardingSchool = oldc->mYouthBoardingSchool;
        //            c->mMedicalDepartment = oldc->mMedicalDepartment;
        //            c->mMerchandising = oldc->mMerchandising;
        //            c->mTrainingGrounds = oldc->mTrainingGrounds;
        //            c->mMedia = oldc->mMedia;
        //            c->mResearch = oldc->mResearch;
        //            c->mAiStrategy = oldc->mAiStrategy;
        //            c->mLandscape = oldc->mLandscape;
        //            c->mSettlement = oldc->mSettlement;
        //            c->mBadge = oldc->mBadge;
        //            c->mClubColour = oldc->mClubColour;
        //            c->mClubColour2 = oldc->mClubColour2;
        //            c->mMerchandiseColour = oldc->mMerchandiseColour;
        //            c->mHeaderColour = oldc->mHeaderColour;
        //            c->mBackgroundColour = oldc->mBackgroundColour;
        //            c->mBadgeColour1 = oldc->mBadgeColour1;
        //            c->mBadgeColour2 = oldc->mBadgeColour2;
        //            c->mHistory = oldc->mHistory;
        //            c->mGeoCoords = oldc->mGeoCoords;
        //            c->mNationalPrestige = oldc->mNationalPrestige;
        //            c->mInternationalPrestige = oldc->mInternationalPrestige;
        //            c->mTransfersCountry = oldc->mTransfersCountry;
        //            c->mYouthPlayersCountry = oldc->mYouthPlayersCountry;
        //            c->mKit = oldc->mKit;
        //            c->mLowestLeagues = oldc->mLowestLeagues;
        //            c->mMediaPressure = oldc->mMediaPressure;
        //            c->mTraditionalClub = oldc->mTraditionalClub;
        //            c->mRichGuyControlled = oldc->mRichGuyControlled;
        //            c->mYouthPlayersAreBasques = oldc->mYouthPlayersAreBasques;
        //            c->mFanName2 = oldc->mFanName2;
        //            c->mTermForTeam1 = oldc->mTermForTeam1;
        //            c->mTermForTeam2 = oldc->mTermForTeam2;
        //            c->mSponsorAmount = oldc->mSponsorAmount;
        //            c->mSponsorDuration = oldc->mSponsorDuration;
        //            c->mSpecialSponsor = oldc->mSpecialSponsor;
        //            c->mIndividualTvMoney = oldc->mIndividualTvMoney;
        //            c->mFanMembers = oldc->mFanMembers;
        //        }
        //        else {
        //            ::Error("Club %08X not found in db1", c->mUniqueID);
        //        }
        //    }
        //    if (c->mCountry->mId == FifamNation::Israel ||
        //        c->mCountry->mId == FifamNation::Anguilla ||
        //        c->mCountry->mId == FifamNation::Papua_New_Guinea ||
        //        c->mCountry->mId == FifamNation::Montserrat ||
        //        c->mCountry->mId == FifamNation::Madagascar ||
        //        c->mCountry->mId == FifamNation::Antigua_and_Barbuda)
        //    {
        //        FifamClub *oldc = db1->GetClubFromUID(c->mUniqueID);
        //        if (oldc) {
        //            CopyClub(c->mPartnershipClub, oldc->mPartnershipClub);
        //            c->mRivalClubs.clear();
        //            for (auto r : oldc->mRivalClubs) {
        //                CopyClub(r, r);
        //                if (r.IsValid())
        //                    c->mRivalClubs.push_back(r);
        //            }
        //        }
        //    }
        //}
        for (auto &p : db2->mPlayers) {
            if (p->mFootballManagerID > 0 && oldPlayers.contains(p->mFootballManagerID)) {
                FifamPlayer *oldp = oldPlayers[p->mFootballManagerID];
                if (oldp) {
                    if (!oldp->mNickname.empty() && p->mNickname.empty())
                        p->mNickname = oldp->mNickname;
                    //p->mTalent = oldp->mTalent;
                    //p->mGeneralExperience = oldp->mGeneralExperience;
                    //p->mInternationalExperience = oldp->mInternationalExperience;
                    //p->mNationalExperience = oldp->mNationalExperience;
                    //p->mMainPosition = oldp->mMainPosition;
                    //p->mPlayingStyle = oldp->mPlayingStyle;
                    //p->mPositionBias = oldp->mPositionBias;
                    //auto oldLeadership = p->mAttributes.Leadership;
                    //p->mAttributes = oldp->mAttributes;
                    //p->mAttributes.Leadership = oldLeadership;
                    p->mHistory.mEntries.clear();
                    for (auto h : oldp->mHistory.mEntries) {
                        CopyClub(h.mClub, h.mClub);
                        p->mHistory.mEntries.push_back(h);
                    }
                    p->mStartingConditions.mLoan = oldp->mStartingConditions.mLoan;
                    p->mStartingConditions.mFutureTransfer = oldp->mStartingConditions.mFutureTransfer;
                    p->mStartingConditions.mFutureLoan = oldp->mStartingConditions.mFutureLoan;
                    p->mStartingConditions.mFutureReLoan = oldp->mStartingConditions.mFutureReLoan;
                    if (oldp->mStartingConditions.mLoan.Enabled() && oldp->mStartingConditions.mLoan.mLoanedClub.IsValid()) {
                        CopyClub(p->mStartingConditions.mLoan.mLoanedClub, oldp->mStartingConditions.mLoan.mLoanedClub);
                        Bool removed = false; Bool historyFix = false;
                        if (!p->mStartingConditions.mLoan.mLoanedClub.IsValid()) {
                            p->mStartingConditions.mLoan.Disable();
                            removed = true;
                            if (!p->mHistory.mEntries.empty() && p->mHistory.mEntries.back().mStillInThisClub && p->mHistory.mEntries.back().mLoan) {
                                p->mHistory.mEntries.back().mLoan = false;
                                historyFix = true;
                            }
                        }
                        fw.WriteLine(L"LOAN", p->GetName(), p->mFootballManagerID, ClubID(oldp->mStartingConditions.mLoan.mLoanedClub), ClubName(oldp->mStartingConditions.mLoan.mLoanedClub), ClubName(p->mStartingConditions.mLoan.mLoanedClub), removed, historyFix);
                    }
                    if (oldp->mStartingConditions.mFutureTransfer.Enabled() && oldp->mStartingConditions.mFutureTransfer.mNewClub.IsValid()) {
                        CopyClub(p->mStartingConditions.mFutureTransfer.mNewClub, oldp->mStartingConditions.mFutureTransfer.mNewClub);
                        Bool removed = false; Bool historyFix = false;
                        if (!p->mStartingConditions.mFutureTransfer.mNewClub.IsValid()) {
                            p->mStartingConditions.mFutureTransfer.Disable();
                            removed = true;
                        }
                        fw.WriteLine(L"FUTURETRANSFER", p->GetName(), p->mFootballManagerID, ClubID(oldp->mStartingConditions.mFutureTransfer.mNewClub), ClubName(oldp->mStartingConditions.mFutureTransfer.mNewClub), ClubName(p->mStartingConditions.mFutureTransfer.mNewClub), removed, historyFix);
                    }
                    if (oldp->mStartingConditions.mFutureLoan.Enabled() && oldp->mStartingConditions.mFutureLoan.mLoanedClub.IsValid()) {
                        CopyClub(p->mStartingConditions.mFutureLoan.mLoanedClub, oldp->mStartingConditions.mFutureLoan.mLoanedClub);
                        Bool removed = false; Bool historyFix = false;
                        if (!p->mStartingConditions.mFutureLoan.mLoanedClub.IsValid()) {
                            p->mStartingConditions.mFutureLoan.Disable();
                            removed = true;
                        }
                        fw.WriteLine(L"FUTURELOAN", p->GetName(), p->mFootballManagerID, ClubID(oldp->mStartingConditions.mFutureLoan.mLoanedClub), ClubName(oldp->mStartingConditions.mFutureLoan.mLoanedClub), ClubName(p->mStartingConditions.mFutureLoan.mLoanedClub), removed, historyFix);
                    }
                    if (oldp->mStartingConditions.mFutureReLoan.Enabled() && oldp->mStartingConditions.mFutureReLoan.mLoanedClub.IsValid()) {
                        CopyClub(p->mStartingConditions.mFutureReLoan.mLoanedClub, oldp->mStartingConditions.mFutureReLoan.mLoanedClub);
                        Bool removed = false; Bool historyFix = false;
                        if (!p->mStartingConditions.mFutureReLoan.mLoanedClub.IsValid()) {
                            p->mStartingConditions.mFutureReLoan.Disable();
                            removed = true;
                        }
                        fw.WriteLine(L"FUTURERELOAN", p->GetName(), p->mFootballManagerID, ClubID(oldp->mStartingConditions.mFutureReLoan.mLoanedClub), ClubName(oldp->mStartingConditions.mFutureReLoan.mLoanedClub), ClubName(p->mStartingConditions.mFutureReLoan.mLoanedClub), removed, historyFix);
                    }
                    //FifamClub *c = p->mClub;
                    //if (c) {
                    //    if (c->mCountry->mId == FifamNation::Israel ||
                    //        c->mCountry->mId == FifamNation::Anguilla ||
                    //        c->mCountry->mId == FifamNation::Papua_New_Guinea ||
                    //        c->mCountry->mId == FifamNation::Montserrat ||
                    //        c->mCountry->mId == FifamNation::Madagascar ||
                    //        c->mCountry->mId == FifamNation::Antigua_and_Barbuda)
                    //    {
                    //        if (p->mContract.mBuyBackClauseClub.IsValid() && oldp->mContract.mBuyBackClauseClub.IsValid()) {
                    //            CopyClub(p->mContract.mBuyBackClauseClub, oldp->mContract.mBuyBackClauseClub);
                    //            if (!p->mContract.mBuyBackClauseClub.IsValid()) {
                    //                p->mContract.mBuyBackClauseEnabled = false;
                    //                p->mContract.mBuyBackClauseCost = 0;
                    //                p->mContract.mBuyBackClauseValidUntil.MakeEmpty();
                    //            }
                    //        }
                    //        if (p->mStartingConditions.mLoan.mLoanedClub.IsValid() && oldp->mStartingConditions.mLoan.mLoanedClub.IsValid()) {
                    //            CopyClub(p->mStartingConditions.mLoan.mLoanedClub, oldp->mStartingConditions.mLoan.mLoanedClub);
                    //            if (!p->mStartingConditions.mLoan.mLoanedClub.IsValid())
                    //                p->mStartingConditions.mLoan.Disable();
                    //        }
                    //        if (p->mStartingConditions.mFutureTransfer.mNewClub.IsValid() && oldp->mStartingConditions.mFutureTransfer.mNewClub.IsValid()) {
                    //            CopyClub(p->mStartingConditions.mFutureTransfer.mNewClub, oldp->mStartingConditions.mFutureTransfer.mNewClub);
                    //            if (!p->mStartingConditions.mFutureTransfer.mNewClub.IsValid())
                    //                p->mStartingConditions.mFutureTransfer.Disable();
                    //        }
                    //        if (p->mStartingConditions.mFutureLoan.mLoanedClub.IsValid() && oldp->mStartingConditions.mFutureLoan.mLoanedClub.IsValid()) {
                    //            CopyClub(p->mStartingConditions.mFutureLoan.mLoanedClub, oldp->mStartingConditions.mFutureLoan.mLoanedClub);
                    //            if (!p->mStartingConditions.mFutureLoan.mLoanedClub.IsValid())
                    //                p->mStartingConditions.mFutureLoan.Disable();
                    //        }
                    //        if (p->mStartingConditions.mFutureReLoan.mLoanedClub.IsValid() && oldp->mStartingConditions.mFutureReLoan.mLoanedClub.IsValid()) {
                    //            CopyClub(p->mStartingConditions.mFutureReLoan.mLoanedClub, oldp->mStartingConditions.mFutureReLoan.mLoanedClub);
                    //            if (!p->mStartingConditions.mFutureReLoan.mLoanedClub.IsValid())
                    //                p->mStartingConditions.mFutureReLoan.Disable();
                    //        }
                    //    }
                    //}
                    if (p->mClub && (p->mClub->mCountry->mId == FifamNation::Ukraine || p->mClub->mCountry->mId == FifamNation::Russia)) {
                        p->mAppearance = oldp->mAppearance;
                    }
                }
            }
            if (!p->mClub) {
                if (p->mStartingConditions.mLoan.Enabled()) {
                    fws.WriteLine(L"FREEAGENTLOAN", p->GetName(), p->mFootballManagerID, L"", L"");
                    p->mStartingConditions.mLoan.Disable();
                }
                if (p->mStartingConditions.mFutureLoan.Enabled()) {
                    fws.WriteLine(L"FREEAGENTFUTURELOAN", p->GetName(), p->mFootballManagerID, L"", L"");
                    p->mStartingConditions.mFutureLoan.Disable();
                }
                if (p->mStartingConditions.mFutureReLoan.Enabled()) {
                    fws.WriteLine(L"FREEAGENTFUTURERELOAN", p->GetName(), p->mFootballManagerID, L"", L"");
                    p->mStartingConditions.mFutureReLoan.Disable();
                }
                if (p->mStartingConditions.mFutureTransfer.Enabled()) {
                    fws.WriteLine(L"FREEAGENTFUTURETRANSFER", p->GetName(), p->mFootballManagerID, L"", L"");
                    p->mStartingConditions.mFutureTransfer.Disable();
                }
            }
            else {
                if (p->mStartingConditions.mLoan.Enabled() && p->mClub == p->mStartingConditions.mLoan.mLoanedClub.mPtr) {
                    fws.WriteLine(L"SAMECLUBLOAN", p->GetName(), p->mFootballManagerID, ClubID(p->mClub), ClubName(p->mClub));
                    p->mStartingConditions.mLoan.Disable();
                }
                if (p->mStartingConditions.mFutureLoan.Enabled() && p->mClub == p->mStartingConditions.mFutureLoan.mLoanedClub.mPtr) {
                    fws.WriteLine(L"SAMECLUBFUTURELOAN", p->GetName(), p->mFootballManagerID, ClubID(p->mClub), ClubName(p->mClub));
                    //p->mStartingConditions.mFutureLoan.Disable();
                }
                if (p->mStartingConditions.mFutureReLoan.Enabled() && p->mClub == p->mStartingConditions.mFutureReLoan.mLoanedClub.mPtr) {
                    fws.WriteLine(L"SAMECLUBFUTURERELOAN", p->GetName(), p->mFootballManagerID, ClubID(p->mClub), ClubName(p->mClub));
                    //p->mStartingConditions.mFutureReLoan.Disable();
                }
                if (p->mStartingConditions.mFutureTransfer.Enabled() && p->mClub == p->mStartingConditions.mFutureTransfer.mNewClub.mPtr) {
                    if (!p->mStartingConditions.mLoan.Enabled()) {
                        p->mStartingConditions.mFutureTransfer.Disable();
                        fws.WriteLine(L"SAMECLUBFUTURETRANSFER_FIX", p->GetName(), p->mFootballManagerID, ClubID(p->mClub), ClubName(p->mClub));
                    }
                    else
                        fws.WriteLine(L"SAMECLUBFUTURETRANSFER", p->GetName(), p->mFootballManagerID, ClubID(p->mClub), ClubName(p->mClub));
                }
                if (!p->mHistory.mEntries.empty() && p->mHistory.mEntries.back().mStillInThisClub && p->mHistory.mEntries.back().mClub.mPtr == p->mClub) {
                    if (p->mHistory.mEntries.back().mLoan) {
                        if (!p->mStartingConditions.mLoan.Enabled()) {
                            p->mHistory.mEntries.back().mLoan = false;
                            Bool merged = false;
                            if (p->mHistory.mEntries.size() > 1) {
                                auto &a = p->mHistory.mEntries[p->mHistory.mEntries.size() - 2];
                                auto &b = p->mHistory.mEntries[p->mHistory.mEntries.size() - 1];
                                if (a.mClub.mPtr == b.mClub.mPtr && a.mLoan == b.mLoan) {
                                    a.mEndDate = b.mEndDate;
                                    a.mStillInThisClub = b.mStillInThisClub;
                                    a.mMatches += b.mMatches;
                                    a.mGoals += b.mGoals;
                                    a.mReserveMatches += b.mReserveMatches;
                                    a.mReserveGoals += b.mReserveGoals;
                                    a.mTransferFee += b.mTransferFee;
                                    p->mHistory.mEntries.pop_back();
                                    merged = true;
                                }
                            }
                            fwh.WriteLine(p->GetName(), p->mFootballManagerID, ClubID(p->mClub), ClubName(p->mClub), 0, merged);
                        }
                    }
                    else {
                        if (p->mStartingConditions.mLoan.Enabled()) {
                            p->mHistory.mEntries.back().mLoan = true;
                            Bool merged = false;
                            if (p->mHistory.mEntries.size() > 1) {
                                auto &a = p->mHistory.mEntries[p->mHistory.mEntries.size() - 2];
                                auto &b = p->mHistory.mEntries[p->mHistory.mEntries.size() - 1];
                                if (a.mClub.mPtr == b.mClub.mPtr && a.mLoan == b.mLoan) {
                                    a.mEndDate = b.mEndDate;
                                    a.mStillInThisClub = b.mStillInThisClub;
                                    a.mMatches += b.mMatches;
                                    a.mGoals += b.mGoals;
                                    a.mReserveMatches += b.mReserveMatches;
                                    a.mReserveGoals += b.mReserveGoals;
                                    a.mTransferFee += b.mTransferFee;
                                    p->mHistory.mEntries.pop_back();
                                    merged = true;
                                }
                            }
                            fwh.WriteLine(p->GetName(), p->mFootballManagerID, ClubID(p->mClub), ClubName(p->mClub), 1, merged);
                        }
                    }
                }
            }
        }
        //for (auto &s : db2->mStaffs) {
        //    if (s->mFootballManagerID > 0 && oldStaff.contains(s->mFootballManagerID)) {
        //        FifamStaff *olds = oldStaff[s->mFootballManagerID];
        //        if (olds) {
        //            CopyClub(s->mFavouriteClub, olds->mFavouriteClub);
        //            CopyClub(s->mWouldnSignFor, olds->mWouldnSignFor);
        //        }
        //    }
        //}
        //
        //
        //Map<Int, String> jerseyNames;
        //FifamReader r("jersey_names.csv");
        //if (r.Available()) {
        //    while (!r.IsEof()) {
        //        if (!r.EmptyLine()) {
        //            Int id = -1;
        //            String name;
        //            r.ReadLine(id, name);
        //            if (id > 0 && !name.empty())
        //                jerseyNames[id] = name;
        //        }
        //        else
        //            r.SkipLine();
        //    }
        //}
        //for (auto &p : db2->mPlayers) {
        //    if (p->mFootballManagerID > 0 && jerseyNames.contains(p->mFootballManagerID)) {
        //        String jerseyName = jerseyNames[p->mFootballManagerID];
        //        if (!jerseyName.empty()) {
        //            jerseyName = FixPersonName(jerseyName, 13);
        //            if (!jerseyName.empty() && jerseyName.size() <= 19) {
        //                p->mNickname = jerseyName;
        //                std::wcout << p->GetName() << L" >> " << jerseyName << std::endl;
        //            }
        //        }
        //    }
        //}
        
        db2->Write(13, FifamDatabase::GetGameDbVersion(13), L"db_new");
        return;

        Map<FifamClub *, FifamCompLeague *> clubLeague1;
        Map<FifamClub *, FifamCompLeague *> clubLeague2;

        for (auto const &[id, c] : db1->mCompMap) {
            FifamCompLeague *l = c->AsLeague();
            if (l && l->mID.mType == FifamCompType::League) {
                for (auto const &t : l->mTeams) {
                    if (t.IsValid())
                        clubLeague1[t.mPtr] = l;
                }
            }
        }
        for (auto const &[id, c] : db2->mCompMap) {
            FifamCompLeague *l = c->AsLeague();
            if (l && l->mID.mType == FifamCompType::League) {
                for (auto const &t : l->mTeams) {
                    if (t.IsValid())
                        clubLeague2[t.mPtr] = l;
                }
            }
        }
        Vector<Pair<String, String>> renPlayersAndStaff;
        Vector<Pair<String, String>> renXXL;
        Vector<Pair<String, String>> renReferees;
        // clubs
        {
            FifamWriter w(L"clubs_removed_ids.txt");
            w.WriteLineWithSeparator(L'\t', L"id", L"name", L"prestige");
            for (auto const &[id, c] : db1->mClubsMap) {
                if (!db2->mClubsMap.contains(id))
                    w.WriteLineWithSeparator(L'\t', Utils::Format(L"0x%08X", id), FifamTr(c->mName), (c->mInternationalPrestige * 20 + c->mNationalPrestige) * 99 / 420);
            }
        }
        {
            FifamWriter w(L"clubs_added_ids.txt");
            w.WriteLineWithSeparator(L'\t', L"id", L"name", L"prestige");
            for (auto const &[id, c] : db2->mClubsMap) {
                if (!db1->mClubsMap.contains(id))
                    w.WriteLineWithSeparator(L'\t', Utils::Format(L"0x%08X", id), FifamTr(c->mName), (c->mInternationalPrestige * 20 + c->mNationalPrestige) * 99 / 420);
            }
        }
        // referees
        {
            Map<String, FifamReferee *> referees;
            for (auto const &r : db2->mReferees)
                referees[FifamNames::GetPersonStringId(gameVersion, r->mFirstName, r->mLastName, String(), Date(), 0)] = r;
            FifamWriter w(L"referee_removed_ids.txt");
            w.WriteLineWithSeparator(L'\t', L"id", L"foomid", L"name", L"firstname", L"lastname", L"country");
            for (auto const &r : db1->mReferees) {
                String id = FifamNames::GetPersonStringId(gameVersion, r->mFirstName, r->mLastName, String(), Date(), 0);
                if (!referees.contains(id))
                    w.WriteLineWithSeparator(L'\t', id, r->mFootballManagerID, r->mFirstName + L" " + r->mLastName, r->mFirstName, r->mLastName, FifamTr(r->mCountry->mName));
            }
        }
        {
            Map<String, FifamReferee *> referees;
            for (auto const &r : db1->mReferees)
                referees[FifamNames::GetPersonStringId(gameVersion, r->mFirstName, r->mLastName, String(), Date(), 0)] = r;
            FifamWriter w(L"referee_added_ids.txt");
            w.WriteLineWithSeparator(L'\t', L"id", L"foomid", L"name", L"firstname", L"lastname", L"country");
            for (auto const &r : db2->mReferees) {
                String id = FifamNames::GetPersonStringId(gameVersion, r->mFirstName, r->mLastName, String(), Date(), 0);
                if (!referees.contains(id))
                    w.WriteLineWithSeparator(L'\t', id, r->mFootballManagerID, r->mFirstName + L" " + r->mLastName, r->mFirstName, r->mLastName, FifamTr(r->mCountry->mName));
            }
        }
        {
            Map<UInt, FifamReferee *> referees;
            for (auto const &r : db2->mReferees) {
                if (r->mFootballManagerID > 0)
                    referees[r->mFootballManagerID] = r;
            }
            FifamWriter w(L"foom_referee_removed.txt");
            w.WriteLineWithSeparator(L'\t', L"id", L"foomid", L"name", L"firstname", L"lastname", L"country");
            for (auto const &r : db1->mReferees) {
                if (r->mFootballManagerID > 0 && !referees.contains(r->mFootballManagerID)) {
                    String id = FifamNames::GetPersonStringId(gameVersion, r->mFirstName, r->mLastName, String(), Date(), 0);
                    w.WriteLineWithSeparator(L'\t', id, r->mFootballManagerID, r->mFirstName + L" " + r->mLastName, r->mFirstName, r->mLastName, FifamTr(r->mCountry->mName));
                }
            }
        }
        {
            Map<UInt, FifamReferee *> referees;
            for (auto const &r : db1->mReferees) {
                if (r->mFootballManagerID > 0)
                    referees[r->mFootballManagerID] = r;
            }
            FifamWriter w(L"foom_referee_changed_id.txt");
            w.WriteLineWithSeparator(L'\t', L"oldid", L"newid", L"foomid", L"name", L"firstname", L"lastname", L"country");
            for (auto const &r : db2->mReferees) {
                if (r->mFootballManagerID > 0 && referees.contains(r->mFootballManagerID)) {
                    auto ro = referees[r->mFootballManagerID];
                    String id1 = FifamNames::GetPersonStringId(gameVersion, ro->mFirstName, ro->mLastName, String(), Date(), 0);
                    String id2 = FifamNames::GetPersonStringId(gameVersion, r->mFirstName, r->mLastName, String(), Date(), 0);
                    if (id1 != id2) {
                        w.WriteLineWithSeparator(L'\t', id1, id2, r->mFootballManagerID, r->mFirstName + L" " + r->mLastName, r->mFirstName, r->mLastName, FifamTr(r->mCountry->mName));
                        renReferees.emplace_back(id1, id2);
                    }
                }
            }
        }
        {
            FifamWriter w(L"foom_referee_no_id.txt");
            w.WriteLineWithSeparator(L'\t', L"id", L"foomid", L"name", L"firstname", L"lastname", L"country");
            for (auto const &r : db2->mReferees) {
                if (r->mFootballManagerID <= 0) {
                    String id = FifamNames::GetPersonStringId(gameVersion, r->mFirstName, r->mLastName, String(), Date(), 0);
                    w.WriteLineWithSeparator(L'\t', id, r->mFootballManagerID, r->mFirstName + L" " + r->mLastName, r->mFirstName, r->mLastName, FifamTr(r->mCountry->mName));
                }
            }
        }
        {
            Map<UInt, FifamReferee *> referees;
            for (auto const &r : db1->mReferees) {
                if (r->mFootballManagerID > 0)
                    referees[r->mFootballManagerID] = r;
            }
            FifamWriter w(L"foom_referee_added.txt");
            w.WriteLineWithSeparator(L'\t', L"id", L"foomid", L"name", L"firstname", L"lastname", L"country");
            for (auto const &r : db2->mReferees) {
                if (r->mFootballManagerID > 0 && !referees.contains(r->mFootballManagerID)) {
                    String id = FifamNames::GetPersonStringId(gameVersion, r->mFirstName, r->mLastName, String(), Date(), 0);
                    w.WriteLineWithSeparator(L'\t', id, r->mFootballManagerID, r->mFirstName + L" " + r->mLastName, r->mFirstName, r->mLastName, FifamTr(r->mCountry->mName));
                }
            }
        }
        // staff
        {
            Map<String, FifamStaff *> staffs;
            for (auto const &s : db2->mStaffs)
                staffs[s->mWriteableStringID] = s;
            FifamWriter w(L"staff_removed_ids.txt");
            w.WriteLineWithSeparator(L'\t', L"id", L"foomid", L"name", L"firstname", L"lastname", L"pseudonym", L"nickname", L"birthdate", L"nationality",
                L"clubname", L"clubcountry", "clubleaguelevel", L"position", L"level", L"talent");
            for (auto const &s : db1->mStaffs) {
                if (!staffs.contains(s->mWriteableStringID)) {
                    UInt clubLeagueLevel = 255;
                    String clubName, clubCountry;
                    if (s->mClub) {
                        clubName = FifamTr(s->mClub->mName);
                        clubCountry = FifamTr(s->mClub->mCountry->mName);
                        if (clubLeague1.contains(s->mClub))
                            clubLeagueLevel = clubLeague1[s->mClub]->mLeagueLevel + 1;
                    }
                    UInt level = 0;
                    if (s->mClubPosition == FifamClubStaffPosition::Manager || s->mClubPosition == FifamClubStaffPosition::ChiefExec || s->mClubPosition == FifamClubStaffPosition::President)
                        level = s->GetManagerLevel() * 99 / 15;
                    else
                        level = s->GetStaffLevel();
                    w.WriteLineWithSeparator(L'\t', s->mWriteableStringID, s->mFootballManagerID, s->GetName(), s->mFirstName, s->mLastName, s->mPseudonym, s->mNickname,
                        s->mBirthday.ToString(), NationName(db1, s->mNationality[0]), clubName, clubCountry, clubLeagueLevel, s->mClubPosition.ToStr(), level, s->mTalent);
                }
            }
        }
        {
            Map<String, FifamStaff *> staffs;
            for (auto const &s : db1->mStaffs)
                staffs[s->mWriteableStringID] = s;
            FifamWriter w(L"staff_added_ids.txt");
            w.WriteLineWithSeparator(L'\t', L"id", L"foomid", L"name", L"firstname", L"lastname", L"pseudonym", L"nickname", L"birthdate", L"nationality",
                L"clubname", L"clubcountry", "clubleaguelevel", L"position", L"level", L"talent");
            for (auto const &s : db2->mStaffs) {
                if (!staffs.contains(s->mWriteableStringID)) {
                    UInt clubLeagueLevel = 255;
                    String clubName, clubCountry;
                    if (s->mClub) {
                        clubName = FifamTr(s->mClub->mName);
                        clubCountry = FifamTr(s->mClub->mCountry->mName);
                        if (clubLeague2.contains(s->mClub))
                            clubLeagueLevel = clubLeague2[s->mClub]->mLeagueLevel + 1;
                    }
                    UInt level = 0;
                    if (s->mClubPosition == FifamClubStaffPosition::Manager || s->mClubPosition == FifamClubStaffPosition::ChiefExec || s->mClubPosition == FifamClubStaffPosition::President)
                        level = s->GetManagerLevel() * 99 / 15;
                    else
                        level = s->GetStaffLevel();
                    w.WriteLineWithSeparator(L'\t', s->mWriteableStringID, s->mFootballManagerID, s->GetName(), s->mFirstName, s->mLastName, s->mPseudonym, s->mNickname,
                        s->mBirthday.ToString(), NationName(db2, s->mNationality[0]), clubName, clubCountry, clubLeagueLevel, s->mClubPosition.ToStr(), level, s->mTalent);
                }
            }
        }
        {
            Map<UInt, FifamStaff *> staffs;
            for (auto const &s : db2->mStaffs) {
                if (s->mFootballManagerID > 0)
                    staffs[s->mFootballManagerID] = s;
            }
            FifamWriter w(L"foom_staff_removed.txt");
            w.WriteLineWithSeparator(L'\t', L"id", L"foomid", L"name", L"firstname", L"lastname", L"pseudonym", L"nickname", L"birthdate", L"nationality",
                L"clubname", L"clubcountry", "clubleaguelevel", L"position", L"level", L"talent");
            for (auto const &s : db1->mStaffs) {
                if (s->mFootballManagerID > 0 && !staffs.contains(s->mFootballManagerID)) {
                    UInt clubLeagueLevel = 255;
                    String clubName, clubCountry;
                    if (s->mClub) {
                        clubName = FifamTr(s->mClub->mName);
                        clubCountry = FifamTr(s->mClub->mCountry->mName);
                        if (clubLeague1.contains(s->mClub))
                            clubLeagueLevel = clubLeague1[s->mClub]->mLeagueLevel + 1;
                    }
                    UInt level = 0;
                    if (s->mClubPosition == FifamClubStaffPosition::Manager || s->mClubPosition == FifamClubStaffPosition::ChiefExec || s->mClubPosition == FifamClubStaffPosition::President)
                        level = s->GetManagerLevel() * 99 / 15;
                    else
                        level = s->GetStaffLevel();
                    w.WriteLineWithSeparator(L'\t', s->mWriteableStringID, s->mFootballManagerID, s->GetName(), s->mFirstName, s->mLastName, s->mPseudonym, s->mNickname,
                        s->mBirthday.ToString(), NationName(db1, s->mNationality[0]), clubName, clubCountry, clubLeagueLevel, s->mClubPosition.ToStr(), level, s->mTalent);
                }
            }
        }
        {
            Map<UInt, FifamStaff *> staffs;
            for (auto const &s : db1->mStaffs) {
                if (s->mFootballManagerID > 0)
                    staffs[s->mFootballManagerID] = s;
            }
            FifamWriter w(L"foom_staff_added.txt");
            w.WriteLineWithSeparator(L'\t', L"id", L"foomid", L"name", L"firstname", L"lastname", L"pseudonym", L"nickname", L"birthdate", L"nationality",
                L"clubname", L"clubcountry", "clubleaguelevel", L"position", L"level", L"talent");
            for (auto const &s : db2->mStaffs) {
                if (s->mFootballManagerID > 0 && !staffs.contains(s->mFootballManagerID)) {
                    UInt clubLeagueLevel = 255;
                    String clubName, clubCountry;
                    if (s->mClub) {
                        clubName = FifamTr(s->mClub->mName);
                        clubCountry = FifamTr(s->mClub->mCountry->mName);
                        if (clubLeague2.contains(s->mClub))
                            clubLeagueLevel = clubLeague2[s->mClub]->mLeagueLevel + 1;
                    }
                    UInt level = 0;
                    if (s->mClubPosition == FifamClubStaffPosition::Manager || s->mClubPosition == FifamClubStaffPosition::ChiefExec || s->mClubPosition == FifamClubStaffPosition::President)
                        level = s->GetManagerLevel() * 99 / 15;
                    else
                        level = s->GetStaffLevel();
                    w.WriteLineWithSeparator(L'\t', s->mWriteableStringID, s->mFootballManagerID, s->GetName(), s->mFirstName, s->mLastName, s->mPseudonym, s->mNickname,
                        s->mBirthday.ToString(), NationName(db2, s->mNationality[0]), clubName, clubCountry, clubLeagueLevel, s->mClubPosition.ToStr(), level, s->mTalent);
                }
            }
        }
        {
            Map<UInt, FifamStaff *> staffs;
            for (auto const &s : db1->mStaffs) {
                if (s->mFootballManagerID > 0)
                    staffs[s->mFootballManagerID] = s;
            }
            FifamWriter w(L"foom_staff_changed_id.txt");
            w.WriteLineWithSeparator(L'\t', L"oldid", L"newid", L"foomid", L"name", L"firstname", L"lastname", L"pseudonym", L"nickname", L"birthdate", L"nationality",
                L"clubname", L"clubcountry", "clubleaguelevel", L"position", L"level", L"talent");
            for (auto const &s : db2->mStaffs) {
                if (s->mFootballManagerID > 0 && staffs.contains(s->mFootballManagerID)) {
                    FifamStaff *olds = staffs[s->mFootballManagerID];
                    if (s->mWriteableStringID != olds->mWriteableStringID) {
                        UInt clubLeagueLevel = 255;
                        String clubName, clubCountry;
                        if (s->mClub) {
                            clubName = FifamTr(s->mClub->mName);
                            clubCountry = FifamTr(s->mClub->mCountry->mName);
                            if (clubLeague2.contains(s->mClub))
                                clubLeagueLevel = clubLeague2[s->mClub]->mLeagueLevel + 1;
                        }
                        UInt level = 0;
                        if (s->mClubPosition == FifamClubStaffPosition::Manager || s->mClubPosition == FifamClubStaffPosition::ChiefExec || s->mClubPosition == FifamClubStaffPosition::President)
                            level = s->GetManagerLevel() * 99 / 15;
                        else
                            level = s->GetStaffLevel();
                        w.WriteLineWithSeparator(L'\t', olds->mWriteableStringID, s->mWriteableStringID, s->mFootballManagerID, s->GetName(), s->mFirstName, s->mLastName, s->mPseudonym, s->mNickname,
                            s->mBirthday.ToString(), NationName(db2, s->mNationality[0]), clubName, clubCountry, clubLeagueLevel, s->mClubPosition.ToStr(), level, s->mTalent);
                        renPlayersAndStaff.emplace_back(olds->mWriteableStringID, s->mWriteableStringID);
                    }
                }
            }
        }
        {
            FifamWriter w(L"foom_staff_no_id.txt");
            w.WriteLineWithSeparator(L'\t', L"id", L"foomid", L"name", L"firstname", L"lastname", L"pseudonym", L"nickname", L"birthdate", L"nationality",
                L"clubname", L"clubcountry", "clubleaguelevel", L"position", L"level", L"talent");
            for (auto const &s : db2->mStaffs) {
                if (s->mFootballManagerID <= 0) {
                    UInt clubLeagueLevel = 255;
                    String clubName, clubCountry;
                    if (s->mClub) {
                        clubName = FifamTr(s->mClub->mName);
                        clubCountry = FifamTr(s->mClub->mCountry->mName);
                        if (clubLeague2.contains(s->mClub))
                            clubLeagueLevel = clubLeague2[s->mClub]->mLeagueLevel + 1;
                    }
                    UInt level = 0;
                    if (s->mClubPosition == FifamClubStaffPosition::Manager || s->mClubPosition == FifamClubStaffPosition::ChiefExec || s->mClubPosition == FifamClubStaffPosition::President)
                        level = s->GetManagerLevel() * 99 / 15;
                    else
                        level = s->GetStaffLevel();
                    w.WriteLineWithSeparator(L'\t', s->mWriteableStringID, s->mFootballManagerID, s->GetName(), s->mFirstName, s->mLastName, s->mPseudonym, s->mNickname,
                        s->mBirthday.ToString(), NationName(db2, s->mNationality[0]), clubName, clubCountry, clubLeagueLevel, s->mClubPosition.ToStr(), level, s->mTalent);
                }
            }
        }
        // players
        {
            Map<String, FifamPlayer *> players;
            for (auto const &p : db2->mPlayers)
                players[p->mWriteableStringID] = p;
            FifamWriter w(L"player_removed_ids.txt");
            w.WriteLineWithSeparator(L'\t', L"id", L"foomid", L"name", L"firstname", L"lastname", L"pseudonym", L"nickname", L"birthdate", L"nationality",
                L"clubname", L"clubcountry", "clubleaguelevel", L"position", L"level", L"talent");
            for (auto const &p : db1->mPlayers) {
                if (!players.contains(p->mWriteableStringID)) {
                    UInt clubLeagueLevel = 255;
                    String clubName, clubCountry;
                    if (p->mClub) {
                        clubName = FifamTr(p->mClub->mName);
                        clubCountry = FifamTr(p->mClub->mCountry->mName);
                        if (clubLeague1.contains(p->mClub))
                            clubLeagueLevel = clubLeague1[p->mClub]->mLeagueLevel + 1;
                    }
                    w.WriteLineWithSeparator(L'\t', p->mWriteableStringID, p->mFootballManagerID, p->GetName(), p->mFirstName, p->mLastName, p->mPseudonym, p->mNickname,
                        p->mBirthday.ToString(), NationName(db1, p->mNationality[0]), clubName, clubCountry, clubLeagueLevel, p->mMainPosition.ToStr(), p->GetLevel(), p->mTalent);
                }
            }
        }
        {
            Map<String, FifamPlayer *> players;
            for (auto const &p : db1->mPlayers)
                players[p->mWriteableStringID] = p;
            FifamWriter w(L"player_added_ids.txt");
            w.WriteLineWithSeparator(L'\t', L"id", L"foomid", L"name", L"firstname", L"lastname", L"pseudonym", L"nickname", L"birthdate", L"nationality",
                L"clubname", L"clubcountry", "clubleaguelevel", L"position", L"level", L"talent");
            for (auto const &p : db2->mPlayers) {
                if (!players.contains(p->mWriteableStringID)) {
                    UInt clubLeagueLevel = 255;
                    String clubName, clubCountry;
                    if (p->mClub) {
                        clubName = FifamTr(p->mClub->mName);
                        clubCountry = FifamTr(p->mClub->mCountry->mName);
                        if (clubLeague2.contains(p->mClub))
                            clubLeagueLevel = clubLeague2[p->mClub]->mLeagueLevel + 1;
                    }
                    w.WriteLineWithSeparator(L'\t', p->mWriteableStringID, p->mFootballManagerID, p->GetName(), p->mFirstName, p->mLastName, p->mPseudonym, p->mNickname,
                        p->mBirthday.ToString(), NationName(db2, p->mNationality[0]), clubName, clubCountry, clubLeagueLevel, p->mMainPosition.ToStr(), p->GetLevel(), p->mTalent);
                }
            }
        }
        {
            FifamWriter w(L"foom_players_no_id.txt");
            w.WriteLineWithSeparator(L'\t', L"id", L"foomid", L"name", L"firstname", L"lastname", L"pseudonym", L"nickname", L"birthdate", L"nationality",
                L"clubname", L"clubcountry", "clubleaguelevel", L"position", L"level", L"talent");
            for (auto const &p : db2->mPlayers) {
                if (p->mFootballManagerID <= 0) {
                    UInt clubLeagueLevel = 255;
                    String clubName, clubCountry;
                    if (p->mClub) {
                        clubName = FifamTr(p->mClub->mName);
                        clubCountry = FifamTr(p->mClub->mCountry->mName);
                        if (clubLeague2.contains(p->mClub))
                            clubLeagueLevel = clubLeague2[p->mClub]->mLeagueLevel + 1;
                    }
                    w.WriteLineWithSeparator(L'\t', p->mWriteableStringID, p->mFootballManagerID, p->GetName(), p->mFirstName, p->mLastName, p->mPseudonym, p->mNickname,
                        p->mBirthday.ToString(), NationName(db2, p->mNationality[0]), clubName, clubCountry, clubLeagueLevel, p->mMainPosition.ToStr(), p->GetLevel(), p->mTalent);
                }
            }
        }
        {
            Map<UInt, FifamPlayer *> players;
            for (auto const &p : db2->mPlayers) {
                if (p->mFootballManagerID > 0)
                    players[p->mFootballManagerID] = p;
            }
            FifamWriter w(L"foom_players_removed.txt");
            w.WriteLineWithSeparator(L'\t', L"id", L"foomid", L"name", L"firstname", L"lastname", L"pseudonym", L"nickname", L"birthdate", L"nationality",
                L"clubname", L"clubcountry", "clubleaguelevel", L"position", L"level", L"talent");
            for (auto const &p : db1->mPlayers) {
                if (p->mFootballManagerID > 0 && !players.contains(p->mFootballManagerID)) {
                    UInt clubLeagueLevel = 255;
                    String clubName, clubCountry;
                    if (p->mClub) {
                        clubName = FifamTr(p->mClub->mName);
                        clubCountry = FifamTr(p->mClub->mCountry->mName);
                        if (clubLeague1.contains(p->mClub))
                            clubLeagueLevel = clubLeague1[p->mClub]->mLeagueLevel + 1;
                    }
                    w.WriteLineWithSeparator(L'\t', p->mWriteableStringID, p->mFootballManagerID, p->GetName(), p->mFirstName, p->mLastName, p->mPseudonym, p->mNickname,
                        p->mBirthday.ToString(), NationName(db1, p->mNationality[0]), clubName, clubCountry, clubLeagueLevel, p->mMainPosition.ToStr(), p->GetLevel(), p->mTalent);
                }
            }
        }
        {
            Map<UInt, FifamPlayer *> players;
            for (auto const &p : db1->mPlayers) {
                if (p->mFootballManagerID > 0)
                    players[p->mFootballManagerID] = p;
            }
            FifamWriter w(L"foom_players_added.txt");
            w.WriteLineWithSeparator(L'\t', L"id", L"foomid", L"name", L"firstname", L"lastname", L"pseudonym", L"nickname", L"birthdate", L"nationality",
                L"clubname", L"clubcountry", "clubleaguelevel", L"position", L"level", L"talent");
            for (auto const &p : db2->mPlayers) {
                if (p->mFootballManagerID > 0 && !players.contains(p->mFootballManagerID)) {
                    UInt clubLeagueLevel = 255;
                    String clubName, clubCountry;
                    if (p->mClub) {
                        clubName = FifamTr(p->mClub->mName);
                        clubCountry = FifamTr(p->mClub->mCountry->mName);
                        if (clubLeague2.contains(p->mClub))
                            clubLeagueLevel = clubLeague2[p->mClub]->mLeagueLevel + 1;
                    }
                    w.WriteLineWithSeparator(L'\t', p->mWriteableStringID, p->mFootballManagerID, p->GetName(), p->mFirstName, p->mLastName, p->mPseudonym, p->mNickname,
                        p->mBirthday.ToString(), NationName(db2, p->mNationality[0]), clubName, clubCountry, clubLeagueLevel, p->mMainPosition.ToStr(), p->GetLevel(), p->mTalent);
                }
            }
        }
        {
            Map<UInt, FifamPlayer *> players;
            for (auto const &p : db1->mPlayers) {
                if (p->mFootballManagerID > 0)
                    players[p->mFootballManagerID] = p;
            }
            FifamWriter w(L"foom_players_changed_id.txt");
            w.WriteLineWithSeparator(L'\t', L"oldid", L"newid", "change_type", L"foomid", L"name", L"firstname", L"lastname", L"pseudonym", L"nickname", L"birthdate", L"nationality",
                L"clubname", L"clubcountry", "clubleaguelevel", L"position", L"level", L"talent");
            for (auto const &p : db2->mPlayers) {
                try {
                    if (p->mFootballManagerID > 0 && players.contains(p->mFootballManagerID)) {
                        FifamPlayer *oldp = players[p->mFootballManagerID];
                        if (p->mWriteableStringID != oldp->mWriteableStringID) {
                            UInt clubLeagueLevel = 255;
                            String clubName, clubCountry;
                            if (p->mClub) {
                                clubName = FifamTr(p->mClub->mName);
                                clubCountry = FifamTr(p->mClub->mCountry->mName);
                                if (clubLeague2.contains(p->mClub))
                                    clubLeagueLevel = clubLeague2[p->mClub]->mLeagueLevel + 1;
                            }
                            String namePart1 = p->mWriteableStringID.substr(0, p->mWriteableStringID.size() - 8);
                            String datePart1 = p->mWriteableStringID.substr(p->mWriteableStringID.size() - 8);
                            String namePart2 = oldp->mWriteableStringID.substr(0, oldp->mWriteableStringID.size() - 8);
                            String datePart2 = oldp->mWriteableStringID.substr(oldp->mWriteableStringID.size() - 8);
                            String changeType;
                            if (namePart1 != namePart2)
                                changeType = L"name";
                            if (datePart1 != datePart2) {
                                if (changeType.empty())
                                    changeType = L"date";
                                else
                                    changeType += L"+date";
                            }
                            w.WriteLineWithSeparator(L'\t', oldp->mWriteableStringID, p->mWriteableStringID, changeType, p->mFootballManagerID, p->GetName(), p->mFirstName, p->mLastName, p->mPseudonym, p->mNickname,
                                p->mBirthday.ToString(), NationName(db2, p->mNationality[0]), clubName, clubCountry, clubLeagueLevel, p->mMainPosition.ToStr(), p->GetLevel(), p->mTalent);
                            renPlayersAndStaff.emplace_back(oldp->mWriteableStringID, p->mWriteableStringID);
                            renXXL.emplace_back(oldp->mWriteableStringID, p->mWriteableStringID);
                        }
                    }
                }
                catch (exception e) {
                    ::Error("%s\n%d", e.what(), p->mFootballManagerID);
                }
            }
        }
        {
            Map<UInt, FifamPlayer *> players;
            for (auto const &p : db1->mPlayers) {
                if (p->mFootballManagerID > 0)
                    players[p->mFootballManagerID] = p;
            }
            FifamWriter w(L"foom_players_changed_level.txt");
            w.WriteLineWithSeparator(L'\t', L"foomid", L"name", L"birthdate", L"nationality", L"clubname",
                L"clubcountry", "clubleaguelevel", L"position", L"oldposition", L"positionchange", L"level", L"oldlevel", L"levelchange", L"talent");
            for (auto const &p : db2->mPlayers) {
                if (p->mFootballManagerID > 0 && players.contains(p->mFootballManagerID)) {
                    FifamPlayer *oldp = players[p->mFootballManagerID];
                    if (p->GetLevel() != oldp->GetLevel()) {
                        UInt clubLeagueLevel = 255;
                        String clubName, clubCountry;
                        if (p->mClub) {
                            clubName = FifamTr(p->mClub->mName);
                            clubCountry = FifamTr(p->mClub->mCountry->mName);
                            if (clubLeague2.contains(p->mClub))
                                clubLeagueLevel = clubLeague2[p->mClub]->mLeagueLevel + 1;
                        }
                        w.WriteLineWithSeparator(L'\t', p->mFootballManagerID, p->GetName(), p->mBirthday.ToString(), NationName(db2, p->mNationality[0]), clubName,
                            clubCountry, clubLeagueLevel, p->mMainPosition.ToStr(), oldp->mMainPosition.ToStr(), p->mMainPosition != oldp->mMainPosition,
                            p->GetLevel(), oldp->GetLevel(), p->GetLevel() - oldp->GetLevel(), p->mTalent);
                    }
                }
            }
        }
        {
            Map<UInt, FifamPlayer *> players;
            for (auto const &p : db1->mPlayers) {
                if (p->mFootballManagerID > 0)
                    players[p->mFootballManagerID] = p;
            }
            FifamWriter w(L"foom_players_changed_talent.txt");
            w.WriteLineWithSeparator(L'\t', L"foomid", L"name", L"birthdate", L"nationality", L"clubname", L"clubcountry",
                "clubleaguelevel", L"position", L"level", L"oldlevel", L"talent", L"oldtalent", L"talentchange");
            for (auto const &p : db2->mPlayers) {
                if (p->mFootballManagerID > 0 && players.contains(p->mFootballManagerID)) {
                    FifamPlayer *oldp = players[p->mFootballManagerID];
                    if (p->mTalent != oldp->mTalent) {
                        UInt clubLeagueLevel = 255;
                        String clubName, clubCountry;
                        if (p->mClub) {
                            clubName = FifamTr(p->mClub->mName);
                            clubCountry = FifamTr(p->mClub->mCountry->mName);
                            if (clubLeague2.contains(p->mClub))
                                clubLeagueLevel = clubLeague2[p->mClub]->mLeagueLevel + 1;
                        }
                        w.WriteLineWithSeparator(L'\t', p->mFootballManagerID, p->GetName(), p->mBirthday.ToString(), NationName(db2, p->mNationality[0]), clubName,
                            clubCountry, clubLeagueLevel, p->mMainPosition.ToStr(), p->GetLevel(), oldp->GetLevel(), p->mTalent, oldp->mTalent,
                            p->mTalent - oldp->mTalent);
                    }
                }
            }
        }
        {
            FifamWriter w(L"rename_players_and_staff.bat");
            for (auto const &r : renPlayersAndStaff)
                w.WriteLine(L"ren " + r.first + L".png " + r.second + L".png");
        }
        {
            FifamWriter w(L"rename_xxl.bat");
            for (auto const &r : renXXL)
                w.WriteLine(L"ren " + r.first + L".png " + r.second + L".png");
        }
        {
            FifamWriter w(L"rename_referees.bat");
            for (auto const &r : renReferees)
                w.WriteLine(L"ren " + r.first + L".png " + r.second + L".png");
        }
    }
};
