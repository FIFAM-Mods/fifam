#pragma once
#include "FifamDatabase.h"

class ReApplyGeoCoords {
public:
    ReApplyGeoCoords() {
        FifamDatabase::mReadingOptions.mUseCustomFormations = true;
        FifamDatabase db_coords(13, "db_coords");
        FifamDatabase db(13, "db");
        for (auto club : db.mClubs) {
            auto fixedClub = db_coords.GetClubFromUID(club->mUniqueID);
            if (fixedClub)
                club->mGeoCoords = fixedClub->mGeoCoords;
        }
        for (UInt i = 0; i < FifamDatabase::NUM_COUNTRIES; i++)
            db.mCountries[i]->mNationalTeam.mGeoCoords = db_coords.mCountries[i]->mNationalTeam.mGeoCoords;
        db.mWritingOptions.mWriteFixtures = false;
        db.mWritingOptions.mWriteExternalScripts = false;
        db.Write(13, FifamDatabase::GetGameDbVersion(13), "output\\database");
    }
};
