#include "FifamCountry.h"
#include "FifamDatabase.h"
#include "FifamUtils.h"
#include "FifamCompLeague.h"
#include "FifamCompRound.h"
#include "FifamCompPool.h"
#include "FifamCompCup.h"
#include "FifamNames.h"
#include "Error.h"

FifamCountry::FifamCountry(UInt id, FifamDatabase *db) {
    mId = id;
    mDatabase = db;
}

FifamReferee *FifamCountry::AddReferee() {
    FifamReferee *referee = new FifamReferee;
    referee->mCountry = this;
    mReferees.push_back(referee);
    mDatabase->mReferees.insert(referee);
    return referee;
}

FifamStadium *FifamCountry::AddStadium() {
    FifamStadium *stadium = new FifamStadium;
    mStadiums.push_back(stadium);
    mDatabase->mStadiums.insert(stadium);
    return stadium;
}

FifamSponsor *FifamCountry::AddSponsor() {
    FifamSponsor *sponsor = new FifamSponsor;
    mSponsors.push_back(sponsor);
    mDatabase->mSponsors.insert(sponsor);
    return sponsor;
}

FifamCACPlayer *FifamCountry::AddCACPlayer() {
    FifamCACPlayer *player = new FifamCACPlayer;
    mCACPlayers.push_back(player);
    mDatabase->mCACPlayers.insert(player);
    return player;
}

Bool FifamCountry::Read(FifamReader &reader) {
    if (reader.ReadStartIndex(L"COUNTRY")) {
        reader.ReadVersion();
        if (reader.IsVersionGreaterOrEqual(0x2007, 0x12))
            reader.SkipLine();
        if (reader.IsVersionGreaterOrEqual(0x2007, 0x17))
            reader.ReadLine(Unknown._2);

        if (FifamDatabase::mReadingOptions.mReadCountryCompetitions) {
            if (reader.GetGameId() < 11) {
                if (reader.ReadStartIndex(L"COMPETITIONPART"))
                    reader.ReadEndIndex(L"COMPETITIONPART");
                if (reader.ReadStartIndex(L"COMPETITION")) {
                    UInt numComps = reader.ReadLine<UInt>();
                    for (UInt i = 0; i < numComps; i++) {
                        if (reader.ReadStartIndex(Utils::Format(L"COMP%d", i))) {
                            mDatabase->ReadCompetition(reader, FifamNation::MakeFromInt(mId));
                            reader.ReadEndIndex(Utils::Format(L"COMP%d", i));
                        }
                    }
                    reader.ReadEndIndex(L"COMPETITION");
                }
            }
            else {
                UInt numLevelNames = reader.ReadLine<UInt>();
                for (UInt i = 0; i < numLevelNames; i++) {
                    auto &levelName = mLeagueLevelNames.emplace_back();
                    reader.ReadLineTranslationArray(levelName);
                }
                if (reader.IsVersionGreaterOrEqual(0x2011, 0x08)) {
                    UInt numCompNames = reader.ReadLine<UInt>();
                    for (UInt i = 0; i < numCompNames; i++) {
                        auto compInfo = reader.ReadLineArray<String>();
                        if (compInfo.size() >= 7) {
                            UInt compId = Utils::SafeConvertInt<UInt>(compInfo[0], true);
                            FifamCompetition *comp = mDatabase->GetCompetition(compId);
                            if (comp) {
                                for (UInt i = 0; i < FifamTranslation::NUM_TRANSLATIONS; i++)
                                    comp->mName[i] = compInfo[i + 1];
                            }
                        }
                    }
                }
            }
        }

        if (reader.ReadStartIndex(L"CLUBS")) {
            UInt numClubs = reader.ReadLine<UInt>();
            for (UInt i = 0; i < numClubs; i++) {
                if (FifamDatabase::mReadingOptions.mReadClubs) {
                    auto club = mDatabase->CreateClub(this);
                    club->Read(reader, i + 1);
                    mDatabase->AddClubToMap(club);
                    mClubsMap[club->mUniqueID] = club;
                }
                else {
                    FifamClub dummy;
                    dummy.Read(reader, i + 1);
                }
            }
            mNationalTeam.mCountry = this;
            mNationalTeam.mDatabase = mDatabase;
            mNationalTeam.mIsNationalTeam = true;
            mNationalTeam.Read(reader, 0xFFFF);
            reader.ReadEndIndex(L"CLUBS");
        }

        if (FifamDatabase::mReadingOptions.mReadPersons) {
            if (reader.ReadStartIndex(L"REFEREES")) {
                auto numReferees = reader.ReadLine<UInt>();
                for (UInt i = 0; i < numReferees; i++)
                    AddReferee()->Read(reader);
                reader.ReadEndIndex(L"REFEREES");
            }
        }

        if (reader.GetGameId() < 11) {
            if (reader.ReadStartIndex(L"SPONSORS")) {
                auto numStadiums = reader.ReadLine<UInt>();
                for (UInt i = 0; i < numStadiums; i++)
                    AddSponsor()->Read(reader);
                reader.ReadEndIndex(L"SPONSORS");
            }
        }

        if (reader.GetGameId() < 11) {
            if (reader.ReadStartIndex(L"STADIUMS")) {
                auto numStadiums = reader.ReadLine<UInt>();
                for (UInt i = 0; i < numStadiums; i++)
                    AddStadium()->Read(reader);
                reader.ReadEndIndex(L"STADIUMS");
            }
        }

        if (FifamDatabase::mReadingOptions.mReadPersons) {
            if (reader.IsVersionGreaterOrEqual(0x2009, 0x05)) {
                if (reader.ReadStartIndex(L"STAFFS")) {
                    UInt numStaffs = reader.ReadLine<UInt>();
                    UInt nextFreeId = mDatabase->GetNextFreePersonID();
                    for (UInt i = 0; i < numStaffs; i++) {
                        FifamStaff *staff = mDatabase->CreateStaff(nullptr, nextFreeId++);
                        staff->ReadWorker(reader);
                        staff->mLinkedCountry.SetFromInt(mId);
                    }
                    reader.ReadEndIndex(L"STAFFS");
                }
            }
        }

        if (FifamDatabase::mReadingOptions.mReadPersons) {
            if (reader.ReadStartIndex(L"ADDMANAGER")) {
                UInt numStaffs = reader.ReadLine<UInt>();
                UInt nextFreeId = mDatabase->GetNextFreePersonID();
                for (UInt i = 0; i < numStaffs; i++) {
                    FifamStaff *staff = mDatabase->CreateStaff(nullptr, nextFreeId++);
                    if (reader.IsVersionGreaterOrEqual(0x2009, 0x05))
                        staff->Read(reader);
                    else
                        staff->ReadFromPlayer(reader);
                    staff->mLinkedCountry.SetFromInt(mId);
                }
                reader.ReadEndIndex(L"ADDMANAGER");
            }
        }

        if (FifamDatabase::mReadingOptions.mReadPersons) {
            if (reader.ReadStartIndex(L"PLAYERPOOL")) {
                UInt numCACPlayers = reader.ReadLine<UInt>();
                for (UInt i = 0; i < numCACPlayers; i++)
                    AddCACPlayer()->Read(reader);
                reader.ReadEndIndex(L"PLAYERPOOL");
            }
        }

        String countryMiscSectionName;
        if (reader.GetGameId() < 11)
            countryMiscSectionName = L"COUNTRY_MIST";
        else
            countryMiscSectionName = L"COUNTRY_MISC";

        if (reader.ReadStartIndex(countryMiscSectionName)) {
            if (reader.GetGameId() < 11) {
                reader.ReadFullLine();
                reader.ReadFullLine();
                reader.ReadFullLine();
                if (reader.IsVersionGreaterOrEqual(0x2006, 0))
                    reader.ReadLine(mOriginalLeagueSystem);
                else
                    reader.SkipLines(2);
            }
            reader.ReadFullLine(mFA_Name);
            if (reader.IsVersionGreaterOrEqual(0x2004, 0)) {
                reader.ReadLineArray(mLanguages);
                reader.ReadLineArray(mLanguageFrequency);
            }
            else {
                reader.ReadLine(mLanguages[0]);
                reader.ReadLine(mLanguageFrequency[0]);
            }
            reader.ReadLine(mLeagueLevelWithReserveTeams);
            reader.ReadLine(Unknown._1);
            if (reader.GetGameId() < 11) {
                reader.ReadFullLine(mFA_PresidentFirstName);
                reader.ReadFullLine(mFA_PresidentLastName);
                reader.ReadLine(mFA_PresidentBirthDate);
            }
            if (!reader.IsVersionGreaterOrEqual(0x2007, 0x17)) {
                reader.ReadFullLine(mNewspaperReporterFirstName);
                reader.ReadFullLine(mNewspaperReporterLastName);
                reader.ReadFullLine(mReporterInTheStadiumFirstName);
                reader.ReadFullLine(mReporterInTheStadiumLastName);
                reader.ReadFullLine(mReporterInTheStudioFirstName);
                reader.ReadFullLine(mReporterInTheStudioLastName);
            }
            UChar yelCardsRule;
            reader.ReadLine(yelCardsRule);
            mYellowCardsLeagueRule.SetFromInt(yelCardsRule & 0x7F);
            mYellowCardsLeagueAfterYelRed = Utils::CheckFlag(yelCardsRule, 0x80);
            reader.ReadLine(yelCardsRule);
            mYellowCardsCupRule.SetFromInt(yelCardsRule & 0x7F);
            mYellowCardsCupAfterYelRed = Utils::CheckFlag(yelCardsRule, 0x80);
            UChar redCardOptions = reader.ReadLine<UChar>();
            if (redCardOptions & 1)
                Unknown._8 = true;
            if (redCardOptions & 2)
                Unknown._9 = true;
            if (redCardOptions & 16)
                mRedCardBanFixedDuration = true;
            reader.ReadLine(mRedCardRule);
            UChar validForAllCompsFlags = reader.ReadLine<UChar>();
            if (validForAllCompsFlags & 1)
                mYellowCardsBanForAllComp = true;
            if (validForAllCompsFlags & 2)
                mSecondYellowCardBanForAllComp = true;
            if (validForAllCompsFlags & 4)
                mRedCardsBanForAllComp = true;
            UChar secondYellowCardLeaguesState = reader.ReadLine<UChar>();
            if (secondYellowCardLeaguesState & 1)
                mSecondYellowCardLeagueState[0] = true;
            if (secondYellowCardLeaguesState & 2)
                mSecondYellowCardLeagueState[1] = true;
            if (secondYellowCardLeaguesState & 4)
                mSecondYellowCardLeagueState[2] = true;
            if (secondYellowCardLeaguesState & 8)
                mSecondYellowCardLeagueState[3] = true;
            if (secondYellowCardLeaguesState & 16)
                mSecondYellowCardLeagueState[4] = true;
            UInt numLeagueLevels = reader.IsVersionGreaterOrEqual(0x2007, 0x13) ? 16 : 5;
            for (UInt i = 0; i < numLeagueLevels; i++)
                mLeagueLevels[i].Read(reader);
            reader.ReadLine(mCupSystemType);
            UChar cupOptions = reader.ReadLine<UChar>();
            if (cupOptions & 1)
                mCupHomeAdvantageForTeamsInLowerLeagues = true;
            if (cupOptions & 2)
                mCupCanPlayersBeCupTied = true;
            reader.ReadLine(mCupFinalStadium);
            reader.ReadLine(mSuperCupType);
            UInt leagueCupType = reader.ReadLine<UInt>();
            mLeagueCupSystemType[0].SetFromInt(leagueCupType & 0xFF);
            mLeagueCupSystemType[1].SetFromInt((leagueCupType >> 8) & 0xFF);
            mLeagueCupSystemType[2].SetFromInt((leagueCupType >> 16) & 0xFF);
            mLeagueCupSystemType[3].SetFromInt((leagueCupType >> 24) & 0xFF);
            if (reader.IsVersionGreaterOrEqual(0x2005, 2)) {
                reader.ReadLine(mYearsForNaturalization);
                reader.ReadLine(Unknown._10[0]);
                reader.ReadLine(Unknown._10[1]);
                reader.ReadLine(Unknown._11);
                reader.ReadLine(Unknown._12);
                reader.ReadLine(Unknown._13);
                reader.ReadLine(Unknown._14);
            }
            UChar flags = reader.ReadLine<UChar>();
            if (reader.GetGameId() <= 7)
                flags &= 6;
            else
                flags &= 181;
            if (flags & 1)
                Unknown.flags._1 = true;
            if (flags & 2)
                mGeneratePlayers = true;
            if (flags & 4)
                mNoTerracesAllowed = true;
            if (flags & 8)
                Unknown.flags._8 = true;
            if (flags & 16)
                Unknown.flags._16 = true;
            if (flags & 32)
                Unknown.flags._32 = true;
            if (flags & 64)
                Unknown.flags._64 = true;
            if (flags & 128)
                Unknown.flags._128 = true;
            if (!reader.IsVersionGreaterOrEqual(0x2005, 0))
                reader.ReadLine(Unknown._19);
            reader.ReadLine(Unknown._15);
            reader.ReadLine(mAmateurRule);
            reader.ReadLine(mClimate);
            for (UInt i = 0; i < 6; i++)
                reader.ReadLine(mAssessmentInfo[i]);
            reader.ReadLine(mProbabilityForOverseasTransfers);
            reader.ReadLine(Unknown._16);
            reader.ReadLine(Unknown._17);
            if (reader.GetGameId() < 11)
                mManager.Read(reader);
            reader.ReadLine(mAveragePlayerRating);
            if (!reader.IsVersionGreaterOrEqual(0x2006, 0)) {
                mAveragePlayerRating *= 3;
                if (mAveragePlayerRating > 100)
                    mAveragePlayerRating = 100;
            }
            reader.ReadLine(Unknown._18);
            reader.ReadLine(mAppearanceHairColor);
            if (reader.IsVersionGreaterOrEqual(0x2007, 4)) {
                reader.ReadLine(mAppearanceHairStyle[0]);
                reader.ReadLine(mAppearanceHairStyle[1]);
                reader.ReadLine(mAppearanceBeard);
                for (UInt i = 0; i < 4; i++)
                    reader.ReadLine(mAppearanceFaceType[i]);
            }
            else {
                reader.ReadLine(mAppearanceHairStyle[0]);
                reader.ReadLine(mAppearanceBeard);
                reader.ReadLine(mAppearanceFaceType[0]);
            }
            reader.ReadLine(mAverageHeight);
            reader.ReadLine(mAverageWeight);
            if (reader.IsVersionGreaterOrEqual(0x2011, 0x01)) {
                UInt numAppearanceDistributions = reader.ReadLine<UInt>();
                mAppearanceData.resize(numAppearanceDistributions);
                for (UInt i = 0; i < numAppearanceDistributions; i++)
                    mAppearanceData[i].Read(reader);
            }
            if (reader.IsVersionGreaterOrEqual(0x2007, 0x0E)) {
                reader.ReadLine(mPreferredTransfersTerritory);
                reader.ReadLine(mFifaRanking);
            }
            if (reader.GetGameId() < 11)
                reader.ReadFullLine(mMostImportantMagazine);
            reader.ReadLine(mNumContinentalChampions);
            reader.ReadLine(mNumWorldCups);
            if (reader.IsVersionGreaterOrEqual(0x2004, 0)) {
                reader.ReadLine(mNumContinentalRunnersUp);
                reader.ReadLine(mNumWorldCupRunnersUp);
                if (reader.IsVersionGreaterOrEqual(0x2004, 8))
                    reader.ReadFullLine(mNotes);
            }
            reader.ReadEndIndex(countryMiscSectionName);

            if (!reader.IsVersionGreaterOrEqual(0x2006, 0)) {
                // create pools
            }

            if (!reader.IsVersionGreaterOrEqual(0x2007, 0)) {
                Array<Float, 10> mp = { 1.0f, 0.9f, 0.82f, 0.75f, 0.68f, 0.57f, 0.46f, 0.3f, 0.2f, 0.1f };
                for (UInt i = 0; i < 10; i++)
                    mLeagueLevels[i].mRating = UChar(Float(mAveragePlayerRating) * mp[i]);
            }

            FifamCheckEnum(mYellowCardsLeagueRule);
            FifamCheckEnum(mYellowCardsCupRule);
            FifamCheckEnum(mRedCardRule);
            if (validForAllCompsFlags >= 8)
                Error("validForAllCompsFlags: unknown value (%u)", validForAllCompsFlags);
            if (secondYellowCardLeaguesState >= 32)
                Error("secondYellowCardLeaguesState: unknown value (%u)", secondYellowCardLeaguesState);
            if ((redCardOptions & 4) || (redCardOptions & 8) || (redCardOptions >= 32))
                Error("redCardOptions: unknown value (%u)", redCardOptions);
            for (UInt i = 0; i < numLeagueLevels; i++)
                FifamCheckEnum(mLeagueLevels[i].mEqualPointsSorting);
            FifamCheckEnum(mCupSystemType);
            if (cupOptions >= 4)
                Error("cupOptions: unknown value (%u)", cupOptions);
            FifamCheckEnum(mSuperCupType);
            for (UInt i = 0; i < 4; i++)
                FifamCheckEnum(mLeagueCupSystemType[i]);
            FifamCheckEnum(mAmateurRule);
            FifamCheckEnum(mClimate);
            FifamCheckEnum(mPreferredTransfersTerritory);
        }
        reader.ReadEndIndex(L"COUNTRY");
        return true;
    }
    return false;
}

Bool FifamCountry::Write(FifamWriter &writer) {
    UInt maxLeagueLevels = GetNumWriteableLeagueLevels();
    writer.WriteStartIndex(L"COUNTRY");
    writer.WriteVersion();
    if (writer.IsVersionGreaterOrEqual(0x2007, 0x12)) {
        if (!mClubsMap.empty())
            writer.WriteLine(mClubsMap.rbegin()->first + 1);
        else
            writer.WriteLine((mId << 16) | 1);
    }
    if (writer.IsVersionGreaterOrEqual(0x2007, 0x17))
        writer.WriteLine(Unknown._2);
    Vector<FifamCompEntry> compLeagues;
    if (writer.GetGameId() < 11) {
        writer.WriteLine(IsCompetitionSystemCorrect());
        //writer.WriteStartIndex(L"COMPETITIONPART");
        //// TODO
        //writer.WriteEndIndex(L"COMPETITIONPART");
        writer.WriteStartIndex(L"COMPETITION");
        auto comps = GetCompetitions(true);
        Vector<FifamCompEntry> compRelegations, compPools, compFACups, compLECups, compSupercups;
        auto IsCupWriteable = [](FifamCompetition *comp) {
            if (comp->GetDbType() == FifamCompDbType::Cup)
                return comp->AsCup()->mCupTemplate != FifamCupSystemType::None || FifamCompCup::DetectCupSystemType(comp->AsCup()) != FifamCupSystemType::None;
            return false;
        };
        for (auto const &compEntry : comps) {
            if (compEntry.first.mType == FifamCompType::League)
                compLeagues.push_back(compEntry);
            else if (compEntry.first.mType == FifamCompType::Relegation)
                compRelegations.push_back(compEntry);
            else if (compEntry.first.mType == FifamCompType::Pool)
                compPools.push_back(compEntry);
            else if (compEntry.first.mType == FifamCompType::FaCup) {
                if (IsCupWriteable(compEntry.second))
                    compFACups.push_back(compEntry);
            }
            else if (compEntry.first.mType == FifamCompType::LeagueCup) {
                if (IsCupWriteable(compEntry.second))
                    compLECups.push_back(compEntry);
            }
            else if (compEntry.first.mType == FifamCompType::SuperCup) {
                if (IsCupWriteable(compEntry.second))
                    compSupercups.push_back(compEntry);
            }
        }
        writer.WriteLine(compLeagues.size() + compRelegations.size() + compPools.size() + compFACups.size() + compLECups.size() + compSupercups.size());
        UInt compIndex = 0;
        auto WriteComps = [&](Vector<FifamCompEntry> const &compPairs) {
            for (UInt i = 0; i < compPairs.size(); i++) {
                writer.WriteStartIndex(Utils::Format(L"COMP%d", compIndex));
                mDatabase->WriteCompetition(writer, compPairs[i].second, FifamNation::MakeFromInt(mId));
                writer.WriteEndIndex(Utils::Format(L"COMP%d", compIndex++));
            }
        };
        WriteComps(compLeagues);
        WriteComps(compRelegations);
        WriteComps(compPools);
        WriteComps(compFACups);
        WriteComps(compLECups);
        WriteComps(compSupercups);
        UInt numLevelNames = Utils::Min(maxLeagueLevels, mLeagueLevelNames.size());
        writer.WriteLine(numLevelNames);
        for (UInt i = 0; i < numLevelNames; i++)
            writer.WriteLineTranslationArray(mLeagueLevelNames[i]);
        writer.WriteEndIndex(L"COMPETITION");
    }
    else {
        UInt numLevelNames = Utils::Min(maxLeagueLevels, mLeagueLevelNames.size());
        writer.WriteLine(numLevelNames);
        for (UInt i = 0; i < numLevelNames; i++)
            writer.WriteLineTranslationArray(mLeagueLevelNames[i]);
        if (writer.IsVersionGreaterOrEqual(0x2011, 0x08)) {
            auto comps = GetCompetitions();
            writer.WriteLine(comps.size());
            for (UInt i = 0; i < comps.size(); i++) {
                writer.Write(comps[i].first.ToHexStr());
                writer.Write(L",");
                writer.WriteLineTranslationArray(comps[i].second->mName);
            }
        }
    }
    writer.WriteStartIndex(L"CLUBS");
    Vector<FifamClub *> clubs;
    for (FifamClub *club : mClubs) {
        if (club->GetIsWriteable())
            clubs.push_back(club);
    }
    writer.WriteLine(clubs.size());
    for (UInt i = 0; i < clubs.size(); i++)
        clubs[i]->Write(writer, i + 1);
    mNationalTeam.Write(writer, 0xFFFF);
    writer.WriteEndIndex(L"CLUBS");

    writer.WriteStartIndex(L"REFEREES");
    writer.WriteLine(mReferees.size());
    for (auto referee : mReferees)
        referee->Write(writer);
    writer.WriteEndIndex(L"REFEREES");

    if (writer.GetGameId() < 11) {
        writer.WriteStartIndex(L"SPONSORS");
        writer.WriteLine(mSponsors.size());
        for (auto sponsor : mSponsors)
            sponsor->Write(writer);
        writer.WriteEndIndex(L"SPONSORS");
    }

    Vector<FifamStaff *> staffWorkers;
    Vector<FifamStaff *> staffBoardAndManagers;

    for (FifamStaff *staff : mDatabase->mStaffs) {
        if (!staff->mClub && staff->mLinkedCountry.ToInt() == mId) {
            if (staff->GetIsWriteable() || mDatabase->mWritingOptions.mNonWriteableStaffsAreFreeAgents) {
                if (staff->mClubPosition.ToInt() < 4)
                    staffBoardAndManagers.push_back(staff);
                else
                    staffWorkers.push_back(staff);
            }
        }
    }

    if (writer.GetGameId() < 11) {
        writer.WriteStartIndex(L"STADIUMS");
        writer.WriteLine(mStadiums.size());
        for (auto stadium : mStadiums)
            stadium->Write(writer);
        writer.WriteEndIndex(L"STADIUMS");
    }

    if (writer.IsVersionGreaterOrEqual(0x2009, 0x05)) {
        writer.WriteStartIndex(L"STAFFS");
        writer.WriteLine(staffWorkers.size());
        for (FifamStaff *staff : staffWorkers)
            staff->WriteWorker(writer);
        writer.WriteEndIndex(L"STAFFS");
    }

    writer.WriteStartIndex(L"ADDMANAGER");
    writer.WriteLine(staffBoardAndManagers.size());
    for (FifamStaff *staff : staffBoardAndManagers) {
        if (writer.IsVersionGreaterOrEqual(0x2009, 0x05))
            staff->Write(writer);
        else
            staff->WriteToPlayer(writer);
    }
    writer.WriteEndIndex(L"ADDMANAGER");

    writer.WriteStartIndex(L"PLAYERPOOL");
    writer.WriteLine(mCACPlayers.size());
    for (auto player : mCACPlayers)
        player->Write(writer);
    writer.WriteEndIndex(L"PLAYERPOOL");

    String countryMiscSectionName;
    if (writer.GetGameId() < 11)
        countryMiscSectionName = L"COUNTRY_MIST";
    else
        countryMiscSectionName = L"COUNTRY_MISC";

    writer.WriteStartIndex(countryMiscSectionName);
    if (writer.GetGameId() < 11) {
        writer.WriteLine(FifamTr(mName));
        writer.WriteLine(FifamTr(mAbbr));
        writer.WriteLine((mContinent.ToInt() & 7) | (mTerritory.ToInt() << 3));
        if (writer.IsVersionGreaterOrEqual(0x2006, 0))
            writer.WriteLine(mOriginalLeagueSystem);
        else {
            writer.WriteLine(compLeagues.size());
            writer.WriteLine(0);
        }
    }
    writer.WriteLine(mFA_Name);
    writer.WriteLineArray(mLanguages);
    writer.WriteLineArray(mLanguageFrequency);
    writer.WriteLine(mLeagueLevelWithReserveTeams);
    writer.WriteLine(Unknown._1);
    if (writer.GetGameId() < 11) {
        writer.WriteLine(mFA_PresidentFirstName);
        writer.WriteLine(mFA_PresidentLastName);
        writer.WriteLine(mFA_PresidentBirthDate);
    }
    if (!writer.IsVersionGreaterOrEqual(0x2007, 0x17)) {
        writer.WriteLine(mNewspaperReporterFirstName);
        writer.WriteLine(mNewspaperReporterLastName);
        writer.WriteLine(mReporterInTheStadiumFirstName);
        writer.WriteLine(mReporterInTheStadiumLastName);
        writer.WriteLine(mReporterInTheStudioFirstName);
        writer.WriteLine(mReporterInTheStudioLastName);
    }
    UChar yelCardsRule = mYellowCardsLeagueRule.ToInt() & 0x7F;
    if (mYellowCardsLeagueAfterYelRed)
        yelCardsRule |= 0x80;
    writer.WriteLine(yelCardsRule);
    yelCardsRule = mYellowCardsCupRule.ToInt() & 0x7F;
    if (mYellowCardsCupAfterYelRed)
        yelCardsRule |= 0x80;
    writer.WriteLine(yelCardsRule);
    UChar redCardOptions = 0;
    if (Unknown._8)
        redCardOptions |= 1;
    if (Unknown._9)
        redCardOptions |= 2;
    if (mRedCardBanFixedDuration)
        redCardOptions |= 16;
    writer.WriteLine(redCardOptions);
    writer.WriteLine(mRedCardRule);
    UChar validForAllCompsFlags = 0;
    if (mYellowCardsBanForAllComp)
        validForAllCompsFlags |= 1;
    if (mSecondYellowCardBanForAllComp)
        validForAllCompsFlags |= 2;
    if (mRedCardsBanForAllComp)
        validForAllCompsFlags |= 4;
    writer.WriteLine(validForAllCompsFlags);
    UChar secondYellowCardLeaguesState = 0;
    if (mSecondYellowCardLeagueState[0])
        secondYellowCardLeaguesState |= 1;
    if (mSecondYellowCardLeagueState[1])
        secondYellowCardLeaguesState |= 2;
    if (mSecondYellowCardLeagueState[2])
        secondYellowCardLeaguesState |= 4;
    if (mSecondYellowCardLeagueState[3])
        secondYellowCardLeaguesState |= 8;
    if (mSecondYellowCardLeagueState[4])
        secondYellowCardLeaguesState |= 16;
    writer.WriteLine(secondYellowCardLeaguesState);
    UInt numLeagueLevels = writer.IsVersionGreaterOrEqual(0x2007, 0x13) ? 16 : 5;
    if (!writer.IsVersionGreaterOrEqual(0x2006, 0)) {
        Map<UChar, Pair<UChar, UChar>> countryLeagueLevelsLeagues;
        UChar leagueCounter = 0;
        for (auto l : compLeagues) {
            if (l.second->GetDbType() == FifamCompDbType::League) {
                if (countryLeagueLevelsLeagues.find(l.second->AsLeague()->mLeagueLevel) == countryLeagueLevelsLeagues.end()) {
                    auto &p = countryLeagueLevelsLeagues[l.second->AsLeague()->mLeagueLevel];
                    p.first = leagueCounter;
                    p.second = leagueCounter;
                }
                else {
                    auto &p = countryLeagueLevelsLeagues[l.second->AsLeague()->mLeagueLevel];
                    p.second = leagueCounter;
                }
                leagueCounter++;
            }
        }
        for (UInt i = 0; i < numLeagueLevels; i++) {
            if (countryLeagueLevelsLeagues.find(i) == countryLeagueLevelsLeagues.end())
                mLeagueLevels[i].Write(writer, leagueCounter, leagueCounter);
            else
                mLeagueLevels[i].Write(writer, countryLeagueLevelsLeagues[i].first, countryLeagueLevelsLeagues[i].second);
        }
    }
    else {
        for (UInt i = 0; i < numLeagueLevels; i++)
            mLeagueLevels[i].Write(writer, 0, 0);
    }
    writer.WriteLine(mCupSystemType);
    UChar cupOptions = 0;
    if (mCupHomeAdvantageForTeamsInLowerLeagues)
        cupOptions |= 1;
    if (mCupCanPlayersBeCupTied)
        cupOptions |= 2;
    writer.WriteLine(cupOptions);
    writer.WriteLine(mCupFinalStadium);
    writer.WriteLine(mSuperCupType);
    writer.WriteLine(mLeagueCupSystemType[0].ToInt() | (mLeagueCupSystemType[1].ToInt() << 8)
        | (mLeagueCupSystemType[2].ToInt() << 16) | (mLeagueCupSystemType[3].ToInt() << 24));
    writer.WriteLine(mYearsForNaturalization);
    writer.WriteLine(Unknown._10[0]);
    writer.WriteLine(Unknown._10[1]);
    writer.WriteLine(Unknown._11);
    writer.WriteLine(Unknown._12);
    writer.WriteLine(Unknown._13);
    writer.WriteLine(Unknown._14);
    UChar flags = 0;
    if (Unknown.flags._1)
        flags |= 1;
    if (mGeneratePlayers)
        flags |= 2;
    if (mNoTerracesAllowed)
        flags |= 4;
    if (Unknown.flags._8)
        flags |= 8;
    if (Unknown.flags._16)
        flags |= 16;
    if (Unknown.flags._32)
        flags |= 32;
    if (Unknown.flags._64)
        flags |= 64;
    if (Unknown.flags._128)
        flags |= 128;
    if (writer.GetGameId() <= 7)
        flags &= 6;
    else
        flags &= 181;
    writer.WriteLine(flags);
    writer.WriteLine(Unknown._15);
    writer.WriteLine(mAmateurRule);
    writer.WriteLine(mClimate);
    for (UInt i = 0; i < 6; i++)
        writer.WriteLine(mAssessmentInfo[i]);
    writer.WriteLine(mProbabilityForOverseasTransfers);
    writer.WriteLine(Unknown._16);
    writer.WriteLine(Unknown._17);
    if (writer.GetGameId() < 11)
        mManager.Write(writer);
    writer.WriteLine(mAveragePlayerRating);
    writer.WriteLine(Unknown._18);
    writer.WriteLine(mAppearanceHairColor);
    writer.WriteLine(mAppearanceHairStyle[0]);
    writer.WriteLine(mAppearanceHairStyle[1]);
    writer.WriteLine(mAppearanceBeard);
    for (UInt i = 0; i < 4; i++)
        writer.WriteLine(mAppearanceFaceType[i]);
    writer.WriteLine(mAverageHeight);
    writer.WriteLine(mAverageWeight);
    if (writer.IsVersionGreaterOrEqual(0x2011, 0x01)) {
        writer.WriteLine(mAppearanceData.size());
        for (auto &appearance : mAppearanceData)
            appearance.Write(writer);
    }
    if (writer.IsVersionGreaterOrEqual(0x2007, 0xEu)) {
        writer.WriteLine(mPreferredTransfersTerritory);
        writer.WriteLine(mFifaRanking);
    }
    if (writer.GetGameId() < 11)
        writer.WriteLine(mMostImportantMagazine);
    writer.WriteLine(mNumContinentalChampions);
    writer.WriteLine(mNumWorldCups);
    writer.WriteLine(mNumContinentalRunnersUp);
    writer.WriteLine(mNumWorldCupRunnersUp);
    writer.WriteLine(mNotes);
    writer.WriteEndIndex(countryMiscSectionName);
    writer.WriteEndIndex(L"COUNTRY");
    return true;
}

Bool FifamCountry::ReadScript(FifamReader &reader) {
    reader.ReadVersion();
    while (reader.ReadStartIndex(L"COMPETITION")) {
        mDatabase->ReadCompetition(reader, FifamNation::MakeFromInt(mId));
        reader.ReadEndIndex(L"COMPETITION");
    }
    return true;
}

Bool FifamCountry::WriteFixtures(FifamWriter &writer) {
    writer.WriteVersion();
    auto countryComps = GetCompetitions(true);
    for (auto &compEntry : countryComps) {
        FifamCompetition *comp = compEntry.second;
        writer.WriteLine(L"; -------------------------------------------------------------------");
        writer.WriteStartIndex(L"COMPETITION");
        writer.WriteLine(FifamCompID(FifamUtils::GetWriteableID(comp->mID, writer.GetGameId())).ToStr());
        writer.WriteLine(comp->GetDbType().ToStr());
        UInt numTeams = 0;
        UInt numRegisteredTeams = 0;
        FifamDbWriteableIDsList teamIDs;
        Vector<UShort> matchdaysFirstSeason;
        Vector<UShort> matchdaysSecondSeason;
        Vector<Vector<Pair<UChar, UChar>>> fixtures;
        Vector<Array<UInt, 4>> bonuses;
        if (comp->GetDbType() == FifamCompDbType::League) {
            auto league = comp->AsLeague();
            numTeams = league->mNumTeams;
            teamIDs = FifamUtils::MakeWriteableIDsList(league->mTeams);
            numRegisteredTeams = Utils::Min(numTeams, teamIDs.size());
            matchdaysFirstSeason = league->mFirstSeasonMatchdays;
            matchdaysSecondSeason = league->mSecondSeasonMatchdays;
            fixtures = league->mFixtures;
            bonuses = { league->mBonuses };
        }
        else if (comp->GetDbType() == FifamCompDbType::Round) {
            auto round = comp->AsRound();
            numTeams = round->mNumTeams;
            matchdaysFirstSeason = { round->mFirstSeasonMatchdays[0], round->mFirstSeasonMatchdays[1] };
            matchdaysSecondSeason = { round->mSecondSeasonMatchdays[0], round->mSecondSeasonMatchdays[1] };
            bonuses = { round->mBonuses };
        }
        else if (comp->GetDbType() == FifamCompDbType::Pool) {
            auto pool = comp->AsPool();
            numTeams = pool->mNumTeams;
            bonuses = { pool->mBonuses };
        }
        else if (comp->GetDbType() == FifamCompDbType::Cup) {
            auto cup = comp->AsCup();
            numTeams = cup->mNumTeams;
            matchdaysFirstSeason = cup->mFirstSeasonMatchdays;
            matchdaysSecondSeason = cup->mSecondSeasonMatchdays;
            bonuses.resize(cup->mRounds.size());
            for (UInt i = 0; i < cup->mRounds.size(); i++)
                bonuses[i] = cup->mRounds[i].mBonuses;
        }
        writer.WriteLine(numTeams);
        writer.WriteLine(numRegisteredTeams);
        writer.WriteStartIndex(L"TEAMS");
        if (numRegisteredTeams > 0) {
            for (UInt i = 0; i < numRegisteredTeams; i++) {
                if (i != 0)
                    writer.Write(L",");
                writer.Write(Utils::Format(L"%08X", teamIDs[i]));
            }
            writer.WriteNewLine();
        }
        writer.WriteEndIndex(L"TEAMS");
        if (comp->GetDbType() == FifamCompDbType::League || comp->GetDbType() == FifamCompDbType::Cup || comp->GetDbType() == FifamCompDbType::Round) {
            writer.WriteStartIndex(L"MATCHDAYS");
            Utils::Remove(matchdaysFirstSeason, 0);
            writer.WriteLineArray(matchdaysFirstSeason);
            Utils::Remove(matchdaysSecondSeason, 0);
            writer.WriteLineArray(matchdaysSecondSeason);
            writer.WriteEndIndex(L"MATCHDAYS");
        }
        if (comp->GetDbType() == FifamCompDbType::League) {
            writer.WriteStartIndex(L"FIXTURES");
            if (fixtures.size() > 0) {
                writer.WriteLine(fixtures.size(), fixtures[0].size());
                for (UInt i = 0; i < fixtures.size(); i++) {
                    for (UInt f = 0; f < fixtures[i].size(); f++) {
                        if (f != 0)
                            writer.Write(L",");
                        writer.Write(fixtures[i][f].first, fixtures[i][f].second);
                    }
                    writer.WriteNewLine();
                }
            }
            else {
                writer.WriteLine(0, 0);
            }
            writer.WriteEndIndex(L"FIXTURES");
        }
        writer.WriteStartIndex(L"BONUS");
        for (UInt i = 0; i < 4; i++) {
            Vector<UInt> roundBonuses(bonuses.size());
            for (UInt b = 0; b < bonuses.size(); b++)
                roundBonuses[b] = bonuses[b][i];
            writer.WriteLineArray(roundBonuses);
        }
        writer.WriteEndIndex(L"BONUS");
        writer.WriteEndIndex(L"COMPETITION");
    }
    return true;
}

Bool FifamCountry::WriteScript(FifamWriter &writer) {
    writer.WriteVersion();
    auto countryComps = GetCompetitions(true);
    for (auto &comp : countryComps) {
        writer.WriteStartIndex(L"COMPETITION");
        mDatabase->WriteCompetition(writer, comp.second, FifamNation::MakeFromInt(mId));
        writer.WriteEndIndex(L"COMPETITION");
    }
    return true;
}

Bool FifamCountry::IsCompetitionSystemCorrect() {
    // TODO check only writeable clubs
    if (mClubs.size() < 10)
        return false;
    FifamCompetition *compLeague = mDatabase->GetCompetition({ mId, FifamCompType::League, 0 });
    if (!compLeague || compLeague->AsLeague()->mNumTeams < 8)
        return false;
    // check competition errors
    return true;
}

Vector<FifamCompEntry> FifamCountry::GetCompetitions(bool onlyWriteable) {
    Vector<FifamCompEntry> countryComps;
    for (auto const &compEntry : mDatabase->mCompMap) {
        if (compEntry.first.mRegion.ToInt() == mId) {
            if (!onlyWriteable ||
                compEntry.second->GetDbType() == FifamCompDbType::League ||
                compEntry.second->GetDbType() == FifamCompDbType::Cup ||
                compEntry.second->GetDbType() == FifamCompDbType::Round ||
                compEntry.second->GetDbType() == FifamCompDbType::Pool)
            {
                countryComps.push_back(compEntry);
            }
        }
    }
    return countryComps;
}

UInt FifamCountry::GetNumWriteableLeagueLevels() const {
    return mNumWriteableLeagueLevels;
}
