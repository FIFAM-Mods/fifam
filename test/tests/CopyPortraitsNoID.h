#pragma once
#include "FifamDbEnvironment.h"

class CopyPortraitsNoID {
public:
    CopyPortraitsNoID() {
        FifamDatabase::mReadingOptions.mUseCustomFormations = true;
        FifamDatabase db(13, "db");
        Vector<Path> fifamPortrats = {
           R"(D:\Projects\fifam\content\fm13\art_02\portraits\club\160x160)",
           R"(D:\Projects\fifam\content\fm13\art_03\portraits\club\160x160)",
           R"(D:\Projects\fifam\content\fm13\art_06\portraits\club\160x160)",
           R"(D:\Projects\fifam\content\fm13\art_07\portraits\club\160x160)",
           R"(D:\Projects\fifam\content\fm13\art_08\portraits\club\160x160)",
           R"(D:\Projects\fifam\content\fm13\art_09\portraits\club\160x160)",
           R"(C:\Users\Dmitri\Desktop\FM 23 update 1\_UPDATE_PORTRAITS_1\portraits\club\160x160)"
        };
        Vector<Path> fifamPortratsFMZ = {
           R"(C:\Users\Dmitri\Desktop\FM 23 update 1\_UPDATE_PORTRAITS_2\portraits\club\160x160)",
           R"(D:\Projects\fifam\content\fm13\portraits_fmz\art_02\portraits\club\160x160)",
           R"(D:\Projects\fifam\content\fm13\portraits_fmz\art_03\portraits\club\160x160)"
        };
        Path playerPortraitsPath = "portraits_noid\\club\\160x160";
        create_directories(playerPortraitsPath);
        db.SetupWriteableStatus(13);
        for (auto& [id, p] : db.mPersonsMap) {
            if (p->mFootballManagerID <= 0) {
                bool hasPortrait = false;
                for (auto const& i : fifamPortrats) {
                    if (exists(i / (p->mWriteableStringID + L".png"))) {
                        hasPortrait = true;
                        break;
                    }
                }
                if (!hasPortrait) {
                    for (auto const& i : fifamPortratsFMZ) {
                        Path fmzPath = i / (p->mWriteableStringID + L".png");
                        if (exists(fmzPath)) {
                            copy(fmzPath, playerPortraitsPath / (p->mWriteableStringID + L".png"), copy_options::overwrite_existing);
                            break;
                        }
                    }
                }
            }
        }
    }
};
