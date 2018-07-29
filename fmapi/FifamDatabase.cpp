#include "FifamDatabase.h"
#include <iostream>

FifamDatabase::FifamDatabase() {}

FifamDatabase::FifamDatabase(UInt gameId, const Path &dbPath) {
    Read(gameId, dbPath);
}

unsigned int FifamDatabase::GetInternalGameCountryId(UInt gameId, UChar nationId) {
    FifamNation nation;
    nation.SetFromInt(nationId);
    if (gameId < 8) {
        if (nation == FifamNation::Montenegro)
            return 0;
        if (nation > FifamNation::Montenegro)
            return nation.ToInt() - 1;
    }
    return nation.ToInt();
}

bool FifamDatabase::IsCountryPresent(UInt gameId, UChar nationId) {
    return GetInternalGameCountryId(gameId, nationId) != 0;
}

void FifamDatabase::Read(UInt gameId, Path const &dbPath) {
    Clear();

    path gamePath = dbPath.parent_path();
    path scriptPath = gamePath / L"script";
    if (!exists(scriptPath))
        scriptPath.clear();
    Bool unicode = gameId >= 8;

    FifamReader countriesReader(dbPath / L"Countries.sav", gameId, unicode);
    if (countriesReader.Available()) {
        auto &reader = countriesReader;
        auto firstLine = reader.ReadFullLine();
        if (Utils::StartsWith(firstLine, L"Countries Version: ")) {
            auto verParams = Utils::Split(firstLine, ' ', false);
            if (verParams.size() >= 3) {
                UInt countriesVer = Utils::ToNumber(verParams[2]);
                if (countriesVer == 2 || countriesVer == 3) {
                    for (UInt i = 0; i < NUM_COUNTRIES; i++) {
                        mCountries[i] = new FifamCountry(i + 1, this);
                        auto &country = mCountries[i];
                        if (IsCountryPresent(gameId, country->mId)) {
                            reader.ReadLineTranslationArray(country->mName);
                            reader.ReadLineTranslationArray(country->mAbbr);
                            reader.ReadLineTranslationArray(country->mNameGender);
                            reader.ReadLine(country->mContinent);
                            reader.ReadLine(country->mFirstLanguageForNames);
                            reader.ReadLine(country->mSecondLanguageForNames);
                            reader.ReadLine(country->mTerritory);
                        }
                    }
                }
            }
        }
    }

    for (UChar i = 0; i < NUM_COUNTRIES; i++) {
        auto &country = mCountries[i];
        if (!country)
            continue;
        std::wcout << L"Reading contry " << country->mId << L" (" << Tr(country->mName) << L")" << std::endl;
        UChar countryFileId = GetInternalGameCountryId(gameId, i + 1);
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
            if (numCups > 0)
                numCups -= 1;
            for (UInt i = 0; i < numCups; i++) {
                if (cupAllocReader.ReadStartIndex(Utils::Format(L"CUP%d", i + 1))) {
                    mCupTemplates.push_back(new FifamCupAlloc);
                    mCupTemplates.back()->Read(cupAllocReader);
                    cupAllocReader.ReadEndIndex(Utils::Format(L"CUP%d", i + 1));
                }
            }
        }
    }
}

void FifamDatabase::Write(UInt gameId, UShort vYear, UShort vNumber, Path const &dbPath) {
    if (!exists(dbPath))
        create_directories(dbPath);
    path gamePath = dbPath.parent_path();
    path scriptPath = gamePath / L"script";
    if (!exists(scriptPath))
        scriptPath.clear();
    Bool unicode = gameId >= 8;

    FifamWriter countriesWriter(dbPath / L"Countries.sav", gameId, 0, 0, unicode);
    if (countriesWriter.Available()) {
        auto &writer = countriesWriter;
        UInt countriesVer = (gameId <= 7) ? 2 : 3;
        writer.WriteLine(Utils::Format(L"Countries Version: %d", countriesVer));
        for (UInt i = 0; i < NUM_COUNTRIES; i++) {
            auto &country = mCountries[i];
            if (country && IsCountryPresent(gameId, country->mId)) {
                writer.WriteLineTranslationArray(country->mName);
                writer.WriteLineTranslationArray(country->mAbbr);
                writer.WriteLineTranslationArray(country->mNameGender);
                writer.WriteLine(country->mContinent);
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
            cupAllocWriter.WriteLine(mCupTemplates.size() + 1);
            for (UInt i = 0; i < mCupTemplates.size(); i++) {
                cupAllocWriter.WriteStartIndex(Utils::Format(L"CUP%d", i + 1));
                mCupTemplates[i]->Write(cupAllocWriter);
                cupAllocWriter.WriteEndIndex(Utils::Format(L"CUP%d", i + 1));
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

    mClubsMap.clear();

    for (auto player : mPlayers)
        delete player;
    mPlayers.clear();

    for (auto referee : mReferees)
        delete referee;
    mReferees.clear();

    for (auto staff : mStaffs)
        delete staff;
    mStaffs.clear();

    mPersonsMap.clear();

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

FifamClub *FifamDatabase::CreateClub(FifamCountry *country) {
    FifamClub *club = new FifamClub;
    club->mCountry = country;
    mClubs.insert(club);
    country->mClubs.push_back(club);
    return club;
}

void FifamDatabase::AddClubToMap(FifamClub *club) {
    mClubsMap[club->mUniqueID] = club;
}

FifamPlayer *FifamDatabase::CreatePlayer(FifamClub *club, UInt id) {
    FifamPlayer *player = new FifamPlayer;
    player->mID = id;
    mPlayers.insert(player);
    mPersonsMap[id] = player;
    club->mPlayers.push_back(player);
    return player;
}

FifamStaff *FifamDatabase::CreateStaff(FifamClub *club, UInt id) {
    FifamStaff *staff = new FifamStaff;
    staff->mID = id;
    mStaffs.insert(staff);
    mPersonsMap[id] = staff;
    club->mStaffs.push_back(staff);
    return staff;
}
