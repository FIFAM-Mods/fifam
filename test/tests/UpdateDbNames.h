#pragma once
#include "FifamDbEnvironment.h"

class UpdateDbNames {
public:
    UpdateDbNames() {
        UInt const gameVersion = 13;
        Path dbPathNew = L"E:\\Games\\FIFA Manager 13\\database_final";
        Path dbPathOut = L"E:\\Games\\FIFA Manager 13\\database_final_updated";
        Path dbPathOld = L"E:\\Games\\FIFA Manager 13\\database";
        Path dbPathOrig = L"D:\\my\\projects\\fifam\\dbs\\fm14\\database";

        FifamDatabase::mReadingOptions.mUseCustomFormations = true;
        FifamDatabase *dbnew = new FifamDatabase(gameVersion, dbPathNew);
        FifamDatabase::mReadingOptions.mReadPersons = false;
        FifamDatabase *dbold = new FifamDatabase(gameVersion, dbPathOld);
        FifamDatabase* originalDb = new FifamDatabase(gameVersion, dbPathOrig);

        Map<UInt, FifamClub*> origClubByFifaID;
        for (FifamClub* c : originalDb->mClubs) {
            if (c->mFifaID != 0)
                origClubByFifaID[c->mFifaID] = c;
        }

        ///Map<UInt, FifamPlayer *> oldPlayers;
        ///
        ///for (FifamPlayer *p : dbold->mPlayers) {
        ///    if (p->mEmpicsId)
        ///        oldPlayers[p->mEmpicsId] = p;
        ///}
        // update clubs
        for (FifamClub *c : dbnew->mClubs) {
            auto oldc = dbold->GetClubFromUID(c->mUniqueID);
            if (oldc) {
                if (c->mCountry->mId != FifamNation::Ukraine && c->mCountry->mId != FifamNation::Russia && c->mCountry->mId != FifamNation::Belarus) {
                    c->mName = oldc->mName;
                    c->mName2 = oldc->mName2;
                    c->mShortName = oldc->mShortName;
                    c->mShortName2 = oldc->mShortName2;
                    c->mAbbreviation = oldc->mAbbreviation;
                }
                auto UpdateRecord = [](FifamClubRecord &r, FifamClubRecord const& from_record) {
                    if (r.mOpponentName.find(L'?') != String::npos) {
                        r.mAttendance = 0;
                        r.mOpponentName.clear();
                        r.mScore1 = 0;
                        r.mScore2 = 0;
                        r.mYear = 0;
                    }
                    if (r.mOpponentName.empty() && !from_record.mOpponentName.empty())
                        r = from_record;
                };
                UpdateRecord(c->mHistory.mRecordAttendance, oldc->mHistory.mRecordAttendance);
                UpdateRecord(c->mHistory.mRecordHomeWin, oldc->mHistory.mRecordHomeWin);
                UpdateRecord(c->mHistory.mRecordHomeDefeat, oldc->mHistory.mRecordHomeDefeat);
                UpdateRecord(c->mHistory.mRecordAwayWin, oldc->mHistory.mRecordAwayWin);
                UpdateRecord(c->mHistory.mRecordAwayDefeat, oldc->mHistory.mRecordAwayDefeat);

                if (c->mTransfersCountry[0] == FifamNation::None && oldc->mTransfersCountry[0] != FifamNation::None) {
                    c->mTransfersCountry[0] = oldc->mTransfersCountry[0];
                    c->mTransfersCountry[1] = oldc->mTransfersCountry[1];
                }

                if (c->mAddress.empty())
                    c->mAddress = oldc->mAddress;
                if (c->mTelephone.empty())
                    c->mTelephone = oldc->mTelephone;
                if (c->mWebsiteAndMail.empty())
                    c->mWebsiteAndMail = oldc->mWebsiteAndMail;
                if (c->mFansites.empty())
                    c->mFansites = oldc->mFansites;
                if (c->mFanMembers == 0)
                    c->mFanMembers = oldc->mFanMembers;
                if (c->mMascotName.empty())
                    c->mMascotName = oldc->mMascotName;

                auto UpdateTranslationsStr = [](FifamTrArray<String> &ary, FifamTrArray<String> const &from_ary) {
                    for (UInt i = 0; i < FifamTranslation::NUM_TRANSLATIONS; i++)  {
                        if (ary[i].empty() && !from_ary[i].empty())
                            ary[i] = from_ary[i];
                    }
                };
                auto UpdateTranslationsArticle = [](FifamTrArray<UChar>& ary, FifamTrArray<UChar> const& from_ary) {
                    for (UInt i = 0; i < FifamTranslation::NUM_TRANSLATIONS; i++) {
                        if (ary[i] == 0 && !from_ary[i] != 0)
                            ary[i] = from_ary[i];
                    }
                };

                UpdateTranslationsStr(c->mPlayerInText, oldc->mPlayerInText);
                UpdateTranslationsStr(c->mFanName1, oldc->mFanName1);
                UpdateTranslationsStr(c->mFanName2, oldc->mFanName2);
                UpdateTranslationsStr(c->mTermForTeam1, oldc->mTermForTeam1);
                UpdateTranslationsStr(c->mTermForTeam2, oldc->mTermForTeam2);
                UpdateTranslationsArticle(c->mAbbreviationArticle, oldc->mAbbreviationArticle);
                UpdateTranslationsArticle(c->mPlayerInTextArticle, oldc->mPlayerInTextArticle);
                UpdateTranslationsArticle(c->mFanName1Article, oldc->mFanName1Article);
                UpdateTranslationsArticle(c->mClubNameUsageInPhrase, oldc->mClubNameUsageInPhrase);
                UpdateTranslationsArticle(c->mClubNameUsageInPhrase2, oldc->mClubNameUsageInPhrase2);
                UpdateTranslationsArticle(c->mFanName2Article, oldc->mFanName2Article);
                UpdateTranslationsArticle(c->mTermForTeam1Article, oldc->mTermForTeam1Article);
                UpdateTranslationsArticle(c->mTermForTeam2Article, oldc->mTermForTeam2Article);

                // get some data from original DB
                if (c->mFifaID != 0 && origClubByFifaID.contains(c->mFifaID)) {
                    auto origc = origClubByFifaID[c->mFifaID];
                    if (c->mAiStrategy == FifamClubAiStrategy::General)
                        c->mAiStrategy = oldc->mAiStrategy;
                    c->mLandscape = oldc->mLandscape;
                    c->mSettlement = oldc->mSettlement;
                    c->mMediaPressure = oldc->mMediaPressure;
                }
            }
        }
        // update players
        ///for (FifamPlayer *p : dbnew->mPlayers) {
        ///    auto it = oldPlayers.find(p->mEmpicsId);
        ///    if (it != oldPlayers.end()) {
        ///        auto oldp = (*it).second;
        ///        p->mAppearance = oldp->mAppearance;
        ///        p->mSpecialFace = oldp->mSpecialFace;
        ///    }
        ///}
        // write new db
        dbnew->Write(gameVersion, FifamDatabase::GetGameDbVersion(gameVersion), dbPathOut);
    }
};
