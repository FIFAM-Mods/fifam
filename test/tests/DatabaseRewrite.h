#pragma once
#include "FifamDbEnvironment.h"
#include "FifamPlayerLevel.h"
#include "FifamStaff.h"
#include "Random.h"

class DatabaseRewrite {
public:
    DatabaseRewrite() {
        UInt const gameVersion = 13;
        UInt const minBudget = 10'000;
        Path dbPathIn = L"E:\\Games\\FIFA Manager 13 Clean 1\\database";
        Path dbPathOut = L"E:\\Games\\FIFA Manager 13 Clean 1\\database 2";
        Path dbPathRef = L"E:\\Games\\FIFA Manager 13 Clean 1\\database correct";

        FifamDatabase::mReadingOptions.mUseCustomFormations = true;
        FifamDatabase *db = new FifamDatabase(gameVersion, dbPathIn);

        if (true) {
            FifamDatabase *ref_db = new FifamDatabase(gameVersion, dbPathRef);
            Map<UInt, FifamPlayer *> ref_players;
            Map<String, FifamStaff *> ref_staff;
            for (FifamPlayer *p : ref_db->mPlayers) {
                if (p->mFootballManagerID >= 0)
                    ref_players[p->mFootballManagerID] = p;
            }
            //for (FifamStaff *s : ref_db->mStaffs) {
            //    String id = s->GetStringUniqueId(gameVersion);
            //    auto it = ref_staff.find(id);
            //    if (it != ref_staff.end())
            //        ::Error(L"Staff id duplicated: %s", id.c_str());
            //    ref_staff[id] = s;
            //}
            for (FifamPlayer *p : db->mPlayers) {
                if (p->mFootballManagerID >= 0) {
                    auto it = ref_players.find(p->mFootballManagerID);
                    if (it != ref_players.end()) {
                        FifamPlayer *oldP = (*it).second;
                        if (p->mSpecialFace != oldP->mSpecialFace) {
                            ::Error(L"Special face was deassigned: %s", p->GetName().c_str());
                            p->mSpecialFace = oldP->mSpecialFace;
                        }
                        p->mAppearance = oldP->mAppearance;
                        continue;
                        //p->mHistory.mEntries.clear();
                        //for (auto h : oldP->mHistory.mEntries) {
                        //    if (h.mClub.IsValid()) {
                        //        h.mClub.mPtr = db->GetClubFromUID(h.mClub.mPtr->mUniqueID);
                        //        if (!h.mClub.mPtr)
                        //            h.mClub.mTeamType = FifamClubTeamType::First;
                        //    }
                        //    p->mHistory.mEntries.push_back(h);
                        //}
                        // talent
                        p->mTalent = oldP->mTalent;
                        // experience
                        p->mGeneralExperience = oldP->mGeneralExperience;
                        p->mInternationalExperience = oldP->mInternationalExperience;
                        p->mNationalExperience = oldP->mNationalExperience;
                        // character
                        p->mCharacter = oldP->mCharacter;
                        // position bias
                        p->mPositionBias = oldP->mPositionBias;
                        // main position
                        p->mMainPosition = oldP->mMainPosition;
                        // abilities
                        auto oldLeadership = p->mAttributes.Leadership;
                        p->mAttributes = oldP->mAttributes;
                        p->mAttributes.Leadership = oldLeadership;
                        // appearance
                        p->mAppearance = oldP->mAppearance;
                        // NT stats
                        p->mNationalTeamGoals = oldP->mNationalTeamGoals;
                        p->mNationalTeamMatches = oldP->mNationalTeamMatches;
                        p->mCurrentlyInNationalTeam = oldP->mCurrentlyInNationalTeam;
                        p->mRetiredFromNationalTeam = oldP->mRetiredFromNationalTeam;
                        // agent
                        p->mPlayerAgent = oldP->mPlayerAgent;
                        // other
                        p->mPlayingStyle = oldP->mPlayingStyle;

                        // Disabled this to convert shoes from FIFA
                        p->mShoeType = oldP->mShoeType;

                        // history
                        /*
                        p->mHistory.mEntries.clear();
                        for (auto h : oldP->mHistory.mEntries) {
                            if (h.mClub.IsValid()) {
                                h.mClub.mPtr = mFifamDatabase->GetClubFromUID(h.mClub.mPtr->mUniqueID);
                                if (!h.mClub.mPtr)
                                    h.mClub.mTeamType = FifamClubTeamType::First;
                            }
                            p->mHistory.mEntries.push_back(h);
                        }
                        */
                    }
                }
            }
            //for (FifamStaff *s : db->mStaffs) {
            //    String id = s->GetStringUniqueId(gameVersion);
            //    auto it = ref_staff.find(id);
            //    if (it != ref_staff.end()) {
            //        FifamStaff *oldS = (*it).second;
            //        s->mCoachPlayingOrientation = oldS->mCoachPlayingOrientation;
            //    }
            //}
            for (FifamClub *c : db->mClubs) {
                FifamClub *oldC = ref_db->GetClubFromUID(c->mUniqueID);
                if (oldC) {
                    /*
                    FifamTrSetAll(c->mName, FifamTr(oldC->mName));
                    FifamTrSetAll(c->mShortName, FifamTr(oldC->mShortName));
                    FifamTrSetAll(c->mShortName2, FifamTr(oldC->mShortName2));
                    FifamTrSetAll(c->mAbbreviation, FifamTr(oldC->mAbbreviation));
                    FifamTrSetAll(c->mCityName, FifamTr(oldC->mCityName));
                    FifamTrSetAll(c->mPlayerInText, FifamTr(oldC->mPlayerInText));
                    FifamTrSetAll(c->mFanName1, FifamTr(oldC->mFanName1));
                    FifamTrSetAll(c->mFanName2, FifamTr(oldC->mFanName2));
                    FifamTrSetAll(c->mFanName1Article, FifamTr(oldC->mFanName1Article));
                    FifamTrSetAll(c->mFanName2Article, FifamTr(oldC->mFanName2Article));
                    c->mMascotName = oldC->mMascotName;
                    c->mFanMembers = oldC->mFanMembers;
                    c->mMaxAttendance = oldC->mMaxAttendance;
                    c->mPotentialFansCount = oldC->mPotentialFansCount;
                    c->mAverageAttendanceLastSeason = oldC->mAverageAttendanceLastSeason;
                    c->mCountOfSoldSeasonTickets = oldC->mCountOfSoldSeasonTickets;
                    c->mTraditionalClub = oldC->mTraditionalClub;
                    c->mInternationalPrestige = oldC->mInternationalPrestige;
                    c->mNationalPrestige = oldC->mNationalPrestige;
                    c->mMediaPressure = oldC->mMediaPressure;
                    c->mIndividualTvMoney = oldC->mIndividualTvMoney;
                    */
                    c->mInitialCapital = oldC->mInitialCapital;
                    if (c->mInitialCapital > 0)
                        c->mTransferBudget = c->mInitialCapital / 2;
                    else
                        c->mTransferBudget = 0;

                    c->mFirstTeamLastSeasonInfo = oldC->mFirstTeamLastSeasonInfo;
                    c->mReserveTeamLastSeasonInfo = oldC->mReserveTeamLastSeasonInfo;

                    /*
                    c->mJointStockCompany = oldC->mJointStockCompany;
                    c->mRichGuyControlled = oldC->mRichGuyControlled;
                    c->mAddress = oldC->mAddress;
                    c->mTelephone = oldC->mTelephone;
                    c->mWebsiteAndMail = oldC->mWebsiteAndMail;
                    c->mFansites = oldC->mFansites;
                    */

                    // stadium
                    /*
                    FifamTrSetAll(c->mStadiumName, FifamTr(oldC->mStadiumName));
                    c->mStadiumSeatsCapacity = oldC->mStadiumSeatsCapacity;
                    c->mStadiumStandingsCapacity = oldC->mStadiumStandingsCapacity;
                    c->mStadiumVipCapacity = oldC->mStadiumVipCapacity;
                    c->mStadiumVenue = oldC->mStadiumVenue;
                    c->mStadiumType = oldC->mStadiumType;
                    c->mClubFacilities = oldC->mClubFacilities;
                    c->mYouthBoardingSchool = oldC->mYouthBoardingSchool;
                    c->mYouthCentre = oldC->mYouthCentre;
                    c->mAiStrategy = oldC->mAiStrategy;
                    c->mSettlement = oldC->mSettlement;
                    c->mLandscape = oldC->mLandscape;
                    */

                    // SPONSOR - TODO (need to update from foom)
                    //c->mSponsorAmount = oldC->mSponsorAmount;
                    //c->mSponsorDuration = oldC->mSponsorDuration;
                    //c->mSpecialSponsor = oldC->mSpecialSponsor;

                    /*
                    c->mGeoCoords = oldC->mGeoCoords;
                    c->mYouthPlayersAreBasques = oldC->mYouthPlayersAreBasques;
                    c->mYouthPlayersCountry = oldC->mYouthPlayersCountry;
                    c->mTransfersCountry = oldC->mTransfersCountry;

                    // kit
                    c->mKit = oldC->mKit;
                    // badge
                    c->mClubColour = oldC->mClubColour;
                    c->mClubColour2 = oldC->mClubColour2;
                    c->mMerchandiseColour = oldC->mMerchandiseColour;
                    c->mHeaderColour = oldC->mHeaderColour;
                    c->mBackgroundColour = oldC->mBackgroundColour;
                    */
                }
            }
        }

        //Continent,Country,ClubId,ClubName,Cash,Salaries,SalariesLeft,Transfers,TransfersLeft,Infrstructure,InfrstructureLeft,Misc,MiscLeft,Reserve
        //struct ClubBudget {
        //    Int cash = 0,
        //        salaries = 0,
        //        salariesLeft = 0,
        //        transfers = 0,
        //        transfersLeft = 0,
        //        infrastructure = 0,
        //        infrastructureLeft = 0,
        //        misc = 0,
        //        miscLeft = 0,
        //        reserve = 0;
        //};
        //
        //Map<UInt, ClubBudget> budgets;
        //FifamReader reader(L"D:\\Games\\FIFA Manager 13\\ClubBudgets.csv", gameVersion);
        //reader.SkipLine();
        //while (!reader.IsEof()) {
        //    if (reader.EmptyLine())
        //        reader.SkipLine();
        //    else {
        //        String d;
        //        ClubBudget b;
        //        UInt clubId = 0;
        //        reader.ReadLine(d, d, clubId, d, b.cash, b.salaries, b.salariesLeft, b.transfers, b.transfersLeft, b.infrastructure, b.infrastructureLeft,
        //            b.misc, b.miscLeft, b.reserve);
        //        if (clubId != 0)
        //            budgets[clubId] = b;
        //    }
        //}
        //
        //for (FifamClub *c : db->mClubs) {
        //    auto it = budgets.find(c->mUniqueID);
        //    if (it != budgets.end()) {
        //        auto const &b = (*it).second;
        //        Int total = 0;
        //        Int inc = 0;
        //        if (b.salariesLeft < 0)
        //            total += b.salariesLeft;
        //        if (b.transfersLeft < 0)
        //            total += b.transfersLeft;
        //        if (b.infrastructureLeft < 0)
        //            total += b.infrastructureLeft;
        //        if (b.miscLeft < 0)
        //            total += b.miscLeft;
        //        if (b.reserve < 0)
        //            total += b.reserve;
        //        if (total < 0)
        //            inc = minBudget + total * -1;
        //        else {
        //            total = b.salariesLeft + b.transfersLeft + b.infrastructureLeft + b.miscLeft + b.reserve;
        //            if (total < minBudget)
        //                inc = minBudget - total;
        //        }
        //        if (inc != 0) {
        //            Int old = c->mInitialCapital;
        //            c->mInitialCapital += inc;
        //            if (c->mInitialCapital >= 0)
        //                c->mTransferBudget = c->mInitialCapital / 2;
        //            std::wcout << L"Changed budget of " << FifamTr(c->mName) << L" from " << old << L" to " << c->mInitialCapital << std::endl;
        //        }
        //    }
        //}
        
        db->Write(gameVersion, FifamDatabase::GetGameDbVersion(gameVersion), dbPathOut);
    }
};
