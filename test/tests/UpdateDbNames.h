#pragma once
#include "FifamDbEnvironment.h"

class UpdateDbNames {
public:
    UpdateDbNames() {
        UInt const gameVersion = 13;
        Path dbPathNew = L"E:\\Games\\FIFA Manager 13\\database_new";
        Path dbPathOut = L"E:\\Games\\FIFA Manager 13\\database_NEW_CORR";
        Path dbPathOld = L"E:\\Games\\FIFA Manager 13\\database_old";

        FifamDatabase::mReadingOptions.mUseCustomFormations = true;
        FifamDatabase *dbnew = new FifamDatabase(gameVersion, dbPathNew);
        FifamDatabase *dbold = new FifamDatabase(gameVersion, dbPathOld);

        Map<UInt, FifamPlayer *> oldPlayers;

        for (FifamPlayer *p : dbold->mPlayers) {
            if (p->mEmpicsId)
                oldPlayers[p->mEmpicsId] = p;
        }
        // update clubs
        for (FifamClub *c : dbnew->mClubs) {
            auto oldc = dbold->GetClubFromUID(c->mUniqueID);
            if (oldc) {
                c->mClubColour = oldc->mClubColour;
                c->mClubColour2 = oldc->mClubColour2;
                c->mMerchandiseColour = oldc->mMerchandiseColour;
                c->mHeaderColour = oldc->mHeaderColour;
                c->mBackgroundColour = oldc->mBackgroundColour;
                c->mAbbreviation = oldc->mAbbreviation;
                c->mBadge = oldc->mBadge;
                c->mName = oldc->mName;
                c->mName2 = oldc->mName2;
                c->mShortName = oldc->mShortName;
                c->mShortName2 = oldc->mShortName2;
                c->mStadiumName = oldc->mStadiumName;
                c->mStadiumSeatsCapacity = oldc->mStadiumSeatsCapacity;
                c->mStadiumStandingsCapacity = oldc->mStadiumStandingsCapacity;
                c->mStadiumVipCapacity = oldc->mStadiumVipCapacity;
                c->mKit = oldc->mKit;
                // update club budget

            }
        }
        // update players
        for (FifamPlayer *p : dbnew->mPlayers) {
            auto it = oldPlayers.find(p->mEmpicsId);
            if (it != oldPlayers.end()) {
                auto oldp = (*it).second;
                p->mAppearance = oldp->mAppearance;
                p->mSpecialFace = oldp->mSpecialFace;
            }
        }
        // write new db
        dbnew->Write(gameVersion, FifamDatabase::GetGameDbVersion(gameVersion), dbPathOut);
    }
};
