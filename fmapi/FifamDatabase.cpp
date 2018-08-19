#include "FifamDatabase.h"
#include "FifamUtils.h"
#include "FifamReadWrite.h"
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
        std::wcout << L"Reading country " << country->mId << L" (" << FifamTr(country->mName) << L")" << std::endl;
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

    FifamReader rulesReader(dbPath / L"Rules.sav", gameId, unicode);
    if (rulesReader.Available())
        mRules.Read(rulesReader, this);
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

    // Resolve competition, club, player links

    std::wcout << L"Resolving club links" << std::endl;
    for (FifamClub *club : mClubs)
        ResolveClubLinks(club, gameId);
    std::wcout << L"Resolving national team links" << std::endl;
    for (UChar i = 0; i < NUM_COUNTRIES; i++) {
        if (mCountries[i])
            ResolveClubLinks(&mCountries[i]->mNationalTeam, gameId);
    }
    std::wcout << L"Resolving player links" << std::endl;
    for (FifamPlayer *player : mPlayers)
        ResolvePlayerLinks(player, gameId);
    std::wcout << L"Resolving rules links" << std::endl;
    for (UInt i = 0; i < FifamContinent::NUM_CONTINENTS; i++) {
        mRules.mContinentalCupChampions[i].mFirstCup =
            ClubFromID(TranslateClubID(FifamUtils::GetSavedClubIDFromClubLink(mRules.mContinentalCupChampions[i].mFirstCup),
                gameId, LATEST_GAME_VERSION));
        mRules.mContinentalCupChampions[i].mSecondCup =
            ClubFromID(TranslateClubID(FifamUtils::GetSavedClubIDFromClubLink(mRules.mContinentalCupChampions[i].mSecondCup),
                gameId, LATEST_GAME_VERSION));
        mRules.mContinentalCupChampions[i].mSuperCup =
            ClubFromID(TranslateClubID(FifamUtils::GetSavedClubIDFromClubLink(mRules.mContinentalCupChampions[i].mSuperCup),
                gameId, LATEST_GAME_VERSION));
        mRules.mContinentalCupStadiums[i].mFirstCup =
            ClubFromID(TranslateClubID(FifamUtils::GetSavedClubIDFromClubLink(mRules.mContinentalCupStadiums[i].mFirstCup),
                gameId, LATEST_GAME_VERSION));
        mRules.mContinentalCupStadiums[i].mSecondCup =
            ClubFromID(TranslateClubID(FifamUtils::GetSavedClubIDFromClubLink(mRules.mContinentalCupStadiums[i].mSecondCup),
                gameId, LATEST_GAME_VERSION));
        mRules.mContinentalCupStadiums[i].mSuperCup =
            ClubFromID(TranslateClubID(FifamUtils::GetSavedClubIDFromClubLink(mRules.mContinentalCupStadiums[i].mSuperCup),
                gameId, LATEST_GAME_VERSION));
    }
    for (UInt i = 0; i < 3; i++) {
        mRules.mFairnessAwardWinners[i] = ClubFromID(TranslateClubID(FifamUtils::GetSavedClubIDFromClubLink(
            mRules.mFairnessAwardWinners[i]), gameId, LATEST_GAME_VERSION));
    }
    mRules.Unknown._1 = ClubFromID(TranslateClubID(FifamUtils::GetSavedClubIDFromClubLink(mRules.Unknown._1),
        gameId, LATEST_GAME_VERSION));
    mRules.Unknown._2 = ClubFromID(TranslateClubID(FifamUtils::GetSavedClubIDFromClubLink(mRules.Unknown._2),
        gameId, LATEST_GAME_VERSION));
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
                writer.WriteLineTranslationArray(country->mName, false);
                writer.WriteLineTranslationArray(country->mAbbr, false);
                writer.WriteLineTranslationArray(country->mNameGender);
                writer.WriteLine(country->mContinent);
                writer.WriteLine(country->mFirstLanguageForNames);
                writer.WriteLine(country->mSecondLanguageForNames);
                writer.WriteLine(country->mTerritory);
            }
        }
    }

    FifamWriter rulesWriter(dbPath / L"Rules.sav", gameId, vYear, vNumber, unicode);
    if (rulesWriter.Available())
        mRules.Write(rulesWriter, this);

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
    club->mDatabase = this;
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

void FifamDatabase::ResolveClubLinks(FifamClub *club, UInt gameId) {
    club->mUniqueID = TranslateClubID(club->mUniqueID, gameId, LATEST_GAME_VERSION);
    for (UInt i = 0; i < 4; i++)
        club->mLowestLeagues[i].Translate(club->mLowestLeagues[i].ToInt(), gameId, LATEST_GAME_VERSION);
    club->mPartnershipClub = ClubFromID(TranslateClubID(FifamUtils::GetSavedClubIDFromClubLink(club->mPartnershipClub), gameId, LATEST_GAME_VERSION));
    for (UInt i = 0; i < 4; i++)
        club->mRivalClubs[i] = ClubFromID(TranslateClubID(FifamUtils::GetSavedClubIDFromClubLink(club->mRivalClubs[i]), gameId, LATEST_GAME_VERSION));
    for (UInt i = 0; i < 3; i++)
        club->mCaptains[i] = PlayerFromID(FifamUtils::GetSavedPlayerIDFromPlayerPtr(club->mCaptains[i]));
}

void FifamDatabase::ResolvePlayerLinks(FifamPlayer *player, UInt gameId) {
    if (!player->mHistory.mEntries.empty()) {
        for (auto &entry : player->mHistory.mEntries)
            entry.mClub = ClubFromID(TranslateClubID(FifamUtils::GetSavedClubIDFromClubLink(entry.mClub), gameId, LATEST_GAME_VERSION));
    }
    player->mContract.mBuyBackClauseClub = 
        ClubFromID(TranslateClubID(FifamUtils::GetSavedClubIDFromClubLink(player->mContract.mBuyBackClauseClub), gameId, LATEST_GAME_VERSION));
}

FifamClubLink FifamDatabase::ClubFromID(UInt ID) {
    FifamClubLink result;
    if (ID != 0) {
        unsigned char countryId = (ID >> 16) & 0xFF;
        if (countryId > 0 && countryId <= NUM_COUNTRIES && mCountries[countryId - 1]) {
            result.mTeamType.SetFromInt(ID >> 24);
            unsigned short index = ID & 0xFFFF;
            if (index == 0xFFFF)
                result.mPtr = &mCountries[countryId - 1]->mNationalTeam;
            if (index > 0 && index <= mCountries[countryId - 1]->mClubs.size())
                result.mPtr = mCountries[countryId - 1]->mClubs[index - 1];
        }
    }
    return result;
}

FifamPlayer *FifamDatabase::PlayerFromID(UInt ID) {
    if (ID != 0) {
        auto it = mPersonsMap.find(ID);
        if (it != mPersonsMap.end()) {
            if (it->second->mPersonType == FifamPersonType::Player)
                return reinterpret_cast<FifamPlayer *>(it->second);;
        }
    }
    return nullptr;
}

UInt FifamDatabase::ClubToID(FifamClubLink const &link) {
    if (link.IsValid() && link.mPtr->mCountry) {
        FifamCountry *country = link.mPtr->mCountry;
        if (link.mPtr == &country->mNationalTeam)
            return 0xFFFF | (country->mId << 16) | (link.mTeamType.ToInt() << 24);
        else {
            auto it = std::find(country->mClubs.begin(), country->mClubs.end(), link.mPtr);
            if (it != country->mClubs.end()) {
                auto index = std::distance(country->mClubs.begin(), it);
                return (index + 1) | (country->mId << 16) | (link.mTeamType.ToInt() << 24);
            }
        }
    }
    return 0;
}

UInt FifamDatabase::PlayerToID(FifamPlayer const *player) {
    return player->mID;
}

UInt FifamDatabase::TranslateClubID(UInt ID, UInt gameFrom, UInt gameTo) {
    if (gameFrom > 8 && gameTo > 8)
        return ID;
    UChar region = (ID >> 16) & 0xFF;
    if (FifamUtils::ConvertRegion(region, gameFrom, gameTo))
        return (ID & 0xFF00FFFF) | (region << 16);
    return 0;
}
