#pragma once
#include "FifamCountry.h"
#include <filesystem>

using namespace std::experimental::filesystem;

class FifamDatabase {
public:
    static const size_t NUM_COUNTRIES = 207;

    std::array<FifamCountry *, NUM_COUNTRIES> mCountries = {};

    std::vector<FifamClub *> mClubs;

    std::vector<FifamPlayer *> mPlayers;

    std::vector<FifamStaff *> mStaff;

    FifamDatabase() {}

    FifamDatabase(size_t gameId, const wchar_t *dbPath) {
        Read(gameId, dbPath);
    }

    unsigned int GetInternalGameCountryId(size_t gameId, FifamNation nationId) {
        if (gameId < 8) {
            if (nationId == FifamNation::Montenegro)
                return 0;
            if (nationId > FifamNation::Montenegro)
                return Utils::ToInt(nationId) - 1;
        }
        return Utils::ToInt(nationId);
    }

    void Read(size_t gameId, const wchar_t *dbPath) {
        Clear();
        for (size_t i = 0; i < NUM_COUNTRIES; i++) {
            std::wcout << L"Reading contry " << i << std::endl;
            FifamNation nationId = Utils::FromInt<FifamNation>(i + 1);
            unsigned int countryFileId = GetInternalGameCountryId(gameId, nationId);
            if (countryFileId != 0) {
                String countryDataPath;
                if (gameId < 11)
                    countryDataPath = Utils::Format(L"%s\\Country%d.sav", dbPath, countryFileId);
                else
                    countryDataPath = Utils::Format(L"%s\\data\\CountryData%d.sav", dbPath, countryFileId);
                FifamReader reader(countryDataPath, gameId);
                if (reader.Available()) {
                    FifamCountry * country = new FifamCountry();
                    mCountries[i] = new FifamCountry();
                    country->Read(reader);
                    reader.Close();
                    if (gameId >= 11) {
                        FifamReader fixturesReader(Utils::Format(L"%s\\data\\CountryFixtures%d.sav", dbPath, countryFileId), gameId);
                        if (fixturesReader.Available()) {
                            country->ReadFixtures(fixturesReader);
                            fixturesReader.Close();
                        }
                        FifamReader scriptReader(Utils::Format(L"%s\\data\\CountryScript%d.sav", dbPath, countryFileId), gameId);
                        if (scriptReader.Available()) {
                            country->ReadFixtures(scriptReader);
                            scriptReader.Close();
                        }
                    }
                    
                }
            }
        }
    }

    void Clear() {
        for (size_t i = 0; i < NUM_COUNTRIES; i++) {
            if (mCountries[i]) {
                delete mCountries[i];
                mCountries[i] = nullptr;
            }
        }
        mClubs.clear();
        mPlayers.clear();
        mStaff.clear();
    }

    ~FifamDatabase() {
        Clear();
    }
};
