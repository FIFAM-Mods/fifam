#pragma once
#include "FifamDatabase.h"

class ReApplyBasqueStatus {
public:
    ReApplyBasqueStatus() {
        Set<Int> basquePlayerIDs;
        FifamReader r("fm_player_second_nations.csv");
        r.SkipLine();
        while (!r.IsEof()) {
            if (!r.EmptyLine()) {
                Int playerId = 1, nationId = -1;
                r.ReadLine(playerId, nationId);
                if (playerId != -1 && nationId == 114502)
                    basquePlayerIDs.insert(playerId);
            }
            else
                r.SkipLine();
        }
        FifamDatabase::mReadingOptions.mUseCustomFormations = true;
        FifamDatabase db(13, "db");
        for (auto &p : db.mPlayers) {
            if (p->mFootballManagerID >= 0 && Utils::Contains(basquePlayerIDs, p->mFootballManagerID))
                p->mIsBasque = true;
        }
        db.mWritingOptions.mWriteFixtures = false;
        db.mWritingOptions.mWriteExternalScripts = false;
        db.Write(13, FifamDatabase::GetGameDbVersion(13), "output\\database");
    }
};
