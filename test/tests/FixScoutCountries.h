#pragma once
#include "FifamDbEnvironment.h"

class FixScoutCountries {
public:
    FixScoutCountries() {
        Map<UInt, Vector<FifamNation>> preferredCountries;
        {
            FifamDatabase::mReadingOptions.mUseCustomFormations = true;
            FifamDatabase db(13, "initial_db");
            for (auto s : db.mStaffs) {
                if (s->mFootballManagerID >= 0 && !s->mScoutPreferredCountries.empty())
                    preferredCountries[s->mFootballManagerID] = s->mScoutPreferredCountries;
            }
        }
        {
            FifamWriter log("log_scouts.txt");
            FifamDatabase::mReadingOptions.mUseCustomFormations = true;
            FifamDatabase db(13, "db");
            for (auto s : db.mStaffs) {
                if (s->mFootballManagerID >= 0 && !s->mScoutPreferredCountries.empty()) {
                    if (preferredCountries.contains(s->mFootballManagerID)) {
                        String line = s->GetName() + L": ";
                        Vector<String> countriesBefore;
                        for (auto const &i : s->mScoutPreferredCountries)
                            countriesBefore.push_back(i.ToStr());
                        line += L"{ " + Utils::Join(countriesBefore, L',') + L" } => ";
                        s->mScoutPreferredCountries = preferredCountries[s->mFootballManagerID];
                        Vector<String> countriesAfter;
                        for (auto const &i : s->mScoutPreferredCountries)
                            countriesAfter.push_back(i.ToStr());
                        line += L"{ " + Utils::Join(countriesAfter, L',') + L" }";
                        log.WriteLine(line);
                    }
                }
            }
            db.mWritingOptions.mWriteFixtures = false;
            db.mWritingOptions.mWriteExternalScripts = false;
            db.Write(13, FifamDatabase::GetGameDbVersion(13), "output\\database");
        }
    }
};
