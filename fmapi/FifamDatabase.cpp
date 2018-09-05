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
    path historicPath = gamePath / L"fmdata" / L"historic";
    if (!exists(historicPath))
        historicPath.clear();
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
                else
                    Error(L"Inocrrect countries version");
            }
        }
        else {
            Error(L"%d", firstLine[1]);
        }
    }

    for (UChar i = 0; i < NUM_COUNTRIES ; i++) {
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
                    FifamReader fixturesReader(dbPath / L"fixture" / Utils::Format(L"CountryFixture%d.sav", countryFileId), gameId, unicode);
                    if (fixturesReader.Available()) {
                        country->ReadFixtures(fixturesReader);
                        fixturesReader.Close();
                    }
                    FifamReader scriptReader(dbPath / L"script" / Utils::Format(L"CountryScript%d.sav", countryFileId), gameId, unicode);
                    if (scriptReader.Available()) {
                        country->ReadScript(scriptReader);
                        scriptReader.Close();
                    }
                }
    
            }
        }
    }

    FifamReader withoutReader(dbPath / L"Without.sav", gameId, unicode);
    if (withoutReader.Available()) {
        if (withoutReader.GetGameId() >= 11)
            withoutReader.ReadVersion();
        if (withoutReader.ReadStartIndex(L"WITHOUT")) {
            UInt numPlayers = withoutReader.ReadLine<UInt>();
            UInt nextFreeId = GetNextFreePersonID();
            for (UInt i = 0; i < numPlayers; i++)
                CreatePlayer(nullptr, nextFreeId++)->Read(withoutReader, this);
            withoutReader.ReadEndIndex(L"WITHOUT");
        }
    }

    FifamReader rulesReader(dbPath / L"Rules.sav", gameId, unicode);
    if (rulesReader.Available())
        mRules.Read(rulesReader, this);

    if (!historicPath.empty())
        mHistoric.Read(historicPath, gameId, this);

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
        ResolveLinksForClub(club, gameId);
    std::wcout << L"Resolving national team links" << std::endl;
    for (UChar i = 0; i < NUM_COUNTRIES; i++) {
        if (mCountries[i])
            ResolveLinksForClub(&mCountries[i]->mNationalTeam, gameId);
    }
    std::wcout << L"Resolving player links" << std::endl;
    for (FifamPlayer *player : mPlayers)
        ResolveLinksForPlayer(player, gameId);
    std::wcout << L"Resolving staff links" << std::endl;
    for (FifamStaff *staff : mStaffs)
        ResolveLinksForStaff(staff, gameId);
    std::wcout << L"Resolving CAC player links" << std::endl;
    for (FifamCACPlayer *player : mCACPlayers)
        ResolveClubLink(player->mPreferredClub, gameId);
    std::wcout << L"Resolving rules links" << std::endl;
    for (UInt i = 0; i < FifamContinent::NUM_CONTINENTS; i++) {
        ResolveClubLink(mRules.mContinentalCupChampions[i].mFirstCup, gameId);
        ResolveClubLink(mRules.mContinentalCupChampions[i].mSecondCup, gameId);
        ResolveClubLink(mRules.mContinentalCupChampions[i].mSuperCup, gameId);
        ResolveClubLink(mRules.mContinentalCupStadiums[i].mFirstCup, gameId);
        ResolveClubLink(mRules.mContinentalCupStadiums[i].mSecondCup, gameId);
        ResolveClubLink(mRules.mContinentalCupStadiums[i].mSuperCup, gameId);
    }
    for (UInt i = 0; i < 3; i++)
        ResolveClubLink(mRules.mFairnessAwardWinners[i], gameId);
    ResolveClubLink(mRules.Unknown._1, gameId);
    ResolveClubLink(mRules.Unknown._2, gameId);
    std::wcout << L"Resolving historic links" << std::endl;
    for (auto &worldPlayer : mHistoric.mFifaWorldPlayers) {
        UInt clubUID = TranslateClubID(FifamUtils::GetSavedClubIDFromClubLink(worldPlayer.mClub), gameId, LATEST_GAME_VERSION);
        GetClubFromUID(worldPlayer.mClub, clubUID);
    }
    for (auto &worstStartingStreak : mHistoric.mWorstStartingStreaks) {
        UInt clubUID = TranslateClubID(FifamUtils::GetSavedClubIDFromClubLink(worstStartingStreak.mClub), gameId, LATEST_GAME_VERSION);
        GetClubFromUID(worstStartingStreak.mClub, clubUID);
        worstStartingStreak.mCompID.SetFromInt(
            FifamCompID::Translate(worstStartingStreak.mCompID.ToInt(), gameId, LATEST_GAME_VERSION));
    }
}

void FifamDatabase::Write(UInt gameId, UShort vYear, UShort vNumber, Path const &dbPath) {
    if (!exists(dbPath))
        create_directories(dbPath);
    path gamePath = dbPath.parent_path();
    path scriptPath = gamePath / L"script";
    if (!exists(scriptPath))
        scriptPath.clear();
    path historicPath = gamePath / L"fmdata" / L"historic";
    if (!exists(historicPath))
        historicPath.clear();
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

    for (UChar i = 0; i < NUM_COUNTRIES; i++) {
        auto &country = mCountries[i];
        if (!country)
            continue;
        std::wcout << L"Writing country " << country->mId << L" (" << FifamTr(country->mName) << L")" << std::endl;
        UChar countryFileId = GetInternalGameCountryId(gameId, i + 1);
        if (countryFileId != 0) {
            Path countryDataPath;
            if (gameId < 11)
                countryDataPath = dbPath / Utils::Format(L"Country%d.sav", countryFileId);
            else
                countryDataPath = dbPath / L"data" / Utils::Format(L"CountryData%d.sav", countryFileId);
            FifamWriter writer(countryDataPath, gameId, vYear, vNumber, unicode);
            if (writer.Available()) {
                country->Write(writer);
                writer.Close();
                if (gameId >= 11) {
                    FifamWriter fixturesWriter(dbPath / L"fixture" / Utils::Format(L"CountryFixture%d.sav", countryFileId), gameId, vYear, vNumber, unicode);
                    if (fixturesWriter.Available()) {
                        country->WriteFixtures(fixturesWriter);
                        fixturesWriter.Close();
                    }
                    FifamWriter scriptWriter(dbPath / L"script" / Utils::Format(L"CountryScript%d.sav", countryFileId), gameId, vYear, vNumber, unicode);
                    if (scriptWriter.Available()) {
                        country->WriteScript(scriptWriter);
                        scriptWriter.Close();
                    }
                }
            }
        }
    }

    FifamWriter withoutWriter(dbPath / L"Without.sav", gameId, vYear, vNumber, unicode);
    if (withoutWriter.Available()) {
        if (withoutWriter.GetGameId() >= 11)
            withoutWriter.WriteVersion();
        withoutWriter.WriteStartIndex(L"WITHOUT");
        Vector<FifamPlayer *> freeAgents;
        for (FifamPlayer *player : mPlayers) {
            if (!player->mClub)
                freeAgents.push_back(player);
        }
        withoutWriter.WriteLine(freeAgents.size());
        if (!freeAgents.empty()) {
            std::sort(freeAgents.begin(), freeAgents.end(), FifamPlayer::SortPlayersByLevel);
            for (FifamPlayer *player : freeAgents)
                player->Write(withoutWriter, this);
        }
        withoutWriter.WriteEndIndex(L"WITHOUT");
    }

    FifamWriter rulesWriter(dbPath / L"Rules.sav", gameId, vYear, vNumber, unicode);
    if (rulesWriter.Available())
        mRules.Write(rulesWriter, this);

    //if (!historicPath.empty())
    //    mHistoric.Write(historicPath, gameId, vYear, vNumber, this);

    if (!scriptPath.empty()) {
        path cupAllocPath = scriptPath / L"cupAlloc.txt";
        //FifamWriter cupAllocWriter(scriptPath / L"cupAlloc.txt", gameId, 0, 0, false);
        //if (cupAllocWriter.Available()) {
        //    cupAllocWriter.WriteLine(mCupTemplates.size() + 1);
        //    for (UInt i = 0; i < mCupTemplates.size(); i++) {
        //        cupAllocWriter.WriteStartIndex(Utils::Format(L"CUP%d", i + 1));
        //        mCupTemplates[i]->Write(cupAllocWriter);
        //        cupAllocWriter.WriteEndIndex(Utils::Format(L"CUP%d", i + 1));
        //    }
        //}
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
    if (club)
        club->mPlayers.push_back(player);
    player->mClub = club;
    return player;
}

FifamStaff *FifamDatabase::CreateStaff(FifamClub *club, UInt id) {
    FifamStaff *staff = new FifamStaff;
    staff->mID = id;
    mStaffs.push_back(staff);
    mPersonsMap[id] = staff;
    if (club)
        club->mStaffs.push_back(staff);
    staff->mClub = club;
    return staff;
}

void FifamDatabase::ResolveLinksForClub(FifamClub *club, UInt gameId) {
    club->mUniqueID = TranslateClubID(club->mUniqueID, gameId, LATEST_GAME_VERSION);
    for (UInt i = 0; i < 4; i++) {
        club->mLowestLeagues[i].SetFromInt(
            FifamCompID::Translate(club->mLowestLeagues[i].ToInt(), gameId, LATEST_GAME_VERSION));
    }
    ResolveClubLink(club->mPartnershipClub, gameId);
    for (UInt i = 0; i < 4; i++)
        ResolveClubLink(club->mRivalClubs[i], gameId);
    for (UInt i = 0; i < 3; i++)
        ResolvePlayerLink(club->mCaptains[i]);
}

void FifamDatabase::ResolveLinksForPlayer(FifamPlayer *player, UInt gameId) {
    for (auto &entry : player->mHistory.mEntries)
        ResolveClubLink(entry.mClub, gameId);
    ResolveClubLink(player->mContract.mBuyBackClauseClub, gameId);
    ResolveClubLink(player->mFirstClub, gameId);
    ResolveClubLink(player->mPreviousClub, gameId);
    ResolveClubLink(player->mFavouriteClub, gameId);
    ResolveClubLink(player->mWouldnSignFor, gameId);
    ResolvePlayerLink(player->mManagerFavouritePlayer);
    for (UInt i = 0; i < 3; i++)
        ResolveClubLink(player->mTransferRumors[i], gameId);
    ResolveClubLink(player->mStartingConditions.mLoan.mLoanedClub, gameId);
    ResolveClubLink(player->mStartingConditions.mFutureTransfer.mNewClub, gameId);
    ResolveClubLink(player->mStartingConditions.mFutureLoan.mLoanedClub, gameId);
    ResolveClubLink(player->mStartingConditions.mFutureReLoan.mLoanedClub, gameId);
}

void FifamDatabase::ResolveLinksForStaff(FifamStaff *staff, UInt gameId) {
    ResolveClubLink(staff->mFavouriteClub, gameId);
    ResolveClubLink(staff->mWouldNeverWorkForClub, gameId);
    ResolvePlayerLink(staff->mFavouritePlayer);
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

void FifamDatabase::ResolveClubLink(FifamClubLink &clubLink, UInt gameFrom, UInt gameTo) {
    clubLink = ClubFromID(TranslateClubID(FifamUtils::GetSavedClubIDFromClubLink(clubLink), gameFrom, gameTo));
}

void FifamDatabase::ResolvePlayerLink(FifamPlayer *&player) {
    player = PlayerFromID(FifamUtils::GetSavedPlayerIDFromPlayerPtr(player));
}

FifamClub *FifamDatabase::GetClubFromUID(UInt uid) {
    auto it = mClubsMap.find(uid);
    if (it != mClubsMap.end())
        return it->second;
    return nullptr;
}

void FifamDatabase::GetClubFromUID(FifamClubLink &link, UInt uid) {
    link.mTeamType = FifamClubTeamType::First;
    link.mPtr = GetClubFromUID(uid);
}

UInt FifamDatabase::GetNextFreePersonID() {
    if (mPersonsMap.empty())
        return 1;
    return (*mPersonsMap.rbegin()).first;
}
