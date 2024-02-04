#pragma once
#include "FifamDbEnvironment.h"

class CopyPortraitsWC {
public:
    static Path FindPortrait(String stringID, Vector<Path> const &portraitPaths) {
        for (auto const &i : portraitPaths) {
            Path p = i / (stringID + L".png");
            if (exists(p))
                return p;
        }
        return Path();
    }

    CopyPortraitsWC() {
        FifamDatabase::mReadingOptions.mUseCustomFormations = true;
        FifamDatabase db(13, R"(D:\Games\FIFA Manager 22\database_wc)");
        Vector<Path> fifamPortrats = {
           R"(D:\Projects\fifam\content\fm13\art_02\portraits\club\160x160)",
           R"(D:\Projects\fifam\content\fm13\art_03\portraits\club\160x160)",
           R"(D:\Projects\fifam\content\fm13\art_06\portraits\club\160x160)",
           R"(D:\Projects\fifam\content\fm13\art_07\portraits\club\160x160)",
           R"(D:\Projects\fifam\content\fm13\art_08\portraits\club\160x160)",
           R"(D:\Projects\fifam\content\fm13\art_09\portraits\club\160x160)",

        };
        Vector<Path> fifamPortratsWC = {
           R"(H:\Downloads\portraits_23\art_02\portraits\club\160x160)",
           R"(H:\Downloads\portraits_23\art_03\portraits\club\160x160)",
           R"(H:\Downloads\portraits_23\art_06\portraits\club\160x160)",
           R"(H:\Downloads\portraits_23\art_07\portraits\club\160x160)",
           R"(H:\Downloads\portraits_23\art_08\portraits\club\160x160)",
           R"(H:\Downloads\portraits_23\art_09\portraits\club\160x160)",
        };
        Path outPath = "portraits_wc";
        create_directories(outPath);
        db.SetupWriteableStatus(13);
        for (auto& [id, p] : db.mPersonsMap) {
            if (FindPortrait(p->mWriteableStringID, fifamPortrats).empty()) {
                Bool hasPortrait = false;
                if (p->AsPlayer() && p->AsPlayer()->mEmpicsId != 0) {
                    String empicsStr = Utils::Format(L"-%d", p->AsPlayer()->mEmpicsId);
                    if (p->mWriteableStringID.ends_with(empicsStr)) {
                        String stringID = p->mWriteableStringID.substr(0, p->mWriteableStringID.size() - empicsStr.size());
                        hasPortrait = FindPortrait(stringID, fifamPortrats).empty() ? false : true;
                    }
                }
                if (!hasPortrait) {
                    Path wcPath = FindPortrait(p->mWriteableStringID, fifamPortratsWC);
                    if (!wcPath.empty())
                        copy(wcPath, outPath / wcPath.filename(), copy_options::overwrite_existing);
                }
            }
        }
    }
};
