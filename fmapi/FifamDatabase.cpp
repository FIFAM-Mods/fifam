#include "FifamDatabase.h"
#include "FifamUtils.h"
#include "FifamReadWrite.h"
#include "FifamCompLeague.h"
#include "FifamCompPool.h"
#include "FifamCompRound.h"
#include "FifamCompCup.h"
#include "FifamCompRoot.h"
#include <iostream>

FifamDatabase::FifamDatabase() {}

FifamDatabase::FifamDatabase(UInt gameId, const Path &dbPath) {
    Read(gameId, dbPath);
}

UInt FifamDatabase::GetInternalGameCountryId(UInt gameId, UChar nationId) {
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

Bool FifamDatabase::IsCountryPresent(UInt gameId, UChar nationId) {
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

    ReadNamesFile(dbPath / L"MaleNames.txt", gameId, mMaleNames);
    ReadNamesFile(dbPath / L"FemaleNames.txt", gameId, mFemaleNames);
    ReadNamesFile(dbPath / L"Surnames.txt", gameId, mSurnames);

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
                    mCupTemplates.back()->Read(cupAllocReader, this);
                    cupAllocReader.ReadEndIndex(Utils::Format(L"CUP%d", i + 1));
                }
            }
        }
    }

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
                        FifamCountry *country = CreateCountry(i + 1);
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
            Error(L"Inocrrect countries file");
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
            else {
                FifamReader scriptReader(dbPath / L"script" / Utils::Format(L"CountryScript%d.sav", countryFileId), gameId, unicode);
                if (scriptReader.Available()) {
                    country->ReadScript(scriptReader);
                    scriptReader.Close();
                }
                countryDataPath = dbPath / L"data" / Utils::Format(L"CountryData%d.sav", countryFileId);
            }
            FifamReader reader(countryDataPath, gameId, unicode);
            if (reader.Available()) {
                country->mHasCountryData = true;
                country->Read(reader);
                reader.Close(); 
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
                CreatePlayer(nullptr, nextFreeId++)->Read(withoutReader);
            withoutReader.ReadEndIndex(L"WITHOUT");
        }
    }

    FifamReader rulesReader(dbPath / L"Rules.sav", gameId, unicode);
    if (rulesReader.Available())
        mRules.Read(rulesReader);

    if (!historicPath.empty())
        mHistoric.Read(historicPath, gameId);

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
    std::wcout << L"Resolving competition links" << std::endl;
    for (auto compEntry : mCompMap)
        ResolveLinksForCompetition(compEntry.second, gameId);
    std::wcout << L"Resolving rules links" << std::endl;
    for (UInt i = 0; i < FifamContinent::NUM_CONTINENTS; i++) {
        ResolveClubLink(mRules.mContinentalCupChampions[i].mFirstCup, gameId);
        ResolveClubLink(mRules.mContinentalCupChampions[i].mSecondCup, gameId);
        ResolveClubLink(mRules.mContinentalCupChampions[i].mSuperCup, gameId);
        ResolveClubLink(mRules.mContinentalCupStadiums[i].mFirstCup, gameId);
        ResolveClubLink(mRules.mContinentalCupStadiums[i].mSecondCup, gameId);
        ResolveClubLink(mRules.mContinentalCupStadiums[i].mSuperCup, gameId);
    }
    ResolveClubLinkList(mRules.mFairnessAwardWinners, gameId);
    ResolveClubLink(mRules.Unknown._1, gameId);
    ResolveClubLink(mRules.Unknown._2, gameId);
    std::wcout << L"Resolving historic links" << std::endl;
    for (auto &worldPlayer : mHistoric.mFifaWorldPlayers)
        ResolveClubUniqueID(worldPlayer.mClub, gameId);
    for (auto &worstStartingStreak : mHistoric.mWorstStartingStreaks) {
        ResolveClubUniqueID(worstStartingStreak.mClub, gameId);
        ResolveCompetitionPtr(worstStartingStreak.mCompetition, gameId);
    }
}

void FifamDatabase::Write(UInt gameId, UShort vYear, UShort vNumber, Path const &dbPath) {
    if (!exists(dbPath))
        create_directories(dbPath);
    if (gameId >= 11) {
        if (!exists(dbPath / L"data"))
            create_directories(dbPath / L"data");
        if (!exists(dbPath / L"fixture"))
            create_directories(dbPath / L"fixture");
        if (!exists(dbPath / L"script"))
            create_directories(dbPath / L"script");
    }
    path gamePath = dbPath.parent_path();
    path scriptPath = gamePath / L"script";
    if (!exists(scriptPath))
        scriptPath.clear();
    path historicPath = gamePath / L"fmdata" / L"historic";
    if (!exists(historicPath))
        historicPath.clear();
    Bool unicode = gameId >= 8;

    SetupWriteableStatus(gameId);

    WriteNamesFile(dbPath / L"MaleNames.txt", gameId, mMaleNames);
    WriteNamesFile(dbPath / L"FemaleNames.txt", gameId, mFemaleNames);
    WriteNamesFile(dbPath / L"Surnames.txt", gameId, mSurnames);

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
            if (!player->mClub) {
                if (player->GetIsWriteable() || mWritingOptions.mNonWriteablePlayersAreFreeAgents)
                    freeAgents.push_back(player);
            }
        }
        withoutWriter.WriteLine(freeAgents.size());
        if (!freeAgents.empty()) {
            std::sort(freeAgents.begin(), freeAgents.end(), FifamPlayer::SortPlayersByLevel);
            for (FifamPlayer *player : freeAgents)
                player->Write(withoutWriter);
        }
        withoutWriter.WriteEndIndex(L"WITHOUT");
    }

    FifamWriter rulesWriter(dbPath / L"Rules.sav", gameId, vYear, vNumber, unicode);
    if (rulesWriter.Available())
        mRules.Write(rulesWriter);

    //if (!historicPath.empty())
    //    mHistoric.Write(historicPath, gameId, vYear, vNumber, this);

    if (!scriptPath.empty()) {
        path cupAllocPath = scriptPath / L"cupAlloc.txt";
        //FifamWriter cupAllocWriter(scriptPath / L"cupAlloc.txt", gameId, 0, 0, false);
        //if (cupAllocWriter.Available()) {
        //    cupAllocWriter.WriteLine(mCupTemplates.size() + 1);
        //    for (UInt i = 0; i < mCupTemplates.size(); i++) {
        //        cupAllocWriter.WriteStartIndex(Utils::Format(L"CUP%d", i + 1));
        //        mCupTemplates[i]->Write(cupAllocWriter, this);
        //        cupAllocWriter.WriteEndIndex(Utils::Format(L"CUP%d", i + 1));
        //    }
        //}
    }
    ResetWriteableStatus();
}

void FifamDatabase::SetupWriteableStatus(UInt gameId) {
    for (auto country : mCountries) {
        if (country) {
            country->mNationalTeam.SetIsWriteable(true);
            country->mNationalTeam.SetWriteableID(0xFFFF | (country->mId << 16));
            country->mNationalTeam.SetWriteableUniqueID(TranslateClubID(country->mNationalTeam.mUniqueID, LATEST_GAME_VERSION, gameId));
        }
    }
    for (auto club : mClubs) {
        UInt id = 0;
        if (club->mCountry) {
            auto it = std::find(club->mCountry->mClubs.begin(), club->mCountry->mClubs.end(), club);
            if (it != club->mCountry->mClubs.end()) {
                auto index = std::distance(club->mCountry->mClubs.begin(), it);
                id = (index + 1) | (club->mCountry->mId << 16);
            }
        }
        club->SetIsWriteable(id != 0);
        club->SetWriteableID(id);
        club->SetWriteableUniqueID(TranslateClubID(club->mUniqueID, LATEST_GAME_VERSION, gameId));
    }
    for (auto personEntry : mPersonsMap) {
        personEntry.second->SetIsWriteable(true);
        personEntry.second->SetWriteableID(personEntry.second->mID);
        personEntry.second->SetWriteableUniqueID(personEntry.second->mID);
    }
    for (auto compEntry : mCompMap) {
        compEntry.second->SetIsWriteable(true);
        UInt compID = FifamUtils::GetCompIdForGameVersion(compEntry.first, gameId).ToInt();
        compEntry.second->SetIsWriteable(compID != 0);
        compEntry.second->SetWriteableID(compID);
        compEntry.second->SetWriteableUniqueID(compID);
    }
}

void FifamDatabase::ResetWriteableStatus() {
    for (auto club : mClubs)
        club->SetIsWriteable(true);
    for (auto personEntry : mPersonsMap)
        personEntry.second->SetIsWriteable(true);
    for (auto compEntry : mCompMap)
        compEntry.second->SetIsWriteable(true);
}

UInt FifamDatabase::GetClubsInCountryLimit(UInt gameId) {
    if (gameId >= 7 && gameId <= 14)
        return mWritingOptions.mMaxClubsInCountry[gameId - 7];
    return 0;
}

UInt FifamDatabase::GetPersonsInClubLimit(UInt gameId) {
    if (gameId >= 7 && gameId <= 14)
        return mWritingOptions.mMaxPersonsInClub[gameId - 7];
    return 0;
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

    for (auto player : mCACPlayers)
        delete player;
    mCACPlayers.clear();

    for (auto stadium : mStadiums)
        delete stadium;
    mStadiums.clear();

    for (auto sponsor : mSponsors)
        delete sponsor;
    mSponsors.clear();

    for (auto cupAlloc : mCupTemplates)
        delete cupAlloc;
    mCupTemplates.clear();

    for (auto comp : mCompMap)
        delete comp.second;
    mCompMap.clear();
}

FifamDatabase::~FifamDatabase() {
    Clear();
}

FifamCountry *FifamDatabase::CreateCountry(UInt id) {
    if (!mCountries[id - 1]) {
        mCountries[id - 1] = new FifamCountry(id, this);
        FifamCompID rootID = { (UChar)id, FifamCompType::Root, 0 };
        if (!GetCompetition(rootID))
            CreateCompetition(FifamCompDbType::Root, rootID);
    }
    return mCountries[id - 1];
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

FifamCompetition *FifamDatabase::CreateCompetition(FifamCompDbType dbType, FifamCompID const &compID, String const &name) {
    FifamCompetition *comp = nullptr;
    switch (dbType.ToInt()) {
    case FifamCompDbType::Root:
        comp = new FifamCompRoot();
        break;
    case FifamCompDbType::Pool:
        comp = new FifamCompPool();
        break;
    case FifamCompDbType::League:
        comp = new FifamCompLeague();
        break;
    case FifamCompDbType::Round:
        comp = new FifamCompRound();
        break;
    case FifamCompDbType::Cup:
        comp = new FifamCompCup();
        break;
    }
    if (comp) {
        auto it = mCompMap.find(compID);
        if (it != mCompMap.end()) {
            delete it->second;
            it->second = comp;
        }
        else
            mCompMap[compID] = comp;
        comp->mID = compID;
        comp->SetName(name);
    }
    return comp;
}

void FifamDatabase::ResolveLinksForClub(FifamClub *club, UInt gameId) {
    ResolveClubUniqueID(club->mUniqueID, gameId);
    ResolveCompetitionList(club->mLowestLeagues, gameId);
    ResolveClubLink(club->mPartnershipClub, gameId);
    ResolveClubLinkList(club->mRivalClubs, gameId);
    for (UInt i = 0; i < 3; i++)
        ResolvePlayerPtr(club->mCaptains[i]);
}

void FifamDatabase::ResolveLinksForPlayer(FifamPlayer *player, UInt gameId) {
    for (auto &entry : player->mHistory.mEntries)
        ResolveClubLink(entry.mClub, gameId);
    ResolveClubLink(player->mContract.mBuyBackClauseClub, gameId);
    ResolveClubLink(player->mFirstClub, gameId);
    ResolveClubLink(player->mPreviousClub, gameId);
    ResolveClubLink(player->mFavouriteClub, gameId);
    ResolveClubLink(player->mWouldnSignFor, gameId);
    ResolvePlayerPtr(player->mManagerFavouritePlayer);
    ResolveClubLinkList(player->mTransferRumors, gameId);
    ResolveClubLink(player->mStartingConditions.mLoan.mLoanedClub, gameId);
    ResolveClubLink(player->mStartingConditions.mFutureTransfer.mNewClub, gameId);
    ResolveClubLink(player->mStartingConditions.mFutureLoan.mLoanedClub, gameId);
    ResolveClubLink(player->mStartingConditions.mFutureReLoan.mLoanedClub, gameId);
}

void FifamDatabase::ResolveLinksForStaff(FifamStaff *staff, UInt gameId) {
    ResolveClubLink(staff->mFavouriteClub, gameId);
    ResolveClubLink(staff->mWouldNeverWorkForClub, gameId);
    ResolvePlayerPtr(staff->mFavouritePlayer);
}

void FifamDatabase::ResolveLinksForCompetition(FifamCompetition *comp, UInt gameId) {
    FifamUtils::TranslateCompIdToLatestVersion(comp->mID, gameId);
    ResolveCompetitionList(comp->mPredecessors, gameId);
    ResolveCompetitionList(comp->mSuccessors, gameId);
    comp->mInstructions.ForAllCompetitionLinks([=](FifamCompetition *& competition, UInt, FifamAbstractInstruction *instruction) {
        ResolveCompetitionPtr(competition, gameId);
    });
    if (comp->GetDbType() == FifamCompDbType::League)
        ResolveClubLinkList(comp->AsLeague()->mTeams, gameId);
    else if (comp->GetDbType() == FifamCompDbType::Pool)
        ResolveCompetitionList(comp->AsPool()->mCompConstraints, gameId);
}

FifamClubLink FifamDatabase::ClubFromID(UInt ID) {
    FifamClubLink result;
    if (ID != 0) {
        UChar countryId = (ID >> 16) & 0xFF;
        if (countryId > 0 && countryId <= NUM_COUNTRIES && mCountries[countryId - 1]) {
            result.mTeamType.SetFromInt(ID >> 24);
            UShort index = ID & 0xFFFF;
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

void FifamDatabase::ResolveClubUniqueID(FifamClubLink &clubLink, UInt gameFrom, UInt gameTo) {
    UInt clubUID = TranslateClubID(FifamUtils::GetSavedClubIDFromClubLink(clubLink), gameFrom, gameTo);
    GetClubFromUID(clubLink, clubUID);
}

void FifamDatabase::ResolveClubUniqueID(UInt &uid, UInt gameFrom, UInt gameTo) {
    uid = TranslateClubID(uid, gameFrom, gameTo);
}

void FifamDatabase::ResolveClubLink(FifamClubLink &clubLink, UInt gameFrom, UInt gameTo) {
    clubLink = ClubFromID(TranslateClubID(FifamUtils::GetSavedClubIDFromClubLink(clubLink), gameFrom, gameTo));
}

void FifamDatabase::ResolvePlayerPtr(FifamPlayer *&player) {
    player = PlayerFromID(FifamUtils::GetSavedPlayerIDFromPtr(player));
}

void FifamDatabase::ResolveCompetitionPtr(FifamCompetition *&comp, UInt gameFrom, UInt gameTo) {
    comp = GetCompetition(FifamUtils::TranslateCompId(FifamUtils::GetSavedCompetitionIDFromPtr(comp), gameFrom, gameTo));
}

void FifamDatabase::ResolveClubLinkList(Vector<FifamClubLink> &vec, UInt gameId, bool unique) {
    for (UInt i = 0; i < vec.size(); i++)
        ResolveClubLink(vec[i], gameId);
    Vector<FifamClubLink> oldVec = vec;
    vec.clear();
    for (UInt i = 0; i < oldVec.size(); i++) {
        if (oldVec[i].IsValid() && (!unique || !Utils::Contains(vec, oldVec[i])))
            vec.push_back(oldVec[i]);
    }
}

void FifamDatabase::ResolveCompetitionList(Vector<FifamCompetition *> &vec, UInt gameId, bool unique) {
    for (UInt i = 0; i < vec.size(); i++)
        ResolveCompetitionPtr(vec[i], gameId);
    Vector<FifamCompetition *> oldVec = vec;
    vec.clear();
    for (UInt i = 0; i < oldVec.size(); i++) {
        if (oldVec[i] && (!unique || !Utils::Contains(vec, oldVec[i])))
            vec.push_back(oldVec[i]);
    }
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

FifamCompetition *FifamDatabase::GetCompetition(FifamCompID const & compID) {
    auto it = mCompMap.find(compID);
    if (it != mCompMap.end())
        return it->second;
    return nullptr;
}

UInt FifamDatabase::GetNextFreePersonID() {
    if (mPersonsMap.empty())
        return 1;
    return (*mPersonsMap.rbegin()).first;
}

void FifamDatabase::ReadNamesFile(Path const &filepath, UInt gameId, NamesMap &outNames) {
    outNames.clear();
    Bool unicode = gameId >= 8;
    FifamReader reader(filepath, gameId, unicode);
    if (reader.Available()) {
        FifamLanguage currLanguage = FifamLanguage::None;
        while (!reader.IsEof()) {
            String line = reader.ReadFullLine();
            Utils::Trim(line);
            if (!line.empty()) {
                if (Utils::StartsWith(line, L"%"))
                    currLanguage.SetFromInt(Utils::ToNumber(line.substr(1)));
                else
                    outNames[currLanguage].insert(line);
            }
        }
    }
}

void FifamDatabase::WriteNamesFile(Path const &filepath, UInt gameId, NamesMap &names) {
    auto WriteNamesForLanguage = [](FifamWriter &writer, UInt langId, Bool &firstLine, NamesMap &names) {
        if (FifamLanguage::Present(langId)) {
            FifamLanguage currLanguage;
            currLanguage.SetFromInt(langId);
            if (names.count(currLanguage) > 0) {
                if (firstLine)
                    firstLine = false;
                else
                    writer.WriteNewLine();
                writer.Write(Utils::Format(L"%%%d %s", currLanguage.ToInt(), currLanguage.ToCStr()));
                for (auto const &name : names[currLanguage]) {
                    writer.WriteNewLine();
                    writer.Write(name);
                }
            }
        }
    };
    Bool unicode = gameId >= 8;
    FifamWriter writer(filepath, gameId, 0, 0, unicode);
    if (writer.Available()) {
        Bool firstLine = true;
        for (UInt i = 1; i <= 62; i++)
            WriteNamesForLanguage(writer, i, firstLine, names);
        WriteNamesForLanguage(writer, 0, firstLine, names);
        for (UInt i = 63; i <= 127; i++)
            WriteNamesForLanguage(writer, i, firstLine, names);
    }
}

FifamCompetition *FifamDatabase::ReadCompetition(FifamReader &reader, FifamNation nationId) {
    String compDbType = reader.ReadLine<String>();
    FifamCompID compID;
    compID.SetFromStr(reader.ReadFullLine(), FifamCompRegion::MakeFromInt(nationId.ToInt()));
    FifamCompetition *newComp = nullptr;
    if (compDbType == L"DB_LEAGUE")
        newComp = CreateCompetition(FifamCompDbType::League, compID);
    else if (compDbType == L"DB_POOL")
        newComp = CreateCompetition(FifamCompDbType::Pool, compID);
    else if (compDbType == L"DB_CUP")
        newComp = CreateCompetition(FifamCompDbType::Cup, compID);
    else if (compDbType == L"DB_ROUND")
        newComp = CreateCompetition(FifamCompDbType::Round, compID);
    if (newComp) {
        newComp->Read(reader, this, nationId);
    }
    return newComp;
}

void FifamDatabase::WriteCompetition(FifamWriter &writer, FifamCompetition *comp, FifamNation nationId) {
    writer.WriteLine(comp->GetDbType().ToStr());
    writer.WriteLine(comp->GetCompIDStr());
    comp->Write(writer, this, nationId);
}

FifamCupAlloc *FifamDatabase::GetCupTemplate(FifamCupSystemType cupSystemType) {
    if (cupSystemType.ToInt() > 0 && cupSystemType.ToInt() <= mCupTemplates.size())
        return mCupTemplates[cupSystemType.ToInt() - 1];
    return nullptr;
}
