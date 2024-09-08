#pragma once
#include "FifamDbEnvironment.h"
#include "FifamClubBadge.h"
#include "Error.h"

class BadgesFM07toFIFA07 {
public:
    BadgesFM07toFIFA07() {
        Path badgesFolder = L"D:\\fm07_badges";
        Path outFolder = L"D:\\fifa07_badges";
        create_directories(outFolder);
        FifamDatabase::mReadingOptions.mReadPersons = false;
        FifamDatabase::mReadingOptions.mReadInternationalCompetitions = false;
        FifamDatabase::mReadingOptions.mReadInternationalCompetitions = false;
        FifamDatabase *db = GetEnvironment<FifamDbEnvironment<FM07, Default>>().GetDatabase();
        Set<UInt> usedIds;
        for (FifamClub *c : db->mClubs) {
            UInt fifaId = c->mFifaID;
            if (fifaId == 0)
                fifaId = c->mKit.mSpecialKitId;
            if (fifaId != 0) {
                if (Utils::Contains(usedIds, fifaId))
                    ::Error("Warning: ID %d was already used", fifaId);
                else {
                    auto badgeSubPath = Utils::ToLower(c->mBadge.GetBadgePath(7));
                    if (!Utils::StartsWith(badgeSubPath, L"color") && badgeSubPath.find(L"badge%d") != String::npos) {
                        Utils::Replace(badgeSubPath, L"badge%d", L"badge256");
                        Path badgePath = badgesFolder / badgeSubPath;
                        if (exists(badgePath))
                            copy(badgePath, outFolder / Utils::Format(L"%d.tga", fifaId));
                    }
                }
            }
        }
    }
};
