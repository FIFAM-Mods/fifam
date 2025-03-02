#pragma once
#include "FifamDbEnvironment.h"

class CopyTeamColors {
public:
    static void Copy(FifamClub *to, FifamClub *from) {
        to->mClubColour = from->mClubColour;
        to->mClubColour2 = from->mClubColour2;
        to->mMerchandiseColour = from->mMerchandiseColour;
        to->mHeaderColour = from->mHeaderColour;
        to->mBackgroundColour = from->mBackgroundColour;
        to->mBadge = from->mBadge;
    }

    CopyTeamColors() {
        FifamDatabase::mReadingOptions.mUseCustomFormations = true;
        FifamDatabase::mReadingOptions.mReadPersons = false;
        FifamDatabase db_colors(13, "db_colors");
        FifamDatabase::mReadingOptions.mUseCustomFormations = true;
        FifamDatabase::mReadingOptions.mReadPersons = true;
        FifamDatabase db(13, "db");
        for (auto club : db.mClubs) {
            auto clubWithColors = db_colors.GetClubFromUID(club->mUniqueID);
            if (clubWithColors)
                Copy(club, clubWithColors);
        }
        for (unsigned int c = 1; c <= FifamDatabase::NUM_COUNTRIES; c++) {
            FifamCountry *to = db.GetCountry(c);
            FifamCountry *from = db_colors.GetCountry(c);
            if (to && from)
                Copy(&to->mNationalTeam, &from->mNationalTeam);
        }
        db.mWritingOptions.mWriteFixtures = false;
        db.mWritingOptions.mWriteExternalScripts = false;
        db.Write(13, FifamDatabase::GetGameDbVersion(13), "output\\database");
    }
};
