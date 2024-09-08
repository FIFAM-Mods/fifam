#pragma once
#include "FifamDbEnvironment.h"

class CopyPlayers {
public:
    FifamDatabase *baseDb = nullptr;
    FifamDatabase *updateDb = nullptr;
    FifamDatabase *referenceDb = nullptr;

    void UpdateCountryPointer(FifamCountry *&countryPtr) {
        if (countryPtr)
            countryPtr = baseDb->mCountries[countryPtr->mId - 1];
    }

    void UpdateClubPointer(FifamClub *&clubPtr) {
        if (clubPtr)
            clubPtr = baseDb->GetClubFromUID(clubPtr->mUniqueID);
    }

    void UpdateClubLink(FifamClubLink &clubLink) {
        UpdateClubPointer(clubLink.mPtr);
    }

    template<typename T>
    void Move(T& dst, T& src) {
        dst = src;
        src.clear();
    }

    CopyPlayers() {
        FifamDatabase::mReadingOptions.mUseCustomFormations = true;
        FifamDatabase::mReadingOptions.mReadPersons = false;
        baseDb = new FifamDatabase(13, "db1");
        FifamDatabase::mReadingOptions.mReadPersons = true;
        updateDb = new FifamDatabase(13, "db2");
        referenceDb = new FifamDatabase(13, "db1");

        //::Error("setup reference info");

        // setup reference info
        Map<Int, FifamPlayer *> referencePlayers;
        Map<Int, FifamStaff *> referenceStaff;
        for (auto p : referenceDb->mPlayers) {
            if (p->mFootballManagerID >= 0 && p->mFootballManagerID < 2002042000)
                referencePlayers[p->mFootballManagerID] = p;
        }
        for (auto s : referenceDb->mStaffs) {
            if (s->mFootballManagerID >= 0 && s->mFootballManagerID < 2002042000)
                referenceStaff[s->mFootballManagerID] = s;
        }

        //::Error("copy club players and staff");
        
        // copy club players and staff
        Move(baseDb->mPersonsMap, updateDb->mPersonsMap);
        Move(baseDb->mPlayers, updateDb->mPlayers);
        Move(baseDb->mStaffs, updateDb->mStaffs);
        FifamWriter removedPlayersWriter("removed_players.csv");
        FifamWriter removedStaffWriter("removed_staff.csv");
        removedPlayersWriter.WriteLine(L"ID,Country,Club,Name,Overall");
        removedStaffWriter.WriteLine(L"ID,Country,Club,Name,Role,Overall");
        for (auto clubUpdate : updateDb->mClubs) {
            auto club = baseDb->GetClubFromUID(clubUpdate->mUniqueID);
            if (club) {
                Move(club->mPlayers, clubUpdate->mPlayers);
                Move(club->mStaffs, clubUpdate->mStaffs);
                club->mCaptains = clubUpdate->mCaptains;
            }
            else {
                for (auto const &p : clubUpdate->mPlayers) {
                    removedPlayersWriter.WriteLine(p->mFootballManagerID, Quoted(FifamTr(clubUpdate->mCountry->mName)),
                        Quoted(FifamTr(clubUpdate->mName)), p->GetName(), p->GetLevel());
                    baseDb->mPlayers.erase(p);
                    baseDb->mPersonsMap.erase(p->mID);
                }
                clubUpdate->mPlayers.clear();
                for (auto const &p : clubUpdate->mStaffs) {
                    removedStaffWriter.WriteLine(p->mFootballManagerID, Quoted(FifamTr(clubUpdate->mCountry->mName)),
                        Quoted(FifamTr(clubUpdate->mName)), p->GetName(), p->mClubPosition.ToStr(), p->GetStaffLevel());
                    baseDb->mStaffs.erase(p);
                    baseDb->mPersonsMap.erase(p->mID);
                }
                clubUpdate->mStaffs.clear();
            }
        }
        removedPlayersWriter.Close();
        removedStaffWriter.Close();

        //::Error("copy NT players and staff");

        // copy NT players and staff
        for (UInt i = 0; i < FifamDatabase::NUM_COUNTRIES; i++) {
            Move(baseDb->mCountries[i]->mNationalTeam.mStaffs, updateDb->mCountries[i]->mNationalTeam.mStaffs);
            baseDb->mCountries[i]->mNationalTeam.mCaptains = updateDb->mCountries[i]->mNationalTeam.mCaptains;
        }

        //::Error("update player club links");

        // update club links
        for (auto &p : baseDb->mPlayers) {
            UpdateClubPointer(p->mClub);
            UpdateClubLink(p->mFavouriteClub);
            UpdateClubLink(p->mWouldnSignFor);
            UpdateClubLink(p->mFirstClub);
            UpdateClubLink(p->mPreviousClub);
            for (auto &t : p->mTransferRumors)
                UpdateClubLink(t);
            UpdateClubLink(p->mContract.mBuyBackClauseClub);
            if (p->mContract.mBuyBackClauseEnabled && !p->mContract.mBuyBackClauseClub.IsValid())
                p->mContract.mBuyBackClauseEnabled = false;
            for (auto &h : p->mHistory.mEntries)
                UpdateClubLink(h.mClub);
            UpdateClubLink(p->mStartingConditions.mLoan.mLoanedClub);
            if (p->mStartingConditions.mLoan.Enabled() && !p->mStartingConditions.mLoan.mLoanedClub.IsValid())
                p->mStartingConditions.mLoan.Disable();
            UpdateClubLink(p->mStartingConditions.mFutureTransfer.mNewClub);
            if (p->mStartingConditions.mFutureTransfer.Enabled() && !p->mStartingConditions.mFutureTransfer.mNewClub.IsValid())
                p->mStartingConditions.mFutureTransfer.Disable();
            UpdateClubLink(p->mStartingConditions.mFutureLoan.mLoanedClub);
            if (p->mStartingConditions.mFutureLoan.Enabled() && !p->mStartingConditions.mFutureLoan.mLoanedClub.IsValid())
                p->mStartingConditions.mFutureLoan.Disable();
            UpdateClubLink(p->mStartingConditions.mFutureReLoan.mLoanedClub);
            if (p->mStartingConditions.mFutureReLoan.Enabled() && !p->mStartingConditions.mFutureReLoan.mLoanedClub.IsValid())
                p->mStartingConditions.mFutureReLoan.Disable();
            // update from reference
            if (Utils::Contains(referencePlayers, p->mFootballManagerID)) {
                auto ref = referencePlayers[p->mFootballManagerID];
                p->mBirthday = ref->mBirthday;
                p->mFirstName = ref->mFirstName;
                p->mLastName = ref->mLastName;
                p->mNickname = ref->mNickname;
                p->mPseudonym = ref->mPseudonym;
            }
        }

        //::Error("update staff club links");

        for (auto &s : baseDb->mStaffs) {
            UpdateClubPointer(s->mClub);
            UpdateClubLink(s->mFavouriteClub);
            UpdateClubLink(s->mWouldnSignFor);
            // update from reference
            if (Utils::Contains(referenceStaff, s->mFootballManagerID)) {
                auto ref = referenceStaff[s->mFootballManagerID];
                s->mBirthday = ref->mBirthday;
                s->mFirstName = ref->mFirstName;
                s->mLastName = ref->mLastName;
                s->mNickname = ref->mNickname;
                s->mPseudonym = ref->mPseudonym;
            }
        }

        //::Error("copy referee");

        // copy referee
        Move(baseDb->mReferees, updateDb->mReferees);
        for (UInt i = 0; i < FifamDatabase::NUM_COUNTRIES; i++)
            Move(baseDb->mCountries[i]->mReferees, updateDb->mCountries[i]->mReferees);
        for (auto &r : baseDb->mReferees)
            UpdateCountryPointer(r->mCountry);

        //::Error("copy portraits");

        // copy portraits
        //Vector<Path> fifamPortrats = {
        //   R"(D:\Projects\fifam\content\fm13\art_02\portraits\club\160x160)",
        //   R"(D:\Projects\fifam\content\fm13\art_03\portraits\club\160x160)",
        //   R"(D:\Projects\fifam\content\fm13\art_06\portraits\club\160x160)",
        //   R"(D:\Projects\fifam\content\fm13\art_07\portraits\club\160x160)",
        //   R"(D:\Projects\fifam\content\fm13\art_08\portraits\club\160x160)",
        //   R"(D:\Projects\fifam\content\fm13\art_09\portraits\club\160x160)",
        //   R"(C:\Users\Dmitri\Desktop\FM 23 update 1\_UPDATE_PORTRAITS_1\portraits\club\160x160)"
        //};
        //Vector<Path> fifamPortratsReferee = {
        //   R"(D:\Projects\fifam\content\fm13\art_02\portraits\Referees\160x160)",
        //   R"(C:\Users\Dmitri\Desktop\FM 23 update 1\_UPDATE_PORTRAITS_1\portraits\Referees\160x160)"
        //};
        //Vector<Path> foomPortrats = {
        //    R"(D:\Projects\fifam\output\tests\faces)",
        //    R"(D:\Projects\fifam\output\tests\faces_dt)",
        //    R"(D:\Projects\fifam\output\tests\faces_japan)"
        //};
        //Path playerPortraitsPath = "portraits\\club\\160x160";
        //Path refereePortraitsPath = "portraits\\Referees\\160x160";
        //create_directories(playerPortraitsPath);
        //create_directories(refereePortraitsPath);
        //auto ConvertPortrait = [](Path const& portraitPath, String const& fileName, Path const& outFolder) {
        //    Image portraitImg(portraitPath.string());
        //    if (portraitImg.baseRows() >= 150 && portraitImg.baseColumns() >= 150) {
        //        if (portraitImg.baseColumns() != portraitImg.baseRows()) {
        //            UInt newSize = Utils::Max(portraitImg.baseColumns(), portraitImg.baseRows());
        //            portraitImg.extent(Geometry(newSize, newSize), Magick::Color(0, 0, 0, 0), MagickCore::GravityType::CenterGravity);
        //        }
        //        portraitImg.filterType(FilterType::LanczosFilter);
        //        portraitImg.resize(Geometry(160, 160));
        //        portraitImg.defineValue("png", "color-type", "6");
        //        portraitImg.write((outFolder / (fileName + L".png")).string());
        //    }
        //};
        //baseDb->SetupWriteableStatus(13);
        //for (auto &[id, p] : baseDb->mPersonsMap) {
        //    if (p->mFootballManagerID >= 0) {
        //        bool hasPortrait = false;
        //        for (auto const& i : fifamPortrats) {
        //            if (exists(i / (p->mWriteableStringID + L".png"))) {
        //                hasPortrait = true;
        //                break;
        //            }
        //        }
        //        if (!hasPortrait) {
        //            for (auto const& i : foomPortrats) {
        //                Path foomPath = i / Utils::Format("%d.png", p->mFootballManagerID);
        //                if (exists(foomPath))
        //                    ConvertPortrait(foomPath, p->mWriteableStringID, playerPortraitsPath);
        //            }
        //        }
        //    }
        //}
        //for (auto& r : baseDb->mReferees) {
        //    if (r->mFootballManagerID >= 0) {
        //        auto writeableId = FifamNames::GetPersonStringId(13, r->mFirstName, r->mLastName, String(), Date(), 0);
        //        bool hasPortrait = false;
        //        for (auto const& i : fifamPortratsReferee) {
        //            if (exists(i / (writeableId + L".png"))) {
        //                hasPortrait = true;
        //                break;
        //            }
        //        }
        //        if (!hasPortrait) {
        //            for (auto const& i : foomPortrats) {
        //                Path foomPath = i / Utils::Format("%d.png", r->mFootballManagerID);
        //                if (exists(foomPath))
        //                    ConvertPortrait(foomPath, writeableId, refereePortraitsPath);
        //            }
        //        }
        //    }
        //}

        //::Error("write db");

        baseDb->mWritingOptions.mWriteFixtures = false;
        baseDb->mWritingOptions.mWriteExternalScripts = false;
        baseDb->Write(13, FifamDatabase::GetGameDbVersion(13), "db_new");
        delete baseDb;
        delete updateDb;
        delete referenceDb;
    }
};
