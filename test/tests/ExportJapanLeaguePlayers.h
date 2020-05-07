#pragma once
#include "FifamDbEnvironment.h"
#include "FifamReadWrite.h"
#include "FifamDatabase.h"

class ExportJapanLeaguePlayers {
public:
    ExportJapanLeaguePlayers() {

        FifamDatabase *db = GetEnvironment<FifamDbEnvironment<FM13, Default>>().GetDatabase();
        /*
        FifamWriter writer(L"D:\\Projects\\fifam\\content\\JPlayers.txt", 14, FifamVersion());
        if (writer.Available()) {
            for (auto c : db->GetCountry(FifamNation::Japan)->mClubs) {
                for (auto p : c->mPlayers) {
                    writer.WriteLine(p->GetStringUniqueId(13, true) + L".png");
                    writer.WriteLine(p->GetStringUniqueId(13, false) + L".png");
                }
            }
        }*/

        static Path personPath[] = {
            R"(E:\Projects\fifam\content\fm13\art_02\portraits\club\160x160)",
            R"(E:\Projects\fifam\content\fm13\art_03\portraits\club\160x160)",
            R"(E:\Projects\fifam\content\fm13\art_06\portraits\club\160x160)",
            R"(E:\Projects\fifam\content\fm13\art_07\portraits\club\160x160)"
        };
        static Path refPath = R"(E:\Projects\fifam\content\fm13\art_05\portraits\Referees\160x160)";
        static Path fmPath = R"(E:\Documents\Sports Interactive\Football Manager 2020\graphics\Wonderkid Faces)";
        static Path jPath = R"(E:\Documents\Sports Interactive\Football Manager 2020\graphics\001_New Players)";

        auto DeletePortrait = [](String const &id, bool referee) {
            if (referee) {
                Path pp = refPath / (id + L".png");
                if (exists(pp))
                    remove(pp);
            }
            else {
                for (UInt i = 0; i < std::size(personPath); i++) {
                    Path pp = personPath[i] / (id + L".png");
                    if (exists(pp)) {
                        remove(pp);
                        break;
                    }
                }
            }
        };

        db->SetupWriteableStatus(14);
      //
      //  // delete referee and staff
      //  for (auto p : db->mReferees) {
      //      if (p->mCountry->mId == FifamNation::Japan)
      //          DeletePortrait(FifamNames::GetPersonStringId(14, p->mFirstName, p->mLastName, String(), Date(), 0), true);
      //  }
      //  for (auto c : db->GetCountry(FifamNation::Japan)->mClubs) {
      //      for (auto p : c->mStaffs)
      //          DeletePortrait(p->mWriteableStringID, false);
      //  }
        // delete players
        for (auto p : db->mPlayers) {
            if (p->mEmpicsId >= 1915386510) {
                //DeletePortrait(p->mWriteableStringID, false);
                //if (p->mEmpicsId >= 1915394345 && p->mEmpicsId <= 1915394608) {
                //    Path fmpp = fmPath / (std::to_string(p->mEmpicsId) + ".png");
                //    if (exists(fmpp))
                //        copy(fmpp, fmPath / (p->mWriteableStringID + L".png"));
                //}
                //else {
                //    Path fmpp = jPath / (std::to_string(p->mEmpicsId) + ".png");
                //    if (exists(fmpp))
                //        copy(fmpp, jPath / (p->mWriteableStringID + L".png"));
                //}
            }
        }
    }
};
