#pragma once
#include "FifamDbEnvironment.h"
#include "FifamReadWrite.h"
#include "FifamDatabase.h"
#include "FifamNames.h"

class CopyPortraitsFromPreviousPatch {
public:
    CopyPortraitsFromPreviousPatch() {
        FifamDatabase db(13, "D:\\Games\\FIFA Manager 22\\database");

        static Path personPath_old[] = {
            R"(D:\Projects\fifam\content\art_02\portraits\club\160x160)",
            R"(D:\Projects\fifam\content\art_03\portraits\club\160x160)",
            R"(D:\Projects\fifam\content\art_06\portraits\club\160x160)",
            R"(D:\Projects\fifam\content\art_07\portraits\club\160x160)",
            R"(D:\Projects\fifam\content\art_08\portraits\club\160x160)"
        };

        static Path refereePath_old[] = {
            R"(D:\Projects\fifam\content\art_02\portraits\Referees\160x160)"
        };

        static Path personPath_new[] = {
            R"(D:\Projects\fifam\content\fm13\art_02\portraits\club\160x160)",
            R"(D:\Projects\fifam\content\fm13\art_03\portraits\club\160x160)",
            R"(D:\Projects\fifam\content\fm13\art_06\portraits\club\160x160)",
            R"(D:\Projects\fifam\content\fm13\art_07\portraits\club\160x160)",
            R"(D:\Projects\fifam\content\fm13\art_08\portraits\club\160x160)"
        };

        static Path refereePath_new[] = {
            R"(D:\Projects\fifam\content\fm13\art_02\portraits\Referees\160x160)"
        };
		
		path outputBase = "output\\missing_portraits";
		path japan_players = outputBase / "japan_players";
		path japan_players_noempics = japan_players / "noempics";
		path japan_staff = outputBase / "japan_staff";
		path japan_referee = outputBase / "japan_referee";
		path other_players = outputBase / "other_players";
		path other_players_noempics = other_players / "noempics";
		path other_staff = outputBase / "other_staff";
		path other_referee = outputBase / "other_referee";
		create_directories(japan_players);
		create_directories(japan_players_noempics);
		create_directories(japan_staff);
		create_directories(japan_referee);
		create_directories(other_players);
		create_directories(other_players_noempics);
		create_directories(other_staff);
		create_directories(other_referee);
		
        auto ProcessPortrait = [&](String const &id, String const &uidNoEmpics, UInt type, bool japan) {
            if (!japan)
                return;
            if (type == 3) {
                bool exist = false;
                for (UInt i = 0; i < std::size(refereePath_new); i++) {
                    Path pp = refereePath_new[i] / (id + L".png");
                    if (exists(pp)) {
                        exist = true;
                        break;
                    }
                }
                if (!exist) {
                    Path outPath = japan ? japan_referee : other_referee;
                    for (UInt i = 0; i < std::size(refereePath_old); i++) {
                        Path pp = refereePath_old[i] / (id + L".png");
                        if (exists(pp)) {
                            copy(pp, outPath / pp.filename(), copy_options::overwrite_existing);
                            break;
                        }
                    }
                }
            }
            else if (type == 2) {
                bool exist = false;
                for (UInt i = 0; i < std::size(personPath_new); i++) {
                    Path pp = personPath_new[i] / (id + L".png");
                    if (exists(pp)) {
                        exist = true;
                        break;
                    }
                }
                if (!exist) {
                    Path outPath = japan ? japan_staff : other_staff;
                    for (UInt i = 0; i < std::size(personPath_old); i++) {
                        Path pp = personPath_old[i] / (id + L".png");
                        if (exists(pp)) {
                            copy(pp, outPath / pp.filename(), copy_options::overwrite_existing);
                            break;
                        }
                    }
                }
            }
            else if (type == 1) {
                bool exist = false;
                for (UInt i = 0; i < std::size(personPath_new); i++) {
                    Path pp = personPath_new[i] / (id + L".png");
                    if (exists(pp)) {
                        exist = true;
                        break;
                    }
                }
                if (!exist) {
                    if (uidNoEmpics.empty()) {
                        Path outPath = japan ? japan_players : other_players;
                        for (UInt i = 0; i < std::size(personPath_old); i++) {
                            Path pp = personPath_old[i] / (id + L".png");
                            if (exists(pp)) {
                                copy(pp, outPath / pp.filename(), copy_options::overwrite_existing);
                                break;
                            }
                        }
                    }
                    else {
                        bool hasEmpics = false;
                        Path outPath = japan ? japan_players : other_players;
                        for (UInt i = 0; i < std::size(personPath_old); i++) {
                            Path pp = personPath_old[i] / (id + L".png");
                            if (exists(pp)) {
                                copy(pp, outPath / pp.filename(), copy_options::overwrite_existing);
                                hasEmpics = true;
                                break;
                            }
                        }
                        if (!hasEmpics) {
                            Path outPath = japan ? japan_players_noempics : other_players_noempics;
                            for (UInt i = 0; i < std::size(personPath_old); i++) {
                                Path pp = personPath_old[i] / (uidNoEmpics + L".png");
                                if (exists(pp)) {
                                    copy(pp, outPath / pp.filename(), copy_options::overwrite_existing);
                                    break;
                                }
                            }
                        }
                    }
                }
            }
        };

        db.SetupWriteableStatus(13);
        
        for (auto p : db.mReferees)
            ProcessPortrait(FifamNames::GetPersonStringId(14, p->mFirstName, p->mLastName, String(), Date(), 0), String(), 3, p->mFootballManagerID >= 2002042000);
        for (auto p : db.mStaffs)
            ProcessPortrait(p->mWriteableStringID, String(), 2, p->mFootballManagerID >= 2002042000);
        for (auto p : db.mPlayers) {
            String uidNoEmpics;
            if (p->mFootballManagerID >= 0) {
                String empicsIdStr = Utils::Format(L"-%u", p->mFootballManagerID);
                if (p->mWriteableStringID.ends_with(empicsIdStr))
                    uidNoEmpics = p->mWriteableStringID.substr(0, p->mWriteableStringID.size() - empicsIdStr.size());
            }
            ProcessPortrait(p->mWriteableStringID, uidNoEmpics, 1, p->mFootballManagerID >= 2002042000);
        }
    }
};
