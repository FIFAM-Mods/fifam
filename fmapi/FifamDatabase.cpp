#include "FifamDatabase.h"
#include "FifamUtils.h"
#include "FifamReadWrite.h"
#include "FifamCompLeague.h"
#include "FifamCompPool.h"
#include "FifamCompRound.h"
#include "FifamCompCup.h"
#include "FifamCompRoot.h"
#include <iostream>

FifamDatabase::ReadingOptions FifamDatabase::mReadingOptions;

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

    ReadNamesFile(dbPath / L"MaleNames.txt", gameId, mMaleNames);
    ReadNamesFile(dbPath / L"FemaleNames.txt", gameId, mFemaleNames);
    ReadNamesFile(dbPath / L"Surnames.txt", gameId, mSurnames);

    if (!scriptPath.empty()) {
        path cupAllocPath = scriptPath / L"cupAlloc.txt";
        FifamReader cupAllocReader(scriptPath / L"cupAlloc.txt", 0);
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

    if (!scriptPath.empty() && mReadingOptions.mReadInternationalCompetitions) {
        UInt gameIdForScripts = mReadingOptions.mGameVersionForScripts == 0 ? gameId : mReadingOptions.mGameVersionForScripts;
        if (gameIdForScripts >= 8) {
            ReadExternalScriptFile(scriptPath / L"Continental - Europe.txt", L"EURO", gameIdForScripts);
            ReadExternalScriptFile(scriptPath / L"Continental - South America.txt", L"SOUTHAM", gameIdForScripts);
            ReadExternalScriptFile(scriptPath / L"Continental - Africa.txt", L"INTAFRICA", gameIdForScripts);
            ReadExternalScriptFile(scriptPath / L"Continental - Asia.txt", L"INTASIA", gameIdForScripts);
            ReadExternalScriptFile(scriptPath / L"Continental - North America.txt", L"INTAMERICA", gameIdForScripts);
            ReadExternalScriptFile(scriptPath / L"Continental - Oceania.txt", L"INTOCEANIA", gameIdForScripts);
            ReadExternalScriptFile(scriptPath / L"WorldCupQualification.txt", L"QUALI_WC", gameIdForScripts);
            ReadExternalScriptFile(scriptPath / L"WorldCup.txt", L"WORLD_CUP", gameIdForScripts);
            ReadExternalScriptFile(scriptPath / L"EuropeanChampionshipQualification.txt", L"QUALI_EC", gameIdForScripts);
            ReadExternalScriptFile(scriptPath / L"EuropeanChampionship.txt", L"EURO_CUP", gameIdForScripts);
            ReadExternalScriptFile(scriptPath / L"WorldCupU20.txt", L"U20_WORLD_CUP", gameIdForScripts);
            ReadExternalScriptFile(scriptPath / L"ConfedCup.txt", L"CONFED_CUP", gameIdForScripts);
            if (gameIdForScripts >= 11) {
                ReadExternalScriptFile(scriptPath / L"CopaAmerica.txt", L"COPA_AMERICA", gameIdForScripts);

                ReadExternalScriptFile(scriptPath / L"Continental - Europe.txt", L"YOUTH", gameIdForScripts);
                ReadExternalScriptFile(scriptPath / L"Continental - South America.txt", L"YOUTH", gameIdForScripts);
                ReadExternalScriptFile(scriptPath / L"EuropeanChampionship.txt", L"EURO_NL", gameIdForScripts);
                ReadExternalScriptFile(scriptPath / L"EuropeanChampionship.txt", L"Q_EURO_NL", gameIdForScripts);
                ReadExternalScriptFile(scriptPath / L"AsiaCup.txt", L"ASIA_CUP", gameIdForScripts);
                ReadExternalScriptFile(scriptPath / L"AsiaCup.txt", L"Q_ASIA_CUP", gameIdForScripts);
                ReadExternalScriptFile(scriptPath / L"OFCCup.txt", L"OFC_CUP", gameIdForScripts);
                ReadExternalScriptFile(scriptPath / L"OFCCup.txt", L"Q_OFC_CUP", gameIdForScripts);
                ReadExternalScriptFile(scriptPath / L"NorthAmericaCup.txt", L"NAM_CUP", gameIdForScripts);
                ReadExternalScriptFile(scriptPath / L"NorthAmericaCup.txt", L"NAM_NL", gameIdForScripts);
                ReadExternalScriptFile(scriptPath / L"NorthAmericaCup.txt", L"Q_NAM_NL", gameIdForScripts);
                ReadExternalScriptFile(scriptPath / L"AfricaCup.txt", L"AFRICA_CUP", gameIdForScripts);
                ReadExternalScriptFile(scriptPath / L"AfricaCup.txt", L"Q_AFRICA_CUP", gameIdForScripts);
                ReadExternalScriptFile(scriptPath / L"WorldCupU17.txt", L"U17_WORLD_CUP", gameIdForScripts);
                ReadExternalScriptFile(scriptPath / L"EuropeanChampionshipU21.txt", L"U21_EC", gameIdForScripts);
                ReadExternalScriptFile(scriptPath / L"EuropeanChampionshipU19.txt", L"U19_EC", gameIdForScripts);
                ReadExternalScriptFile(scriptPath / L"EuropeanChampionshipU17.txt", L"U17_EC", gameIdForScripts);
                ReadExternalScriptFile(scriptPath / L"OlympicGames.txt", L"OLYMPIC", gameIdForScripts);
            }
        }
        else {
            ReadExternalScriptFile(scriptPath / L"EuropeanCup.txt", L"EURO", gameIdForScripts);
            ReadExternalScriptFile(scriptPath / L"SouthAmCup.txt", L"SOUTHAM", gameIdForScripts);
            ReadExternalScriptFile(scriptPath / L"IntAfrica.txt", L"INTAFRICA", gameIdForScripts);
            ReadExternalScriptFile(scriptPath / L"IntAsia.txt", L"INTASIA", gameIdForScripts);
            ReadExternalScriptFile(scriptPath / L"IntAmerica.txt", L"INTAMERICA", gameIdForScripts);
            ReadExternalScriptFile(scriptPath / L"IntOceania.txt", L"INTOCEANIA", gameIdForScripts);
            ReadExternalScriptFile(scriptPath / L"QualiWC.txt", L"QUALI_WC", gameIdForScripts);
            ReadExternalScriptFile(scriptPath / L"WC.txt", L"WORLD_CUP", gameIdForScripts);
            ReadExternalScriptFile(scriptPath / L"QualiEC.txt", L"QUALI_EC", gameIdForScripts);
            ReadExternalScriptFile(scriptPath / L"EC.txt", L"EURO_CUP", gameIdForScripts);
        }
    }

    FifamReader countriesReader(dbPath / L"Countries.sav", gameId, GetGameDbVersion(gameId));
    if (countriesReader.Available()) {
        auto &reader = countriesReader;
        UInt countriesVer = 0;
        if (gameId >= 7) {
            auto firstLine = reader.ReadFullLine();
            if (Utils::StartsWith(firstLine, L"Countries Version: ")) {
                auto verParams = Utils::Split(firstLine, ' ', false);
                if (verParams.size() >= 3)
                    countriesVer = Utils::ToNumber(verParams[2]);
            }
        }
        else
            countriesVer = 1;
        if (countriesVer >= 1 && countriesVer <= 3) {
            for (UInt i = 0; i < NUM_COUNTRIES; i++) {
                FifamCountry *country = CreateCountry(i + 1);
                if (IsCountryPresent(gameId, country->mId)) {
                    reader.ReadLineTranslationArray(country->mName);
                    if (gameId >= 4)
                        reader.ReadLineTranslationArray(country->mAbbr);
                    else
                        FifamTrSetAll(country->mAbbr, reader.ReadLine<String>());
                    if (countriesVer >= 2)
                        reader.ReadLineTranslationArray(country->mNameGender);
                    reader.ReadLine(country->mContinent);
                    reader.ReadLine(country->mFirstLanguageForNames);
                    reader.ReadLine(country->mSecondLanguageForNames);
                    reader.ReadLine(country->mTerritory);
                }
            }
        }
        else
            Error(L"Incorrect countries version");
    }

    if (mReadingOptions.mReadCountriesData) {
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
                    if (mReadingOptions.mReadCountryCompetitions) {
                        FifamReader scriptReader(dbPath / L"script" / Utils::Format(L"CountryScript%d.sav", countryFileId), gameId);
                        if (scriptReader.Available()) {
                            country->ReadScript(scriptReader);
                            scriptReader.Close();
                        }
                    }
                    countryDataPath = dbPath / L"data" / Utils::Format(L"CountryData%d.sav", countryFileId);
                }
                FifamReader reader(countryDataPath, gameId);
                if (reader.Available()) {
                    country->mHasCountryData = true;
                    country->Read(reader);
                    reader.Close();
                }
            }
        }
    }

    if (mReadingOptions.mReadPersons) {
        FifamReader withoutReader(dbPath / L"Without.sav", gameId);
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
            withoutReader.Close();
        }
        if (gameId >= 13) {
            Map<String, Vector<FifamPlayer *>> playerStrIDsMap;
            for (FifamPlayer *player : mPlayers)
                playerStrIDsMap[player->GetStringUniqueId(gameId, false)].push_back(player);
            ReadPlayerRelations(dbPath / L"PlayerRelations.sav", gameId, playerStrIDsMap);
        }
    }

    FifamReader assessmentReader(dbPath / L"Assessment.sav", gameId);
    if (assessmentReader.Available()) {
        assessmentReader.SkipLine();
        while (!assessmentReader.IsEof()) {
            if (!assessmentReader.EmptyLine()) {
                Array<Float, 6> data;
                UChar countryId = 0;
                String dummy;
                if (gameId > 7) {
                    assessmentReader.ReadLineWithSeparator(L'\t', countryId, dummy, dummy, dummy, dummy, dummy, dummy,
                        data[0], data[1], data[2], data[3], data[4], data[5]);
                }
                else {
                    assessmentReader.ReadLineWithSeparator(L'\t', countryId, dummy, dummy, dummy, dummy, dummy,
                        data[0], data[1], data[2], data[3], data[4], data[5]);
                }
                if (countryId > 0 && countryId <= 208 && mCountries[countryId - 1])
                    mCountries[countryId - 1]->mAssessmentData = data;
            }
            else
                assessmentReader.SkipLine();
        }
    }

    FifamReader rulesReader(dbPath / L"Rules.sav", gameId);
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
    ResolveStadiumPtr(mRules.mEuropeCup1Stadium, gameId);
    ResolveStadiumPtr(mRules.mEuropeCup2Stadium, gameId);
    ResolveStadiumPtr(mRules.mSouthAmericaCup1Stadium, gameId);
    std::wcout << L"Resolving historic links" << std::endl;
    for (auto &worldPlayer : mHistoric.mFifaWorldPlayers)
        ResolveClubUniqueID(worldPlayer.mClub, gameId);
    for (auto &worstStartingStreak : mHistoric.mWorstStartingStreaks) {
        ResolveClubUniqueID(worstStartingStreak.mClub, gameId);
        ResolveCompetitionID(worstStartingStreak.mCompetition, gameId);
    }
    std::wcout << L"Done with database reading" << std::endl;
}

void FifamDatabase::Write(UInt gameId, FifamVersion const &version, Path const &dbPath) {
    if (!exists(dbPath))
        create_directories(dbPath);
    if (gameId >= 11) {
        if (!exists(dbPath / L"data"))
            create_directories(dbPath / L"data");
        if (mWritingOptions.mWriteFixtures && !exists(dbPath / L"fixture"))
            create_directories(dbPath / L"fixture");
        if (!exists(dbPath / L"script"))
            create_directories(dbPath / L"script");
    }
    path gamePath = dbPath.parent_path();
    path scriptPath = gamePath / L"script_converted";
    if (!exists(scriptPath))
        create_directories(scriptPath);
    path historicPath = gamePath / L"fmdata" / L"historic";
    if (!exists(historicPath))
        historicPath.clear();
    Bool unicode = gameId >= 8;

    SetupWriteableStatus(gameId);

    //WriteNamesFile(dbPath / L"MaleNames.txt", gameId, mMaleNames);
    //WriteNamesFile(dbPath / L"FemaleNames.txt", gameId, mFemaleNames);
    //WriteNamesFile(dbPath / L"Surnames.txt", gameId, mSurnames);

    Vector<FifamCompEntry> compsEurope, compsSouthAmerica, compsNorthAmerica, compsAfrica, compsAsia, compsOceania,
        compsQualiWC, compsWC, compsQualiEC, compsEC, compsU20WC, compsConfedCup, compsCopaAmerica,
        compsEuropeYouth, compsSouthAmericaYouth, compsEuroNL, compsEuroNLQ, compsNorthAmericaCup, compsNorthAmericaNL,
        compsNorthAmericaNLQ, compsAfricaCup, compsAfricaCupQ, compsAsiaCup, compsAsiaCupQ, compsOFCCup, compsOFCCupQ,
        compsU17WC, compsU21EC, compsU19EC, compsU17EC, compsOlympicGames;
    for (auto const &compEntry : mCompMap) {
        FifamCompetition *comp = compEntry.second;
        if (FifamUtils::GetWriteableID(comp->mID, gameId)) {
            if (comp->GetDbType() == FifamCompDbType::League || comp->GetDbType() == FifamCompDbType::Cup ||
                comp->GetDbType() == FifamCompDbType::Round || comp->GetDbType() == FifamCompDbType::Pool)
            {
                if (comp->mID.mRegion == FifamCompRegion::Europe) {
                    if (comp->mID.mType == FifamCompType::YouthChampionsLeague)
                        compsEuropeYouth.push_back(compEntry);
                    else
                        compsEurope.push_back(compEntry);
                }
                else if (comp->mID.mRegion == FifamCompRegion::SouthAmerica) {
                    if (comp->mID.mType == FifamCompType::YouthChampionsLeague)
                        compsSouthAmericaYouth.push_back(compEntry);
                    else
                        compsSouthAmerica.push_back(compEntry);
                }
                else if (comp->mID.mRegion == FifamCompRegion::NorthAmerica)
                    compsNorthAmerica.push_back(compEntry);
                else if (comp->mID.mRegion == FifamCompRegion::Africa)
                    compsAfrica.push_back(compEntry);
                else if (comp->mID.mRegion == FifamCompRegion::Asia)
                    compsAsia.push_back(compEntry);
                else if (comp->mID.mRegion == FifamCompRegion::Oceania)
                    compsOceania.push_back(compEntry);
                else if (comp->mID.mRegion == FifamCompRegion::International) {
                    if (comp->mID.mType == FifamCompType::QualiWC)
                        compsQualiWC.push_back(compEntry);
                    else if (comp->mID.mType == FifamCompType::WorldCup)
                        compsWC.push_back(compEntry);
                    else if (comp->mID.mType == FifamCompType::QualiEC)
                        compsQualiEC.push_back(compEntry);
                    else if (comp->mID.mType == FifamCompType::EuroCup)
                        compsEC.push_back(compEntry);
                    else if (comp->mID.mType == FifamCompType::U20WorldCup || comp->mID.mType == FifamCompType::U20WCQ)
                        compsU20WC.push_back(compEntry);
                    else if (comp->mID.mType == FifamCompType::ConfedCup)
                        compsConfedCup.push_back(compEntry);
                    else if (comp->mID.mType == FifamCompType::CopaAmerica)
                        compsCopaAmerica.push_back(compEntry);

                    else if (comp->mID.mType == FifamCompType::EuroNL)
                        compsEuroNL.push_back(compEntry);
                    else if (comp->mID.mType == FifamCompType::EuroNLQ)
                        compsEuroNLQ.push_back(compEntry);
                    else if (comp->mID.mType == FifamCompType::NamCup)
                        compsNorthAmericaCup.push_back(compEntry);
                    else if (comp->mID.mType == FifamCompType::NamNL)
                        compsNorthAmericaNL.push_back(compEntry);
                    else if (comp->mID.mType == FifamCompType::NamNLQ)
                        compsNorthAmericaNLQ.push_back(compEntry);
                    else if (comp->mID.mType == FifamCompType::AfricaCup)
                        compsAfricaCup.push_back(compEntry);
                    else if (comp->mID.mType == FifamCompType::AfricaCupQ)
                        compsAfricaCupQ.push_back(compEntry);
                    else if (comp->mID.mType == FifamCompType::AsiaCup)
                        compsAsiaCup.push_back(compEntry);
                    else if (comp->mID.mType == FifamCompType::AsiaCupQ)
                        compsAsiaCupQ.push_back(compEntry);
                    else if (comp->mID.mType == FifamCompType::OfcCup)
                        compsOFCCup.push_back(compEntry);
                    else if (comp->mID.mType == FifamCompType::OfcCupQ)
                        compsOFCCupQ.push_back(compEntry);
                    else if (comp->mID.mType == FifamCompType::U17WC || comp->mID.mType == FifamCompType::U17WCQ)
                        compsU17WC.push_back(compEntry);
                    else if (comp->mID.mType == FifamCompType::U21EC || comp->mID.mType == FifamCompType::U21ECQ)
                        compsU21EC.push_back(compEntry);
                    else if (comp->mID.mType == FifamCompType::U19EC || comp->mID.mType == FifamCompType::U19ECQ)
                        compsU19EC.push_back(compEntry);
                    else if (comp->mID.mType == FifamCompType::U17EC || comp->mID.mType == FifamCompType::U17ECQ)
                        compsU17EC.push_back(compEntry);
                    else if (comp->mID.mType == FifamCompType::Olympic || comp->mID.mType == FifamCompType::OlympicQ)
                        compsOlympicGames.push_back(compEntry);
                }
            }
        }
    }

    if (gameId >= 8) {
        WriteExternalScriptFile(scriptPath / L"Continental - Europe.txt", L"EURO", gameId, compsEurope, 1);
        WriteExternalScriptFile(scriptPath / L"Continental - South America.txt", L"SOUTHAM", gameId, compsSouthAmerica, 0);
        WriteExternalScriptFile(scriptPath / L"Continental - Africa.txt", L"INTAFRICA", gameId, compsAfrica, 0);
        WriteExternalScriptFile(scriptPath / L"Continental - Asia.txt", L"INTASIA", gameId, compsAsia, 0);
        WriteExternalScriptFile(scriptPath / L"Continental - North America.txt", L"INTAMERICA", gameId, compsNorthAmerica, 0);
        WriteExternalScriptFile(scriptPath / L"Continental - Oceania.txt", L"INTOCEANIA", gameId, compsOceania, 0);
        WriteExternalScriptFile(scriptPath / L"WorldCupQualification.txt", L"QUALI_WC", gameId, compsQualiWC, 0);
        WriteExternalScriptFile(scriptPath / L"WorldCup.txt", L"WORLD_CUP", gameId, compsWC, 1);
        WriteExternalScriptFile(scriptPath / L"EuropeanChampionshipQualification.txt", L"QUALI_EC", gameId, compsQualiEC, 1);
        WriteExternalScriptFile(scriptPath / L"EuropeanChampionship.txt", L"EURO_CUP", gameId, compsEC, 1);
        WriteExternalScriptFile(scriptPath / L"WorldCupU20.txt", L"U20_WORLD_CUP", gameId, compsU20WC, 1);
        WriteExternalScriptFile(scriptPath / L"ConfedCup.txt", L"CONFED_CUP", gameId, compsConfedCup, 1);
        if (gameId >= 11) {
            WriteExternalScriptFile(scriptPath / L"CopaAmerica.txt", L"COPA_AMERICA", gameId, compsCopaAmerica, 1);

            /*
            Writing of custom competition types is not implemented
            */
        }
    }
    else {
        WriteExternalScriptFile(scriptPath / L"EuropeanCup.txt", L"EURO", gameId, compsEurope, 0);
        WriteExternalScriptFile(scriptPath / L"SouthAmCup.txt", L"SOUTHAM", gameId, compsSouthAmerica, 0);
        WriteExternalScriptFile(scriptPath / L"IntAfrica.txt", L"INTAFRICA", gameId, compsAfrica, 0);
        WriteExternalScriptFile(scriptPath / L"IntAsia.txt", L"INTASIA", gameId, compsAsia, 0);
        WriteExternalScriptFile(scriptPath / L"IntAmerica.txt", L"INTAMERICA", gameId, compsNorthAmerica, 0);
        WriteExternalScriptFile(scriptPath / L"IntOceania.txt", L"INTOCEANIA", gameId, compsOceania, 0);
        WriteExternalScriptFile(scriptPath / L"QualiWC.txt", L"QUALI_WC", gameId, compsQualiWC, 0);
        WriteExternalScriptFile(scriptPath / L"WC.txt", L"WORLD_CUP", gameId, compsWC, 1);
        WriteExternalScriptFile(scriptPath / L"QualiEC.txt", L"QUALI_EC", gameId, compsQualiEC, 1);
        WriteExternalScriptFile(scriptPath / L"EC.txt", L"EURO_CUP", gameId, compsEC, 1);
    }

    FifamWriter countriesWriter(dbPath / L"Countries.sav", gameId, FifamVersion(FifamDatabase::GetGameDbVersion(gameId)), unicode);
    if (countriesWriter.Available()) {
        auto &writer = countriesWriter;
        if (gameId >= 7) {
            UInt countriesVer = (gameId <= 7) ? 2 : 3;
            writer.WriteLine(Utils::Format(L"Countries Version: %d", countriesVer));
        }
        for (UInt i = 0; i < NUM_COUNTRIES; i++) {
            auto &country = mCountries[i];
            if (country && IsCountryPresent(gameId, country->mId)) {
                writer.WriteLineTranslationArray(country->mName, false);
                if (gameId >= 4)
                    writer.WriteLineTranslationArray(country->mAbbr, false);
                else
                    writer.WriteLine(country->mAbbr[FifamTranslation::English]);
                if (gameId >= 7)
                    writer.WriteLineTranslationArray(country->mNameGender);
                writer.WriteLine(country->mContinent);
                writer.WriteLine(country->mFirstLanguageForNames);
                writer.WriteLine(country->mSecondLanguageForNames);
                writer.WriteLine(country->mTerritory);
            }
        }
        countriesWriter.Close();
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
            FifamWriter writer(countryDataPath, gameId, version, unicode);
            if (writer.Available()) {
                country->Write(writer);
                writer.Close();
                if (gameId >= 11) {
                    if (mWritingOptions.mWriteFixtures) {
                        FifamWriter fixturesWriter(dbPath / L"fixture" / Utils::Format(L"CountryFixture%d.sav", countryFileId), gameId, version, unicode);
                        if (fixturesWriter.Available()) {
                            country->WriteFixtures(fixturesWriter);
                            fixturesWriter.Close();
                        }
                    }
                    FifamWriter scriptWriter(dbPath / L"script" / Utils::Format(L"CountryScript%d.sav", countryFileId), gameId, version, unicode);
                    if (scriptWriter.Available()) {
                        country->WriteScript(scriptWriter);
                        scriptWriter.Close();
                    }
                }
            }
        }
    }

    if (mWritingOptions.mWriteAssessment) {
        FifamWriter assessmentWriter(dbPath / L"Assessment.sav", gameId, version, unicode);
        if (assessmentWriter.Available()) {
            if (gameId > 7)
                assessmentWriter.WriteLine(L"Index\t[English]\t[French]\t[German]\t[Italian]\t[Spanish]\t[Polish]\tYear -6\tYear -5\tYear -4\tYear -3\tYear -2\tYear -1");
            else
                assessmentWriter.WriteLine(L"Index\t[English]\t[French]\t[German]\t[Italian]\t[Spanish]\tYear -6\tYear -5\tYear -4\tYear -3\tYear -2\tYear -1");
            for (UInt i = 0; i < NUM_COUNTRIES; i++) {
                if (mCountries[i] && mCountries[i]->mContinent == FifamContinent::Europe && IsCountryPresent(gameId, i + 1)) {
                    FifamNation nationId;
                    nationId.SetFromInt(i + 1);
                    FifamCountry *c = mCountries[i];
                    if (gameId > 7) {
                        assessmentWriter.WriteLineWithSeparator(L'\t', nationId,
                            c->mName[1], c->mName[0], c->mName[2], c->mName[3], c->mName[4], c->mName[5],
                            c->mAssessmentData[0], c->mAssessmentData[1], c->mAssessmentData[2], c->mAssessmentData[3], c->mAssessmentData[4], c->mAssessmentData[5]);
                    }
                    else {
                        assessmentWriter.WriteLineWithSeparator(L'\t', nationId,
                            c->mName[1], c->mName[0], c->mName[2], c->mName[3], c->mName[4],
                            c->mAssessmentData[0], c->mAssessmentData[1], c->mAssessmentData[2], c->mAssessmentData[3], c->mAssessmentData[4], c->mAssessmentData[5]);
                    }
                }
            }
            assessmentWriter.Close();
        }
    }

    FifamWriter withoutWriter(dbPath / L"Without.sav", gameId, version, unicode);
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
        withoutWriter.Close();
    }

    if (gameId >= 13)
        WritePlayerRelations(dbPath / L"PlayerRelations.sav", gameId);

    FifamWriter rulesWriter(dbPath / L"Rules.sav", gameId, version, unicode);
    if (rulesWriter.Available()) {
        mRules.Write(rulesWriter);
        rulesWriter.Close();
    }

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
            // TODO: update this
            country->mNumWriteableLeagueLevels = (gameId > 7) ? 16 : 5;
            country->mNationalTeam.SetIsWriteable(true);
            country->mNationalTeam.SetWriteableID(0xFFFF | (country->mId << 16));
            country->mNationalTeam.SetWriteableUniqueID(TranslateCountryEntityID(country->mNationalTeam.mUniqueID, LATEST_GAME_VERSION, gameId));
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
        club->SetWriteableUniqueID(TranslateCountryEntityID(club->mUniqueID, LATEST_GAME_VERSION, gameId));
    }
    for (auto stadium : mStadiums) {
        UInt id = 0;
        if (stadium->mCountry) {
            auto it = std::find(stadium->mCountry->mStadiums.begin(), stadium->mCountry->mStadiums.end(), stadium);
            if (it != stadium->mCountry->mStadiums.end()) {
                auto index = std::distance(stadium->mCountry->mStadiums.begin(), it);
                id = (index + 1) | (stadium->mCountry->mId << 16);
            }
        }
        stadium->SetIsWriteable(id != 0);
        stadium->SetWriteableID(id);
        stadium->SetWriteableUniqueID(id);
    }

    UInt lastEmpicsId = 0;
    Map<String, Vector<FifamPlayer *>> playerStrIDsCollisionsMap;

    UInt nextFreePersonId = 1;

    for (auto personEntry : mPersonsMap) {
        if (personEntry.first < 1'000'000'000 && personEntry.first > nextFreePersonId)
            nextFreePersonId = personEntry.first;
    }
    
    for (auto personEntry : mPersonsMap) {
        auto person = personEntry.second;
        person->SetIsWriteable(true);
        UInt writeableId = personEntry.second->mID;
        if (writeableId >= 1'000'000'000)
            writeableId = nextFreePersonId++;
        person->SetWriteableID(writeableId);
        person->SetWriteableUniqueID(writeableId);
        if (person->mPersonType == FifamPersonType::Player) {
            FifamPlayer *player = person->AsPlayer();
            if (player->mEmpicsId > lastEmpicsId)
                lastEmpicsId = player->mEmpicsId;
            playerStrIDsCollisionsMap[player->GetStringUniqueId(gameId, false)].push_back(player);
        }
        else {
            FifamStaff *staff = person->AsStaff();
            person->mWriteableStringID = staff->GetStringUniqueId(gameId);
        }
    }

    for (auto &entry : playerStrIDsCollisionsMap) {
        auto &players = entry.second;
        if (players.size() == 1) {
            players[0]->mWriteableStringID = entry.first;
            // TODO: uncomment this
            //players[0]->mEmpicsId = 0;
        }
        else {
            for (auto player : players) {
                if (player->mEmpicsId == 0) {
                    // TODO: maybe replace it with 'local' IDs (1,2,3,...)
                    player->mEmpicsId = ++lastEmpicsId;
                }
                player->mWriteableStringID = entry.first + Utils::Format(L"-%d", player->mEmpicsId);
            }
        }
    }
}

void FifamDatabase::ResetWriteableStatus() {
    for (auto club : mClubs)
        club->SetIsWriteable(true);
    for (auto personEntry : mPersonsMap)
        personEntry.second->SetIsWriteable(true);
    for (auto stadium : mStadiums)
        stadium->SetIsWriteable(true);
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
    auto it = mPersonsMap.find(id);
    if (it != mPersonsMap.end())
        Error(L"FifamDatabase::CreatePlayer: ID is already in use (%d)", id);
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
    auto it = mPersonsMap.find(id);
    if (it != mPersonsMap.end())
        Error(L"FifamDatabase::CreateStaff: ID is already in use (%d)", id);
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
    ResolveClubLink(staff->mWouldnSignFor, gameId);
    ResolvePlayerPtr(staff->mManagerFavouritePlayer);
}

void FifamDatabase::ResolveLinksForCompetition(FifamCompetition *comp, UInt gameId) {
    ResolveCompetitionList(comp->mPredecessors, gameId);
    ResolveCompetitionList(comp->mSuccessors, gameId);
    comp->mInstructions.ForAllCompetitionLinks([=](FifamCompID &compID, UInt, FifamAbstractInstruction *instruction) {
        ResolveCompetitionID(compID, gameId);
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
            else if (index > 0 && index <= mCountries[countryId - 1]->mClubs.size())
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

FifamStadium *FifamDatabase::StadiumFromID(UInt ID) {
    if (ID != 0) {
        UChar countryId = (ID >> 16) & 0xFF;
        if (countryId > 0 && countryId <= NUM_COUNTRIES && mCountries[countryId - 1]) {
            UShort index = ID & 0xFFFF;
            if (index > 0 && index <= mCountries[countryId - 1]->mClubs.size())
                return mCountries[countryId - 1]->mStadiums[index - 1];
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

UInt FifamDatabase::StadiumToID(FifamStadium const *stadium) {
    if (stadium->mCountry) {
        FifamCountry *country = stadium->mCountry;
        auto it = std::find(country->mStadiums.begin(), country->mStadiums.end(), stadium);
        if (it != country->mStadiums.end()) {
            auto index = std::distance(country->mStadiums.begin(), it);
            return (index + 1) | (country->mId << 16);
        }
    }
    return 0;
}

UInt FifamDatabase::TranslateCountryEntityID(UInt ID, UInt gameFrom, UInt gameTo) {
    if (gameFrom > 8 && gameTo > 8)
        return ID;
    UChar region = (ID >> 16) & 0xFF;
    if (FifamUtils::ConvertRegion(region, gameFrom, gameTo))
        return (ID & 0xFF00FFFF) | (region << 16);
    return 0;
}

void FifamDatabase::ResolveClubUniqueID(FifamClubLink &clubLink, UInt gameFrom, UInt gameTo) {
    UInt clubUID = TranslateCountryEntityID(FifamUtils::GetSavedClubIDFromClubLink(clubLink), gameFrom, gameTo);
    GetClubFromUID(clubLink, clubUID);
}

void FifamDatabase::ResolveClubUniqueID(UInt &uid, UInt gameFrom, UInt gameTo) {
    uid = TranslateCountryEntityID(uid, gameFrom, gameTo);
}

void FifamDatabase::ResolveClubLink(FifamClubLink &clubLink, UInt gameFrom, UInt gameTo) {
    clubLink = ClubFromID(TranslateCountryEntityID(FifamUtils::GetSavedClubIDFromClubLink(clubLink), gameFrom, gameTo));
}

void FifamDatabase::ResolvePlayerPtr(FifamPlayer *&player) {
    player = PlayerFromID(FifamUtils::GetSavedPlayerIDFromPtr(player));
}

void FifamDatabase::ResolveCompetitionID(FifamCompID &comp, UInt gameFrom, UInt gameTo) {
    comp = FifamUtils::TranslateCompId(comp, gameFrom, gameTo);
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

void FifamDatabase::ResolveCompetitionList(Vector<FifamCompID> &vec, UInt gameId, bool unique) {
    for (UInt i = 0; i < vec.size(); i++)
        ResolveCompetitionID(vec[i], gameId);
    Vector<FifamCompID> oldVec = vec;
    vec.clear();
    for (UInt i = 0; i < oldVec.size(); i++) {
        if (!oldVec[i].IsNull() && (!unique || !Utils::Contains(vec, oldVec[i])))
            vec.push_back(oldVec[i]);
    }
}

void FifamDatabase::ResolveStadiumPtr(FifamStadium *&stadium, UInt gameFrom, UInt gameTo) {
    stadium = StadiumFromID(TranslateCountryEntityID(FifamUtils::GetSavedStadiumIDFromPtr(stadium), gameFrom, gameTo));
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
    const UInt BASE_NEW_PERSON_ID = 1'000'000'000;
    if (mPersonsMap.contains(BASE_NEW_PERSON_ID))
        return (*mPersonsMap.rbegin()).first + 1;
    return BASE_NEW_PERSON_ID;
}

void FifamDatabase::ReadNamesFile(Path const &filepath, UInt gameId, NamesMap &outNames) {
    outNames.clear();
    FifamReader reader(filepath, gameId);
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
    FifamWriter writer(filepath, gameId, FifamVersion(), unicode);
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
    if (comp->HasProperty(L"customType"))
        writer.WriteLine(Utils::Format(L"{ %u, %s, %u }", comp->mID.mRegion.ToInt(), comp->GetProperty<String>(L"customType"), comp->mID.mIndex));
    else
        writer.WriteLine(FifamCompID(FifamUtils::GetWriteableID(comp->mID, writer.GetGameId())).ToStr());
    comp->Write(writer, this, nationId);
}

FifamCupAlloc *FifamDatabase::GetCupTemplate(FifamCupSystemType cupSystemType) {
    if (cupSystemType.ToInt() > 0 && cupSystemType.ToInt() <= mCupTemplates.size())
        return mCupTemplates[cupSystemType.ToInt() - 1];
    return nullptr;
}

FifamCountry *FifamDatabase::GetCountry(Int countryId) {
    if (countryId > 0 && countryId <= NUM_COUNTRIES)
        return mCountries[countryId - 1];
    return nullptr;
}

void FifamDatabase::ReadPlayerRelations(Path const &filepath, UInt gameId, Map<String, Vector<FifamPlayer *>> const &playerStrIDsMap) {
    FifamReader reader(filepath, gameId);
    if (reader.Available()) {
        std::wcout << L"Reading player relations" << std::endl;
        while (!reader.IsEof()) {
            auto line = reader.ReadFullLine();
            if (!line.empty() && line[0] != L'#') {
                auto lineParts = Utils::Split(line, L',', true, false);
                if (lineParts.size() > 2) {
                    UInt relationType = 0;
                    if (lineParts[0] == L"BROTHER")
                        relationType = 1;
                    else if (lineParts[0] == L"COUSIN")
                        relationType = 2;
                    if (relationType != 0) {
                        Set<FifamPlayer *> players;
                        for (UInt i = 1; i < lineParts.size(); i++) {
                            if (!lineParts[i].empty()) {
                                UInt empicsId = 0;
                                String nameWithoutEmpicsId;
                                auto lastHyph = lineParts[i].find_last_of(L'-');
                                if (lastHyph != String::npos && Utils::IsNumber(lineParts[i].substr(lastHyph + 1))) {
                                    nameWithoutEmpicsId = lineParts[i].substr(0, lastHyph);
                                    empicsId = Utils::SafeConvertInt<UInt>(lineParts[i].substr(lastHyph + 1));
                                }
                                else
                                    nameWithoutEmpicsId = lineParts[i];
                                auto it = playerStrIDsMap.find(nameWithoutEmpicsId);
                                if (it != playerStrIDsMap.end()) {
                                    FifamPlayer *player = nullptr;
                                    if ((*it).second.size() == 1)
                                        player = (*it).second[0];
                                    else {
                                        if (empicsId != 0) {
                                            for (auto p : ((*it).second)) {
                                                if (p->mEmpicsId == empicsId) {
                                                    player = p;
                                                    break;
                                                }
                                            }
                                        }
                                    }
                                    if (player)
                                        players.insert(player);
                                }
                            }
                        }
                        if (players.size() > 1) {
                            for (auto player : players) {
                                for (auto relatedPlayer : players) {
                                    if (player != relatedPlayer) {
                                        if (relationType == 1)
                                            player->mBrothers.insert(relatedPlayer);
                                        else
                                            player->mCousins.insert(relatedPlayer);
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}

void WriteRelationsLists(FifamWriter &writer, Vector<Set<FifamPlayer *>> &lists, String const &relationName) {
    if (!lists.empty()) {
        std::sort(lists.begin(), lists.end(), [](Set<FifamPlayer *> const &a, Set<FifamPlayer *> const &b) {
            return (*a.begin())->mNationality[0].ToInt() < (*b.begin())->mNationality[0].ToInt();
        });
        UInt lastNation = (*lists[0].begin())->mNationality[0].ToInt();
        for (auto &list : lists) {
            UInt currentNation = (*list.begin())->mNationality[0].ToInt();
            if (list.size() > 1) {
                if (lastNation != currentNation)
                    writer.WriteNewLine();
                writer.Write(relationName);
                for (auto p : list)
                    writer.Write(L",\t" + p->mWriteableStringID + L"\t");
                writer.WriteNewLine();
            }
            lastNation = currentNation;
        }
    }
}

void FifamDatabase::WritePlayerRelations(Path const &filepath, UInt gameId) {
    FifamWriter writer(filepath, gameId, FifamVersion(), gameId > 7);
    if (writer.Available()) {
        std::wcout << L"Writing player relations" << std::endl;
        writer.WriteLine(L"#Relation, Player1, Player2");
        writer.WriteNewLine();

        Vector<Set<FifamPlayer *>> brothersLists;
        Vector<Set<FifamPlayer *>> cousinsLists;

        for (FifamPlayer *player : mPlayers) {
            if (player->GetWriteableID() != 0) {
                if (!player->mBrothers.empty()) {
                    bool wasAlreadyAdded = false;
                    for (auto b : player->mBrothers) {
                        if (b->GetWriteableID() != 0 && b < player) {
                            wasAlreadyAdded = true;
                            break;
                        }
                    }
                    if (!wasAlreadyAdded) {
                        Set<FifamPlayer *> brothers;
                        brothers.insert(player);
                        for (auto b : player->mBrothers) {
                            if (b->GetWriteableID() != 0)
                                brothers.insert(b);
                        }
                        if (brothers.size() > 1)
                            brothersLists.push_back(brothers);
                    }
                }
                if (!player->mCousins.empty()) {
                    for (auto b : player->mCousins) {
                        if (player->GetWriteableID() != 0 && b > player)
                            cousinsLists.push_back({ player, b });
                    }
                }
            }
        }
        WriteRelationsLists(writer, brothersLists, L"BROTHER");
        for (UInt i = 0; i < 3; i++)
            writer.WriteNewLine();
        WriteRelationsLists(writer, cousinsLists, L"COUSIN");
    }
}

void FifamDatabase::ReadExternalScriptFile(Path const &filepath, String const &compKeyName, UInt gameId) {
    FifamReader reader(filepath, gameId);
    if (reader.Available()) {
        std::wcout << L"Reading script file " << filepath.filename() << L"" << std::endl;
        auto numComps = reader.ReadLine<UInt>();
        for (UInt i = 0; i < numComps; i++) {
            if (reader.ReadStartIndex(Utils::Format(L"%s%d", compKeyName.c_str(), i))) {
                ReadCompetition(reader, FifamNation::None);
                reader.ReadEndIndex(Utils::Format(L"%s%d", compKeyName.c_str(), i));
            }
        }
    }
}

void FifamDatabase::WriteExternalScriptFile(Path const &filepath, String const &compKeyName, UInt gameId,
    Vector<FifamCompEntry> const &comps, UInt startIndex)
{
    FifamWriter writer(filepath, gameId, FifamVersion(), gameId > 7);
    if (writer.Available()) {
        std::wcout << L"Writing script file \"" << filepath.filename() << L"\"" << std::endl;
        writer.WriteLine(comps.size() + startIndex);
        for (UInt i = 0; i < comps.size(); i++) {
            writer.WriteStartIndex(Utils::Format(L"%s%d", compKeyName.c_str(), startIndex + i));
            WriteCompetition(writer, comps[i].second, FifamNation::None);
            writer.WriteEndIndex(Utils::Format(L"%s%d", compKeyName.c_str(), startIndex + i));
            if (i != (comps.size() - 1))
                writer.WriteLine(L"; -----------------------------------------------------------------------------");
        }
    }
}

FifamVersion FifamDatabase::GetGameDbVersion(UInt gameId) {
    FifamVersion version;
    if (gameId == 3)
        version.Set(0x2003, 0x01);
    else if (gameId == 4)
        version.Set(0x2004, 0x08);
    else if (gameId == 5)
        version.Set(0x2005, 0x06);
    else if (gameId == 6)
        version.Set(0x2006, 0x04);
    else if (gameId == 7)
        version.Set(0x2007, 0x0C);
    else if (gameId == 8)
        version.Set(0x2007, 0x1E);
    else if (gameId == 9)
        version.Set(0x2009, 0x05);
    else if (gameId == 10)
        version.Set(0x2009, 0x0A);
    else if (gameId == 11)
        version.Set(0x2011, 0x0A);
    else if (gameId == 12)
        version.Set(0x2012, 0x04);
    else if (gameId == 13)
        version.Set(0x2013, 0x0A);
    else if (gameId == 14)
        version.Set(0x2013, 0x0A);
    return version;
}

Bool FifamDatabase::IsUnicodeUsedInGameVersion(UInt gameId) {
    return gameId >= 8;
}
