#pragma once
#include "FifamDbEnvironment.h"
#include "FifamReadWrite.h"
#include "Error.h"

class ExportClubIDs {
public:
    ExportClubIDs() {
        FifamDatabase::mReadingOptions.mReadPersons = false;
        FifamDatabase::mReadingOptions.mReadInternationalCompetitions = false;
        FifamDatabase::mReadingOptions.mReadInternationalCompetitions = false;
        FifamDatabase *db = GetEnvironment<FifamDbEnvironment<FM13, Default>>().GetDatabase();
        FifamWriter writer(L"clubIDs.txt", 14, FifamVersion());
        writer.SetReplaceQuotes(false);
        if (writer.Available()) {
            writer.WriteLine(L"{| class=\"wikitable\"");
            for (auto country : db->mCountries) {
                if (country) {
                    writer.WriteLine(L"|-");
                    String countryName = FifamTr(country->mAbbr);
                    String countryAbbr;
                    if (!countryName.empty())
                        countryAbbr = countryName.substr(0, 1) + Utils::ToLower(countryName.substr(1));
                    writer.WriteLine(L"!colspan=\"2\"|[[File:" + countryAbbr + L".png|24px]] " +
                        FifamTr(country->mName) + L" (ID " + Utils::Format(L"%d", country->mId) + L")");

                    for (auto [clubId, club] : country->mClubsMap) {
                        writer.WriteLine(L"|-");
                        writer.WriteLine(L"|" + Utils::Format(L"%08X", clubId) + L"||" + FifamTr(club->mName));
                    }
                }
            }
            writer.WriteLine(L"|}");
        }
    }
};
