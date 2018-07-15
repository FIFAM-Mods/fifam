#include "FifamDatabase.h"
#include <iostream>

FifamDatabase::FifamDatabase() {}

FifamDatabase::FifamDatabase(size_t gameId, const wchar_t *dbPath) {
    Read(gameId, dbPath);
}

unsigned int FifamDatabase::GetInternalGameCountryId(size_t gameId, FifamNation nationId) {
    if (gameId < 8) {
        if (nationId == FifamNation::Montenegro)
            return 0;
        if (nationId > FifamNation::Montenegro)
            return Utils::ToInt(nationId) - 1;
    }
    return Utils::ToInt(nationId);
}

bool FifamDatabase::IsCountryPresent(size_t gameId, FifamNation nationId) {
    return GetInternalGameCountryId(gameId, nationId) != 0;
}

void FifamDatabase::Read(size_t gameId, Path const &dbPath) {
    Clear();

    path gamePath = dbPath.parent_path();
    path scriptPath = gamePath / L"script";
    if (!exists(scriptPath))
        scriptPath.clear();
    bool unicode = gameId >= 8;

    FifamReader countriesReader(dbPath / L"Countries.sav", gameId, unicode);
    if (countriesReader.Available()) {
        auto &reader = countriesReader;
        auto firstLine = reader.GetFullLine();
        if (Utils::StartsWith(firstLine, L"Countries Version: ")) {
            auto verParams = Utils::Split(firstLine, L" ");
            if (verParams.size() >= 3) {
                UInt countriesVer = Utils::ToNumber(verParams[2]);
                if (countriesVer == 2 || countriesVer == 3) {
                    for (UInt i = 0; i < NUM_COUNTRIES; i++) {
                        mCountries[i] = new FifamCountry(i + 1, this);
                        auto &country = mCountries[i];
                        if (IsCountryPresent(gameId, Utils::FromInt<FifamNation>(country->mId))) {
                            reader.ReadLineTranslationArray(country->mName);
                            reader.ReadLineTranslationArray(country->mAbbr);
                            reader.ReadLineTranslationArray(country->mUseTheForName);
                            reader.ReadLine(country->mAssociation);
                            reader.ReadLine(country->mFirstLanguageForNames);
                            reader.ReadLine(country->mSecondLanguageForNames);
                            reader.ReadLine(country->mTerritory);
                        }
                    }
                }
            }
        }
    }

    for (size_t i = 0; i < NUM_COUNTRIES; i++) {
        auto &country = mCountries[i];
        if (!country)
            continue;
        std::wcout << L"Reading contry " << i << L" (" << Tr(country->mName) << L")" << std::endl;
        FifamNation nationId = Utils::FromInt<FifamNation>(i + 1);
        unsigned int countryFileId = GetInternalGameCountryId(gameId, nationId);
        if (countryFileId != 0) {
            Path countryDataPath;
            if (gameId < 11)
                countryDataPath = dbPath / Utils::Format(L"Country%d.sav", countryFileId);
            else
                countryDataPath = dbPath / L"data" / Utils::Format(L"CountryData%d.sav", countryFileId);
            FifamReader reader(countryDataPath, gameId, unicode);
            if (reader.Available()) {
                country->mHasCountryData = true;
                country->Read(reader);
                reader.Close();
                if (gameId >= 11) {
                    FifamReader fixturesReader(dbPath / L"data" / Utils::Format(L"CountryFixtures%d.sav", countryFileId), gameId, unicode);
                    if (fixturesReader.Available()) {
                        country->ReadFixtures(fixturesReader);
                        fixturesReader.Close();
                    }
                    FifamReader scriptReader(dbPath / L"data" / Utils::Format(L"CountryScript%d.sav", countryFileId), gameId, unicode);
                    if (scriptReader.Available()) {
                        country->ReadFixtures(scriptReader);
                        scriptReader.Close();
                    }
                }

            }
        }
    }
    if (!scriptPath.empty()) {
        path cupAllocPath = scriptPath / L"cupAlloc.txt";
        FifamReader cupAllocReader(scriptPath / L"cupAlloc.txt", 0, false);
        if (cupAllocReader.Available()) {
            auto numCups = cupAllocReader.ReadLine<UInt>();
            for (UInt i = 0; i < numCups; i++) {
                if (cupAllocReader.ReadStartIndex(Utils::Format(L"CUP%d", i))) {
                    mCupTemplates.push_back(new FifamCupAlloc);
                    mCupTemplates.back()->Read(cupAllocReader);
                    cupAllocReader.ReadEndIndex(Utils::Format(L"CUP%d", i));
                }
            }
        }
    }
}

void FifamDatabase::Write(size_t gameId, unsigned short vYear, unsigned short vNumber, Path const &dbPath) {
    if (!exists(dbPath))
        create_directories(dbPath);
    path gamePath = dbPath.parent_path();
    path scriptPath = gamePath / L"script";
    if (!exists(scriptPath))
        scriptPath.clear();
    bool unicode = gameId >= 8;

    FifamWriter countriesWriter(dbPath / L"Countries.sav", gameId, 0, 0, unicode);
    if (countriesWriter.Available()) {
        auto &writer = countriesWriter;
        UInt countriesVer = (gameId <= 7) ? 2 : 3;
        writer.WriteLine(Utils::Format(L"Countries Version: %d", countriesVer));
        for (UInt i = 0; i < NUM_COUNTRIES; i++) {
            auto &country = mCountries[i];
            if (country && IsCountryPresent(gameId, Utils::FromInt<FifamNation>(country->mId))) {
                writer.WriteLineTranslationArray(country->mName);
                writer.WriteLineTranslationArray(country->mAbbr);
                writer.WriteLineTranslationArray(country->mUseTheForName);
                writer.WriteLine(country->mAssociation);
                writer.WriteLine(country->mFirstLanguageForNames);
                writer.WriteLine(country->mSecondLanguageForNames);
                writer.WriteLine(country->mTerritory);
            }
        }
    }

    if (!scriptPath.empty()) {
        path cupAllocPath = scriptPath / L"cupAlloc.txt";
        FifamWriter cupAllocWriter(scriptPath / L"cupAlloc.txt", gameId, 0, 0, false);
        if (cupAllocWriter.Available()) {
            cupAllocWriter.WriteLine(mCupTemplates.size());
            for (UInt i = 0; i < mCupTemplates.size(); i++) {
                cupAllocWriter.WriteStartIndex(Utils::Format(L"CUP%d", i));
                mCupTemplates[i]->Write(cupAllocWriter);
                cupAllocWriter.WriteEndIndex(Utils::Format(L"CUP%d", i));
            }
        }
    }
}

void FifamDatabase::Clear() {
    for (auto country : mCountries)
        delete country;
    mCountries = {};

    for (auto club : mClubs)
        delete club;
    mClubs.clear();

    for (auto player : mPlayers)
        delete player;
    mPlayers.clear();

    for (auto referee : mReferees)
        delete referee;
    mReferees.clear();

    for (auto staff : mStaffs)
        delete staff;
    mStaffs.clear();

    for (auto stadium : mStadiums)
        delete stadium;
    mStadiums.clear();

    for (auto sponsor : mSponsors)
        delete sponsor;
    mSponsors.clear();

    for (auto cupAlloc : mCupTemplates)
        delete cupAlloc;
    mCupTemplates.clear();
}

FifamDatabase::~FifamDatabase() {
    Clear();
}
