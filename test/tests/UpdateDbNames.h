#pragma once
#include "FifamDbEnvironment.h"

class UpdateDbNames {
public:
    UpdateDbNames() {
        UInt const gameVersion = 13;
        Path dbPathIn = L"E:\\Games\\FIFA Manager 13\\database";
        Path dbPathOut = L"E:\\Games\\FIFA Manager 13\\database_NEW_CORR";
        Path dbPathRef = L"E:\\Games\\FIFA Manager 13\\database_names";

        FifamDatabase::mReadingOptions.mUseCustomFormations = true;
        FifamDatabase *db = new FifamDatabase(gameVersion, dbPathIn);
        FifamDatabase::mReadingOptions.mReadPersons = false;
        FifamDatabase *dbref = new FifamDatabase(gameVersion, dbPathRef);

        for (FifamClub *c : db->mClubs) {
            auto ref = dbref->GetClubFromUID(c->mUniqueID);
            if (ref) {
                c->mCityName = ref->mCityName;
                c->mTermForTeam1 = ref->mTermForTeam1;
                c->mTermForTeam2 = ref->mTermForTeam2;
                c->mFanName1 = ref->mFanName1;
                c->mFanName2 = ref->mFanName2;
                c->mStadiumName = ref->mStadiumName;
            }
            /*
            FifamTrSetAll(c->mName, FifamTr(c->mName));
            FifamTrSetAll(c->mName2, FifamTr(c->mName2));
            FifamTrSetAll(c->mShortName, FifamTr(c->mShortName));
            FifamTrSetAll(c->mShortName2, FifamTr(c->mShortName2));
            FifamTrSetAll(c->mAbbreviation, FifamTr(c->mAbbreviation));
            FifamTrSetAll(c->mTermForTeam1, FifamTr(c->mTermForTeam1));
            FifamTrSetAll(c->mTermForTeam2, FifamTr(c->mTermForTeam2));
            FifamTrSetAll(c->mFanName1, FifamTr(c->mFanName1));
            FifamTrSetAll(c->mFanName2, FifamTr(c->mFanName2));
            if (false) {
                FifamTrSetAll(c->mStadiumName, FifamTr(c->mStadiumName));
                FifamTrSetAll(c->mCityName, FifamTr(c->mCityName));
            }
            if (c->mTransfersCountry[0] == FifamNation::None)
                c->mTransfersCountry[0] = FifamNation::MakeFromInt(c->mCountry->mId);
            c->mSpecialSponsor = false;
            */
        }
        db->Write(gameVersion, FifamDatabase::GetGameDbVersion(gameVersion), dbPathOut);
    }
};
