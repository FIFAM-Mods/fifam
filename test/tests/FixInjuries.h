#pragma once
#include "FifamDbEnvironment.h"

class FixInjuries {
public:
    FixInjuries() {
        FifamWriter log("log_injuries.txt");
        FifamDatabase::mReadingOptions.mUseCustomFormations = true;
        FifamDatabase db1(13, "db_initial");
        Map<UInt, FifamPlayer *> initialPlayers;
        for (auto p : db1.mPlayers) {
            if (p->mFootballManagerID >= 0)
                initialPlayers[p->mFootballManagerID] = p;
        }
        FifamDatabase db2(13, "db");
        for (auto p : db2.mPlayers) {
            if (p->mFootballManagerID >= 0 && initialPlayers.contains(p->mFootballManagerID)) {
                if (!p->mStartingConditions.mInjury.Enabled()) {
                    auto &injury = initialPlayers[p->mFootballManagerID]->mStartingConditions.mInjury;
                    if (injury.Enabled()) {
                        //if (injury.mStartDate == FifamDate(30, 6, 2023)) {
                            p->mStartingConditions.mInjury.Setup(injury.mStartDate, injury.mEndDate, injury.mType);
                            log.WriteLine(Utils::Format(L"Re-added injury (%s, %s-%s) for player %s (%d)", injury.mType.ToStr(),
                                injury.mStartDate.ToString(), injury.mEndDate.ToString(), p->GetName(), p->mFootballManagerID));
                        //}
                    }
                }
                else {
                    auto &injury = initialPlayers[p->mFootballManagerID]->mStartingConditions.mInjury;
                    if (injury.Enabled() && (
                        p->mStartingConditions.mInjury.mStartDate != injury.mStartDate ||
                        p->mStartingConditions.mInjury.mEndDate != injury.mEndDate ||
                        p->mStartingConditions.mInjury.mType != injury.mType
                        )
                        )
                    {
                        String dateWasModified;
                        if (p->mStartingConditions.mInjury.mStartDate != injury.mStartDate ||
                            p->mStartingConditions.mInjury.mEndDate != injury.mEndDate
                            )
                        {
                            dateWasModified = L" - date was modified";
                        }
                        log.WriteLine(Utils::Format(L"Injury was changed (%s, %s-%s) for player %s (%d)%s",
                            injury.mType.ToStr(), injury.mStartDate.ToString(), injury.mEndDate.ToString(), p->GetName(),
                            p->mFootballManagerID, dateWasModified));
                    }
                }
            }
        }
        db2.mWritingOptions.mWriteFixtures = false;
        db2.mWritingOptions.mWriteExternalScripts = false;
        db2.Write(13, FifamDatabase::GetGameDbVersion(13), "output\\database");
    }
};
