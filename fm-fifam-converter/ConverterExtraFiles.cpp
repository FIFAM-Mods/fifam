#include "Converter.h"
#include "FifaConverter.h"

void Converter::ReadAdditionalInfo(Path const &infoPath, UInt gameId) {
    {
        std::wcout << L"Reading fifam_countries.txt..." << std::endl;
        FifamReader reader(infoPath / L"fifam_countries.txt", 0);
        if (reader.Available()) {
            reader.SkipLine();
            while (!reader.IsEof()) {
                if (!reader.EmptyLine()) {
                    String dummy;
                    foom::nation::converter_data info;
                    Int FootballManagerID;
                    reader.ReadLineWithSeparator(L'\t', dummy, dummy, dummy, dummy, FootballManagerID, info.mFIFAID, dummy, info.mFIFAManagerID, info.mFIFAManagerReplacementID, info.mFIFATeamID);
                    if (FootballManagerID != 0) {
                        //if (gameId >= 11 && FootballManagerID == 217945) { // Kosovo
                        //    info.mFIFAManagerID = 207;
                        //    info.mFIFAManagerReplacementID = 207;
                        //    info.mFIFAID = 219;
                        //}
                        //else if (gameId <= 7 && FootballManagerID == 62002127) { // Montenegro
                        //    info.mFIFAManagerID = 207;
                        //    info.mFIFAManagerReplacementID = 207;
                        //    info.mFIFAID = 15;
                        //}
                        map_find(mFoomDatabase->mNations, FootballManagerID).mConverterData = info;
                        FifaConverter::mFifaToFifamCountryId[info.mFIFAID] = { info.mFIFAManagerID, info.mFIFAManagerReplacementID };
                    }
                }
                else
                    reader.SkipLine();
            }
        }
    }
    {
        std::wcout << L"Reading fifam-uids.csv..." << std::endl;
        FifamReader reader(infoPath / L"fifam-uids.csv", 0);
        if (reader.Available()) {
            reader.SkipLine();
            Map<UInt, UInt> uidsMap;
            Map<UInt, UInt> uniqueUIDsMap;
            while (!reader.IsEof()) {
                if (!reader.EmptyLine()) {
                    String dummy;
                    foom::club::converter_data info;
                    Int FootballManagerID;
                    reader.ReadLine(dummy, dummy, dummy, dummy, dummy, FootballManagerID, Hexadecimal(info.mFIFAManagerID), info.mFIFAID);
                    map_find(mFoomDatabase->mClubs, FootballManagerID).mConverterData = info;
                    uidsMap[FootballManagerID] = info.mFIFAManagerID;

                    if (info.mFIFAManagerID > 0) {
                        auto it = uniqueUIDsMap.find(info.mFIFAManagerID);
                        if (it != uniqueUIDsMap.end()) {
                            if (mErrors) {
                                Error(L"Duplicated Unique ID: %08X\nClub1: %d\nClub2: %d", info.mFIFAManagerID, (*it).second, FootballManagerID);
                            }
                        }
                        else
                            uniqueUIDsMap[info.mFIFAManagerID] = FootballManagerID;
                    }
                }
                else
                    reader.SkipLine();
            }
            /*
            Array<UInt, FifamDatabase::NUM_COUNTRIES> maxUid = {};
            for (auto[foomId, fifamId] : uidsMap) {
                if (fifamId > 0) {
                    UChar region = (fifamId >> 16) & 0xFF;
                    if (region >= 1 && region <= 207) {
                        if (fifamId > maxUid[region - 1])
                            maxUid[region - 1] = fifamId;
                    }
                }
            }
            for (auto &e : uidsMap) {
                if (e.second == 0) {
                    foom::club *c = mFoomDatabase->get<foom::club>(e.first);
                    if (c) {
                        foom::nation *n = c->mNation;
                        if (!n)
                            n = c->mContinentalCupNation;
                        if (n) {
                            UInt countryId = n->mConverterData.mFIFAManagerID;
                            if (n->mID == 217945)
                                countryId = 207; // Kosovo
                            if (countryId > 0 && countryId <= FifamDatabase::NUM_COUNTRIES)
                                e.second = ++maxUid[countryId - 1];
                        }
                    }
                }
            }
            FifamWriter clubsUIDsWriter(L"out_club_uids.txt", 14, 0, 0);
            for (auto &e : uidsMap) {
                if (e.second == 0)
                    clubsUIDsWriter.WriteLine(Utils::Format(L"%u\t", e.first));
                else
                    clubsUIDsWriter.WriteLine(Utils::Format(L"%u\t%08X", e.first, e.second));
            }
            */
        }
    }
    {
        std::wcout << L"Reading fm_additional_reserve_teams_for_history.txt..." << std::endl;
        FifamReader reader(infoPath / L"fm_additional_reserve_teams_for_history.txt", 0);
        if (reader.Available()) {
            while (!reader.IsEof()) {
                if (!reader.EmptyLine()) {
                    Int parentID = -1;
                    Int childID = -1;
                    Int childType = -1;
                    reader.ReadLineWithSeparator(L'\t', parentID, childID, childType);
                    if (parentID > 0 && childID > 0 && (childType == 0 || childType == 1)) {
                        foom::club *parentClub = mFoomDatabase->get<foom::club>(parentID);
                        if (parentClub) {
                            foom::club *childClub = mFoomDatabase->get<foom::club>(childID);
                            if (childClub) {
                                childClub->mConverterData.mAdditionalHistoryParentTeam = parentClub;
                                childClub->mConverterData.mAdditionalHistoryReserveTeamType = childType;
                            }
                        }
                    }
                }
                else
                    reader.SkipLine();
            }
        }
    }
    {
        std::wcout << L"Reading fifam_languages.csv..." << std::endl;
        FifamReader reader(infoPath / L"fifam_languages.csv", 0);
        if (reader.Available()) {
            reader.SkipLine();
            while (!reader.IsEof()) {
                if (!reader.EmptyLine()) {
                    String dummy;
                    foom::language::converter_data info;
                    Int FootballManagerID;
                    reader.ReadLine(dummy, dummy, FootballManagerID, info.mFIFAManagerID, info.mFIFAManagerReplacementID);
                    map_find(mFoomDatabase->mLanguages, FootballManagerID).mConverterData = info;
                }
                else
                    reader.SkipLine();
            }
        }
    }
    {
        std::wcout << L"Reading fifam_regions.csv..." << std::endl;
        FifamReader reader(infoPath / L"fifam_regions.csv", 0);
        if (reader.Available()) {
            reader.SkipLine();
            while (!reader.IsEof()) {
                if (!reader.EmptyLine()) {
                    String dummy;
                    foom::region::converter_data info;
                    Int FootballManagerID;
                    reader.ReadLine(dummy, FootballManagerID, info.mFIFAManagerID, info.mFIFAManagerReplacementID);
                    map_find(mFoomDatabase->mRegions, FootballManagerID).mConverterData = info;
                }
                else
                    reader.SkipLine();
            }
        }
    }
    {
        std::wcout << L"Reading fifam_injuries.csv..." << std::endl;
        FifamReader reader(infoPath / L"fifam_injuries.csv", 0);
        if (reader.Available()) {
            reader.SkipLine();
            while (!reader.IsEof()) {
                if (!reader.EmptyLine()) {
                    foom::injury::converter_data info;
                    Int FootballManagerID;
                    reader.ReadLine(FootballManagerID, info.mFIFAManagerID);
                    map_find(mFoomDatabase->mInjuries, FootballManagerID).mConverterData = info;
                }
                else
                    reader.SkipLine();
            }
        }
    }
    {
        std::wcout << L"Reading fifam_divisions.txt..." << std::endl;
        String filename;
        if (mToFifa07Database)
            filename = L"fifam_divisions_fifa07.txt";
        else if (mFromFifaDatabase)
            filename = L"fifam_divisions_07.txt";
        else
            filename = L"fifam_divisions.txt";
        FifamReader reader(infoPath / filename, 0);
        if (reader.Available()) {
            reader.SkipLine();
            while (!reader.IsEof()) {
                if (!reader.EmptyLine()) {
                    DivisionInfo d;
                    String e, type, sorting, split, promotionPlayoff, relegationPlayoff, calendarMatches;
                    reader.ReadLineWithSeparator(L'\t', e, d.mNationID, d.mName, d.mShortName, OptionalInt(d.mID), type, d.mLevel, e, e,
                        d.mTeams, d.mRep, d.mPriority, d.mOrder, OptionalInt(d.mRounds), split, OptionalInt(d.mSplitRounds.first),
                        OptionalInt(d.mSplitRounds.second), d.mPromoted, promotionPlayoff, d.mPromotionID, d.mRelegated, relegationPlayoff,
                        d.mRelegationID, d.mStartDate, d.mEndDate, d.mWinterBreakStart, d.mWinterBreakEnd, OptionalInt(d.mNumSubs),
                        OptionalInt(d.mForeignersField), OptionalInt(d.mForeignersGame), OptionalInt(d.mSeasonSquad),
                        OptionalInt(d.mSeasonSquadForeigners), OptionalInt(d.mNonEuSigns), OptionalInt(d.mReserveTeamsAllowed), sorting,
                        d.mAttendanceMp, d.mTransfersMp, d.mTvBonus, d.mWinBouns, d.mPlaceBonus, d.mOneYearCalendar, calendarMatches);

                    String leagueTitle = d.mShortName + L"(" + std::to_wstring(d.mID) + L")";
                    type = Utils::ToLower(type);
                    if (type == L"league")
                        d.mType = DivisionInfo::League;
                    else if (type == L"level")
                        d.mType = DivisionInfo::Level;
                    else {
                        if (mErrors) {
                            //Error(L"Unknown DivisionInfo type: %s", type.c_str());
                        }
                        continue;
                    }

                    Utils::Trim(sorting);
                    sorting = Utils::ToLower(sorting);

                    if (sorting == L"dm")
                        d.mSorting = FifamEqualPointsSorting::DirectMatches;
                    else if (sorting == L"g")
                        d.mSorting = FifamEqualPointsSorting::Goals;
                    else
                        d.mSorting = FifamEqualPointsSorting::GoalDiff;

                    if (d.mLevel <= 0)
                        d.mLevel = 255;
                    else
                        d.mLevel -= 1;

                    Utils::Trim(split);
                    if (!split.empty()) {
                        auto splitParts = Utils::Split(split, L'x');
                        if (splitParts.size() == 2) {
                            d.mSplit.first = Utils::SafeConvertInt<Int>(splitParts[0]);
                            d.mSplit.second = Utils::SafeConvertInt<Int>(splitParts[1]);
                        }
                        else {
                            if (mErrors) {
                                Error(L"Incorrect league split syntax: \"" + split + L"\"");
                            }
                        }
                    }

                    Utils::Trim(promotionPlayoff);
                    if (!promotionPlayoff.empty()) {
                        auto promotionTeams = Utils::Split(promotionPlayoff, L'+');
                        if (promotionTeams.size() > 0) {
                            d.mPromotionPlayoff.resize(promotionTeams.size());
                            d.mTotalTeamsPromotionPlayoff = 0;
                            for (UInt pt = 0; pt < promotionTeams.size(); pt++) {
                                d.mPromotionPlayoff[pt] = Utils::SafeConvertInt<Int>(promotionTeams[pt]);
                                d.mTotalTeamsPromotionPlayoff += d.mPromotionPlayoff[pt];
                            }
                        }
                    }

                    Utils::Trim(relegationPlayoff);
                    if (!relegationPlayoff.empty()) {
                        auto relegationTeams = Utils::Split(relegationPlayoff, L'+');
                        if (relegationTeams.size() > 0) {
                            d.mRelegationPlayoff.resize(relegationTeams.size());
                            d.mTotalTeamsRelegationPlayoff = 0;
                            for (UInt pt = 0; pt < relegationTeams.size(); pt++) {
                                d.mRelegationPlayoff[pt] = Utils::SafeConvertInt<Int>(relegationTeams[pt]);
                                d.mTotalTeamsRelegationPlayoff += d.mRelegationPlayoff[pt];
                            }
                        }
                    }

                    Utils::Trim(calendarMatches);
                    calendarMatches = Utils::ToLower(calendarMatches);

                    if (calendarMatches == L"prior_saturday")
                        d.mCalendarMatches = 1;
                    else if (calendarMatches == L"prior_sunday")
                        d.mCalendarMatches = 2;
                    else if (calendarMatches == L"force_saturday")
                        d.mCalendarMatches = 3;
                    else if (calendarMatches == L"force_sunday")
                        d.mCalendarMatches = 4;

                    if (d.mForeignersField == -1 || d.mForeignersField == 11)
                        d.mForeignersField = 0;
                    else if (d.mForeignersField == 0)
                        d.mForeignersField = 11;
                    else if (d.mForeignersField < -1 || d.mForeignersField > 11) {
                        if (mErrors)
                            Error(L"Incorrect ForeignersField value (" + std::to_wstring(d.mForeignersField) + L") in " + leagueTitle);
                        d.mForeignersField = 0;
                    }

                    if (d.mForeignersGame < 0 || d.mForeignersGame > 17)
                        d.mForeignersGame = 0;
                    else
                        d.mForeignersGame += 1;

                    if (d.mSeasonSquad <= 0 || d.mSeasonSquad > 45)
                        d.mSeasonSquad = 0;
                    else if (d.mSeasonSquad < 18) {
                        if (mErrors)
                            Error(L"Incorrect SeasonSquad value (" + std::to_wstring(d.mForeignersField) + L") in " + leagueTitle);
                        d.mSeasonSquad = 0;
                    }
                    else
                        d.mSeasonSquad -= 17;

                    if (d.mSeasonSquadForeigners < 0 || d.mSeasonSquadForeigners > 29)
                        d.mSeasonSquadForeigners = 0;
                    else
                        d.mSeasonSquadForeigners += 1;

                    mDivisions.push_back(d);
                }
                else
                    reader.SkipLine();
            }
        }
    }
    if (!mFromFifaDatabase)
    {
        std::wcout << L"Reading fifam_playoffs.txt..." << std::endl;
        FifamReader reader(infoPath / L"fifam_playoffs.txt", 0);
        if (reader.Available()) {
            reader.SkipLine();
            while (!reader.IsEof()) {
                if (!reader.EmptyLine()) {
                    PlayOffInfo p;
                    String e, format;
                    reader.ReadLineWithSeparator(L'\t', e, p.mNationID, p.mID, p.mName[FifamTranslation::English], format, OptionalInt(p.mSubs),
                        p.mName[FifamTranslation::German], p.mName[FifamTranslation::French], p.mName[FifamTranslation::Spanish],
                        p.mName[FifamTranslation::Italian], p.mName[FifamTranslation::Polish]);
                    Bool formatError = false;
                    Utils::Trim(format);
                    if (!format.empty()) {
                        if (format[0] == L'L') {
                            String leagueFormat = format.substr(1);
                            if (leagueFormat.empty())
                                formatError = true;
                            else {
                                auto leagueInfo = Utils::Split(leagueFormat, L'-', true, false);
                                if (leagueInfo.size() != 2)
                                    formatError = true;
                                else {
                                    p.mLeague.mTotalTeams = Utils::SafeConvertInt<UShort>(leagueInfo[0]);
                                    if (p.mLeague.mTotalTeams == 0)
                                        formatError = true;
                                    else {
                                        p.mLeague.mNumWinners = Utils::SafeConvertInt<UShort>(leagueInfo[1]);
                                        if (p.mLeague.mNumWinners > p.mLeague.mTotalTeams)
                                            formatError = true;
                                        else {
                                            p.mLeague.mNumLosers = p.mLeague.mTotalTeams - p.mLeague.mNumWinners;
                                            p.mIsLeague = true;
                                        }
                                    }
                                }
                            }
                        }
                        else {
                            auto rounds = Utils::Split(format, L',');
                            if (rounds.size() > 0) {
                                p.mRounds.resize(rounds.size());
                                for (UInt r = 0; r < rounds.size(); r++) {
                                    auto roundParts = Utils::Split(rounds[r], L'+');
                                    // 1LRS+4
                                    if ((roundParts.size() == 1 || roundParts.size() == 2)
                                        && (roundParts[0].size() >= 2 && roundParts[0].size() <= 4)
                                        && roundParts[0][1] == L'L')
                                    {
                                        if (roundParts.size() == 2)
                                            p.mRounds[r].mNewTeams = Utils::SafeConvertInt<UShort>(roundParts[1]);
                                        if (roundParts[0][0] == L'1')
                                            p.mRounds[r].mLegs = 1;
                                        else if (roundParts[0][0] == L'2')
                                            p.mRounds[r].mLegs = 2;
                                        else
                                            formatError = true;
                                        if (!formatError) {
                                            for (UInt f = 2; f < roundParts[0].size(); f++) {
                                                if (roundParts[0][f] == L'S')
                                                    p.mRounds[r].mShuffle = PlayOffInfo::Round::ShuffleType::AllTeams;
                                                else if (roundParts[0][f] == L'H')
                                                    p.mRounds[r].mShuffle = PlayOffInfo::Round::ShuffleType::HalfTeams;
                                                else if (roundParts[0][f] == L'R')
                                                    p.mRounds[r].mLosersRound = true;
                                                else
                                                    formatError = true;
                                            }
                                        }
                                    }
                                    else
                                        formatError = true;
                                }
                            }
                            else
                                formatError = true;
                        }
                    }
                    if (formatError) {
                        if (mErrors) {
                            Error(L"Incorrect play-off format: \"" + format + L"\"");
                        }
                    }
                    else {
                        for (UInt l = 0; l < FifamTranslation::NUM_TRANSLATIONS; l++) {
                            if (l != FifamTranslation::English && p.mName[l].empty())
                                p.mName[l] = p.mName[FifamTranslation::English];
                        }
                        mPlayOffs.push_back(p);
                    }
                }
                else
                    reader.SkipLine();
            }
        }
    }
    if (!mFromFifaDatabase)
    {
        std::wcout << L"Reading fifam_split_names.txt..." << std::endl;
        FifamReader reader(infoPath / L"fifam_split_names.txt", 0);
        if (reader.Available()) {
            reader.SkipLine();
            while (!reader.IsEof()) {
                if (!reader.EmptyLine()) {
                    FifamTrArray<String> s1, s2;
                    UInt leagueId = 0;
                    reader.ReadLineWithSeparator(L'\t', leagueId, s1[FifamTranslation::English], s1[FifamTranslation::German],
                        s1[FifamTranslation::French], s1[FifamTranslation::Spanish], s1[FifamTranslation::Italian], s1[FifamTranslation::Polish],
                        s2[FifamTranslation::English], s2[FifamTranslation::German], s2[FifamTranslation::French], s2[FifamTranslation::Spanish],
                        s2[FifamTranslation::Italian], s2[FifamTranslation::Polish]);
                    if (leagueId != 0) {
                        if (!s1[FifamTranslation::English].empty()) {
                            for (UInt l = 0; l < FifamTranslation::NUM_TRANSLATIONS; l++) {
                                if (l != FifamTranslation::English && s1[l].empty())
                                    s1[l] = s1[FifamTranslation::English];
                            }
                        }
                        if (!s2[FifamTranslation::English].empty()) {
                            for (UInt l = 0; l < FifamTranslation::NUM_TRANSLATIONS; l++) {
                                if (l != FifamTranslation::English && s2[l].empty())
                                    s2[l] = s2[FifamTranslation::English];
                            }
                        }
                        mSplitNames[leagueId] = { s1, s2 };
                    }
                }
                else
                    reader.SkipLine();
            }
        }
    }
    {
        std::wcout << L"Reading fifam_cups.txt..." << std::endl;
        FifamReader reader(infoPath / L"fifam_cups.txt", 0);
        if (reader.Available()) {
            reader.SkipLine();
            while (!reader.IsEof()) {
                if (!reader.EmptyLine()) {
                    CupInfo c;
                    String strDummy, strType, strTemplateType;
                    reader.ReadLineWithSeparator(L'\t', strDummy, c.mNationID, c.mName, c.mShortName, c.mID, c.mLevel, strType, strDummy, c.mReputation, c.mPriority, strTemplateType, c.mRoundDesc[7], c.mRoundDesc[6], c.mRoundDesc[5], c.mRoundDesc[4], c.mRoundDesc[3], c.mRoundDesc[2], c.mRoundDesc[1], c.mRoundDesc[0], c.mStructure, c.mMaxLevel, c.mNumSubs, c.mBonus, c.mTvBonus, c.mStartDate, c.mEndDate, c.mOneYearCalendar);
                    if (c.mPriority < 1)
                        continue;
                    c.mTemplateType.SetFromStr(strTemplateType);
                    if (c.mTemplateType.GetWasSetFromUnknown()) {
                        if (mErrors) {
                            Error(L"Unknown cup template type: %s\nCup name: %s\nCup ID: %d", strTemplateType.c_str(), c.mName.c_str(), c.mID);
                        }
                        continue;
                    }
                    if (c.mLevel < 0)
                        c.mLevel = 0;
                    if (strType == L"Domestic Main Cup")
                        c.mType = CupInfo::FA;
                    else if (strType == L"Super Cup")
                        c.mType = CupInfo::Supercup;
                    else
                        c.mType = CupInfo::League;
                    mCups.push_back(c);
                }
                else
                    reader.SkipLine();
            }
        }
    }
    if (!mGenerateLeaguesFiles) {
        std::wcout << L"Reading league tables..." << std::endl;
        for (auto const &p : recursive_directory_iterator(infoPath / L"leagues")) {
            if (is_regular_file(p.path()) && p.path().extension() == ".txt") {
                FifamReader reader(p.path(), 0);
                if (reader.Available()) {
                    //Message(p.path().string());
                    String filename = p.path().stem().c_str();
                    //mErrors = false;
                    foom::comp *league = nullptr;
                    DivisionInfo *div = nullptr;
                    foom::club *cupWinner = nullptr;
                    foom::club *cupRunnerUp = nullptr;
                    foom::team *leagueWinner = nullptr;
                    Set<UInt> usedTeamIDs;
                    foom::nation *nation = nullptr;
                    Vector<foom::club *> previousClubs;
                    Bool spare = false;
                    Bool hasSpare = false;

                    auto finishLeague = [&] {
                        if (league && div) {
                            if (league->mVecTeams.size() != div->mTeams) {
                                if (mErrors) {
                                    Error(L"League tables:\nincorrect league teams count\nin %s\n%d - %d\nFile %s", div->mName.c_str(), league->mVecTeams.size(), div->mTeams, filename.c_str());
                                }
                                return false;
                            }
                            if (!leagueWinner && !league->mVecTeams.empty())
                                leagueWinner = league->mVecTeams[0];
                            league->mConverterData.mUsesTableData = true;
                        }
                        league = nullptr;
                        div = nullptr;
                        return true;
                    };

                    while (!reader.IsEof()) {
                        if (!reader.EmptyLine()) {
                            String line = reader.ReadFullLine();
                            if (Utils::StartsWith(line, L";"))
                                continue;
                            if (Utils::StartsWith(line, L"LEAGUE") || Utils::StartsWith(line, L"league")) {
                                if (hasSpare) {
                                    if (mErrors) {
                                        Error(L"League tables:\nspare clubs appear before league(s)\nFile %s", filename.c_str());
                                    }
                                    break;
                                }
                                finishLeague();
                                spare = false;
                                Int leagueId = -1;
                                if (swscanf(line.c_str(), L"%*s %d", &leagueId) != 1) {
                                    if (mErrors) {
                                        if (div)
                                            Error(L"League tables:\nincorrect league line format\nin %s\n%s\nFile %s", div->mName.c_str(), line.c_str(), filename.c_str());
                                        else
                                            Error(L"League tables:\nincorrect league line format\n%s\nFile %s", line.c_str(), filename.c_str());
                                    }
                                    break;
                                }
                                league = mFoomDatabase->get<foom::comp>(leagueId);
                                if (!league) {
                                    if (mErrors) {
                                        Error(L"League tables:\nunable to find league by id\nid %u\nFile %s", leagueId, filename.c_str());
                                    }
                                    break;
                                }
                                for (auto &d : mDivisions) {
                                    if (d.mID == leagueId) {
                                        div = &d;
                                        break;
                                    }
                                }
                                if (!div) {
                                    if (mErrors) {
                                        Error(L"League tables:\nunable to find division by id\nid %u\nFile %s", leagueId, filename.c_str());
                                    }
                                    break;
                                }
                                if (!nation)
                                    nation = league->mNation;
                                else if (league->mNation != nation) {
                                    if (mErrors) {
                                        Error(L"League tables:\nleague from different nation\nin %s\nid %u\nFile %s", div->mName.c_str(), leagueId, filename.c_str());
                                    }
                                    break;
                                }
                                league->mVecTeams.clear();
                            }
                            else if (Utils::StartsWith(line, L"SPARE") || Utils::StartsWith(line, L"spare")) {
                                if (hasSpare) {
                                    if (mErrors) {
                                        Error(L"League tables:\nduplicated spare clubs section\nFile %s", filename.c_str());
                                    }
                                    break;
                                }
                                finishLeague();
                                Int nationId = -1;
                                if (swscanf(line.c_str(), L"%*s %d", &nationId) == 1) {
                                    auto spareNation = mFoomDatabase->get<foom::nation>(nationId);
                                    if (spareNation && nation && spareNation != nation) {
                                        if (mErrors) {
                                            Error(L"League tables:\nspare from different nation\nleagues nation: %s\nspare nation: %s\nFile %s", nation->mName.c_str(), spareNation->mName.c_str(), filename.c_str());
                                        }
                                        break;
                                    }
                                    nation = spareNation;

                                }
                                if (!nation) {
                                    if (mErrors) {
                                        Error(L"League tables:\nnation for spare clubs is not defined\nFile %s", filename.c_str());
                                    }
                                    break;
                                }
                                nation->mConverterData.mSpareClubs.clear();
                                spare = true;
                                hasSpare = true;
                            }
                            else if (Utils::StartsWith(line, L"NON") || Utils::StartsWith(line, L"non"))
                                break;
                            else {
                                Int teamId = -1;
                                WideChar status[64];
                                if ((league && div) || spare) {
                                    if (swscanf(line.c_str(), L"%d %s", &teamId, status) == 2) {
                                        foom::club *team = mFoomDatabase->get<foom::club>(teamId);
                                        // TODO: remove this later
                                        if (!team && !spare) {
                                            if (line.size() > 19 && Utils::EndsWith(line, L" (RES)")) {
                                                String clubName = line.substr(13, line.size() - 19);
                                                //Error(clubName);
                                                for (foom::club *prev : previousClubs) {
                                                    if (prev->mName == clubName) {
                                                        if (!prev->mVecReserveTeams.empty()) {
                                                            team = prev->mVecReserveTeams[0].mReserveClub;
                                                            teamId = team->mID;
                                                            break;
                                                        }
                                                    }
                                                }
                                            }
                                        }
                                        if (!team) {
                                            if (mErrors) {
                                                if (spare)
                                                    Error(L"League tables:\nunable to find team by id\nin spare (%s)\nid %u\n%s\nFile %s", nation->mName.c_str(), teamId, line.c_str(), filename.c_str());
                                                else
                                                    Error(L"League tables:\nunable to find team by id\nin %s\nid %u\n%s\nFile %s", div ? div->mName.c_str() : L"", teamId, line.c_str(), filename.c_str());
                                            }
                                        }
                                        else {
                                            if (team->mExtinct && (Utils::EndsWith(line, L" (RES)") || Utils::EndsWith(line, L"(REMOVE-EXTINCT)"))) {
                                                team->mExtinct = false;
                                                if (nation) {
                                                    team->mNation = nation;
                                                    team->mBasedNation = team->mNation;
                                                    team->mContinentalCupNation = team->mNation;
                                                }
                                            }
                                            if (team->mExtinct) {
                                                if (mErrors) {
                                                    if (spare)
                                                        Error(L"League tables:\nextinct team in the spare\nin %s\nid %u\n%s\nFile %s", nation->mName.c_str(), teamId, line.c_str(), filename.c_str());
                                                    else
                                                        Error(L"League tables:\nextinct team in the league\nin %s\nid %u\n%s\nFile %s", div->mName.c_str(), teamId, line.c_str(), filename.c_str());
                                                }
                                            }
                                            else {
                                                if (team->mNation != nation) {
                                                    if (mErrors) {
                                                        if (spare)
                                                            Error(L"League tables:\nteam from incorrect country in the spare\nin %s\nid %u\n%s\nFile %s", nation->mName.c_str(), teamId, line.c_str(), filename.c_str());
                                                        else
                                                            Error(L"League tables:\nteam from incorrect country in the league\nin %s\nid %u\n%s\nFile %s", div->mName.c_str(), teamId, line.c_str(), filename.c_str());
                                                    }
                                                }
                                                else {
                                                    if (Utils::Contains(usedTeamIDs, teamId)) {
                                                        if (mErrors) {
                                                            if (spare)
                                                                Error(L"League tables:\nteam id was already registered in the spare\nin %s\nid %u\n%s\nFile %s", nation->mName.c_str(), teamId, line.c_str(), filename.c_str());
                                                            else
                                                                Error(L"League tables:\nteam id was already registered in the league\nin %s\nid %u\n%s\nFile %s", div->mName.c_str(), teamId, line.c_str(), filename.c_str());
                                                        }
                                                    }
                                                    else {
                                                        if (spare) {
                                                            if (nation)
                                                                nation->mConverterData.mSpareClubs.push_back(team);
                                                        }
                                                        else if (league)
                                                            league->mVecTeams.push_back(team);
                                                        previousClubs.push_back(team);
                                                        usedTeamIDs.insert(teamId);
                                                        String strStatus = status;
                                                        foom::club *possibleCupWinner = nullptr;
                                                        foom::club *possibleCupRunnerUp = nullptr;
                                                        if (strStatus == L"P") {
                                                            team->mConverterData.mTablePromoted = true;
                                                        }
                                                        else if (strStatus == L"R") {
                                                            team->mConverterData.mTableRelegated = true;
                                                        }
                                                        else if (strStatus == L"W") {
                                                            possibleCupWinner = team;
                                                        }
                                                        else if (strStatus == L"F") {
                                                            possibleCupRunnerUp = team;
                                                        }
                                                        else if (strStatus == L"PW" || strStatus == L"WP") {
                                                            team->mConverterData.mTablePromoted = true;
                                                            possibleCupWinner = team;
                                                        }
                                                        else if (strStatus == L"PF" || strStatus == L"FP") {
                                                            team->mConverterData.mTablePromoted = true;
                                                            possibleCupRunnerUp = team;
                                                        }
                                                        else if (strStatus == L"RW" || strStatus == L"WR") {
                                                            team->mConverterData.mTableRelegated = true;
                                                            possibleCupWinner = team;
                                                        }
                                                        else if (strStatus == L"RF" || strStatus == L"FR") {
                                                            team->mConverterData.mTableRelegated = true;
                                                            possibleCupRunnerUp = team;
                                                        }
                                                        else if (strStatus == L"-") {

                                                        }
                                                        else {
                                                            if (mErrors) {
                                                                if (spare)
                                                                    Error(L"League tables:\nunknown status\nin spare %s\nid %u\n%s\nFile %s", nation->mName.c_str(), teamId, line.c_str(), filename.c_str());
                                                                else
                                                                    Error(L"League tables:\nunknown status\nin league %s\nid %u\n%s\nFile %s", div->mName.c_str(), teamId, line.c_str(), filename.c_str());
                                                            }
                                                        }
                                                        if (possibleCupWinner) {
                                                            if (cupWinner) {
                                                                if (mErrors) {
                                                                    if (spare)
                                                                        Error(L"League tables:\ncup winner was already set\nin spare %s\nid %u\n%s\nFile %s", nation->mName.c_str(), teamId, line.c_str(), filename.c_str());
                                                                    else
                                                                        Error(L"League tables:\ncup winner was already set\nin league %s\nid %u\n%s\nFile %s", div->mName.c_str(), teamId, line.c_str(), filename.c_str());
                                                                }
                                                            }
                                                            else {
                                                                cupWinner = possibleCupWinner;
                                                            }
                                                        }
                                                        if (possibleCupRunnerUp) {
                                                            if (cupRunnerUp) {
                                                                if (mErrors) {
                                                                    if (spare)
                                                                        Error(L"League tables:\ncup runner-up was already set\nin spare %s\nid %u\n%s\nFile %s", nation->mName.c_str(), teamId, line.c_str(), filename.c_str());
                                                                    else
                                                                        Error(L"League tables:\ncup runner-up was already set\nin league %s\nid %u\n%s\nFile %s", div->mName.c_str(), teamId, line.c_str(), filename.c_str());
                                                                }
                                                            }
                                                            else {
                                                                cupRunnerUp = possibleCupRunnerUp;
                                                            }
                                                        }
                                                    }
                                                }
                                            }
                                        }
                                    }
                                    else {
                                        if (mErrors) {
                                            if (spare)
                                                Error(L"League tables:\nunable to parse team id line\nin spare %s\nid %u\n%s\nFile %s", nation->mName.c_str(), teamId, line.c_str(), filename.c_str());
                                            else
                                                Error(L"League tables:\nunable to parse team id line\nin league %s\nid %u\n%s\nFile %s", div->mName.c_str(), teamId, line.c_str(), filename.c_str());
                                        }
                                    }
                                }
                            }
                        }
                        else
                            reader.SkipLine();
                    }
                    finishLeague();
                    if (cupWinner)
                        cupWinner->mConverterData.mTableCupWinner = true;
                    if (cupRunnerUp)
                        cupRunnerUp->mConverterData.mTableCupRunnerUp = true;
                    if (nation) {
                        nation->mConverterData.mDomesticComps.cupWinner = cupWinner;
                        nation->mConverterData.mDomesticComps.cupRunnerUp = cupRunnerUp;
                        if (leagueWinner)
                            nation->mConverterData.mDomesticComps.leagueWinner = leagueWinner;
                    }
                    //if (nation && (cupWinner || cupRunnerUp)) {
                    //    for (auto &entry : mFoomDatabase->mComps) {
                    //        auto &c = entry.second;
                    //        if (c.mNation) {
                    //            if (c.mType == 0) { // domestic main league
                    //                c.mVecHistory.erase(std::remove_if(c.mVecHistory.begin(), c.mVecHistory.end(), [](const foom::comp::history &h) {
                    //                    return h.mYear >= CURRENT_YEAR;
                    //                    }), c.mVecHistory.end());
                    //                if (!c.mIsExtinct) {
                    //                    foom::comp::history newEntry;
                    //                    newEntry.mYear = CURRENT_YEAR;
                    //                    newEntry.mFirstPlaced = cupWinner;
                    //                    newEntry.mSecondPlaced = cupRunnerUp;
                    //                    c.mVecHistory.push_back(newEntry);
                    //                }
                    //            }
                    //            else if (c.mType == 2) { // domestic main cup
                    //                c.mVecHistory.erase(std::remove_if(c.mVecHistory.begin(), c.mVecHistory.end(), [](const foom::comp::history & h) {
                    //                    return h.mYear >= CURRENT_YEAR;
                    //                    }), c.mVecHistory.end());
                    //                if (!c.mIsExtinct) {
                    //                    foom::comp::history newEntry;
                    //                    newEntry.mYear = CURRENT_YEAR;
                    //                    newEntry.mFirstPlaced = cupWinner;
                    //                    newEntry.mSecondPlaced = cupRunnerUp;
                    //                    c.mVecHistory.push_back(newEntry);
                    //                }
                    //            }
                    //        }
                    //    }
                    //}
                }
            }
        }
    }
    {
        std::wcout << L"Reading fixtures..." << std::endl;
        for (auto const &p : recursive_directory_iterator(infoPath / L"fixtures")) {
            if (is_regular_file(p.path()) && p.path().extension() == ".txt") {
                UInt leagueId = Utils::SafeConvertInt<UInt>(p.path().filename().c_str());
                if (leagueId > 0) {
                    Vector<FixtureInfo> &fixtures = mFixturesPerLeague[leagueId];
                    fixtures.clear();
                    FifamReader reader(p.path(), 0);
                    if (reader.Available()) {
                        while (!reader.IsEof()) {
                            if (!reader.EmptyLine()) {
                                FixtureInfo f;
                                String dateStr, dummy;
                                reader.ReadLineWithSeparator(L' ', dateStr, dummy, IntPtr(f.mTeam1), IntPtr(f.mTeam2));
                                mFoomDatabase->resolve(f.mTeam1);
                                mFoomDatabase->resolve(f.mTeam2);
                                auto dateParts = Utils::Split(dateStr, L'/', true, false);
                                if (dateParts.size() >= 3) {
                                    f.mDate.Set(Utils::SafeConvertInt<UInt>(dateParts[0]), Utils::SafeConvertInt<UInt>(dateParts[1]),
                                        Utils::SafeConvertInt<UInt>(dateParts[2]));
                                }
                                else {
                                    if (mErrors) {
                                        Error(L"Wrong fixture date in fixture file\nFixture file: %s", p.path().filename().c_str());
                                    }
                                }
                                fixtures.push_back(f);
                            }
                            else
                                reader.SkipLine();
                        }
                    }
                    reader.Close();
                }
            }
        }
    }
    {
        std::wcout << L"Reading calendars..." << std::endl;
        for (auto const &p : recursive_directory_iterator(infoPath / L"calendars")) {
            if (is_regular_file(p.path()) && p.path().extension() == ".csv") {
                FifamReader reader(p.path(), 0);
                if (reader.Available()) {
                    while (!reader.IsEof()) {
                        if (!reader.EmptyLine()) {
                            UInt compId = 0, season = 0;
                            String daysStr;
                            reader.ReadLine(Hexadecimal(compId), season, daysStr);
                            if (compId != 0) {
                                if (season == 0 || season == 1) {
                                    auto days = Utils::Split(daysStr, L',', true, false);
                                    Vector<UShort> daysInts;
                                    daysInts.resize(days.size());
                                    for (UInt d = 0; d < days.size(); d++)
                                        daysInts[d] = Utils::SafeConvertInt<UShort>(days[d]);
                                    if (season == 0)
                                        mCalendarsFirstSeason[compId] = daysInts;
                                    else
                                        mCalendarsSecondSeason[compId] = daysInts;
                                }
                                else {
                                    if (mErrors) {
                                        Error(L"Wrong season ID in calendar file %s", p.path().filename().c_str());
                                    }
                                }
                            }
                            else {
                                if (mErrors) {
                                    Error(L"Wrong competition ID in calendar file %s", p.path().filename().c_str());
                                }
                            }
                        }
                        else
                            reader.SkipLine();
                    }
                }
                reader.Close();
            }
        }
    }
    if (!mQuickTest && gameId > 7) {
        std::wcout << L"Reading fm-fifa-players..." << std::endl;
        FifamReader reader(infoPath / L"fm-fifa-players.csv", 0);
        if (reader.Available()) {
            reader.SkipLine();
            while (!reader.IsEof()) {
                if (!reader.EmptyLine()) {
                    String dummy;
                    String foomId, fifaId, editorFace, editorHair, editorBeard, editorSkin, editorEye;
                    reader.ReadLine(dummy, dummy, dummy, dummy, dummy, dummy, dummy,
                        foomId, fifaId, editorSkin, editorFace, editorHair, editorBeard, editorEye);
                    if (!foomId.empty()) {
                        Int playerId = Utils::SafeConvertInt<Int>(foomId);
                        if (playerId != -1) {
                            foom::player *foomPlayer = mFoomDatabase->get<foom::player>(playerId);
                            if (foomPlayer) {
                                auto SetPlayerParameter = [](auto & param, String const &str) {
                                    if (!str.empty()) {
                                        Int paramId = Utils::SafeConvertInt<Int>(str);
                                        if (paramId != -1)
                                            param = paramId;
                                    }
                                };
                                SetPlayerParameter(foomPlayer->mConverterData.mFifaPlayerId, fifaId);
                                SetPlayerParameter(foomPlayer->mConverterData.mEditorFace, editorFace);
                                SetPlayerParameter(foomPlayer->mConverterData.mEditorHair, editorHair);
                                SetPlayerParameter(foomPlayer->mConverterData.mEditorBeard, editorBeard);
                                SetPlayerParameter(foomPlayer->mConverterData.mEditorSkin, editorSkin);
                                SetPlayerParameter(foomPlayer->mConverterData.mEditorEye, editorEye);
                            }
                        }
                    }
                }
                else
                    reader.SkipLine();
            }
        }
    }
    {
        std::wcout << L"Reading fifam_club_colors..." << std::endl;
        FifamReader reader(infoPath / L"fifam_club_colors.csv", 0);
        if (reader.Available()) {
            reader.SkipLine();
            while (!reader.IsEof()) {
                if (!reader.EmptyLine()) {
                    Int teamId = -1;
                    String colorStr;
                    reader.ReadLine(teamId, colorStr);
                    if (teamId != -1) {
                        colorStr = Utils::ToUpper(colorStr);
                        ClubColor color;
                        if (colorStr == L"KIT") {
                            color.type = ClubColor::KIT;
                            mClubColorsMap[teamId] = color;
                        }
                        else if (colorStr == L"TEAM") {
                            color.type = ClubColor::TEAM;
                            mClubColorsMap[teamId] = color;
                        }
                        else {
                            auto colors = Utils::Split(colorStr, L'-');
                            if (colors.size() == 2) {
                                bool failed = false;
                                for (UInt i = 0; i < 2; i++) {
                                    auto rgb = Utils::Split(colors[i], L',');
                                    if (rgb.size() != 3) {
                                        failed = true;
                                        break;
                                    }
                                    color.rgb[i].Set(Utils::SafeConvertInt<UChar>(rgb[0]),
                                        Utils::SafeConvertInt<UChar>(rgb[1]), Utils::SafeConvertInt<UChar>(rgb[2]));
                                }
                                if (!failed) {
                                    color.type = ClubColor::RGB;
                                    mClubColorsMap[teamId] = color;
                                }
                            }
                        }
                    }
                }
                else
                    reader.SkipLine();
            }
        }
    }
    {
        std::wcout << L"Reading fifam_penalty_points..." << std::endl;
        FifamReader reader(infoPath / L"fifam_penalty_points.txt", 0);
        if (reader.Available()) {
            reader.SkipLine();
            while (!reader.IsEof()) {
                if (!reader.EmptyLine()) {
                    Int teamId = -1;
                    Int points = 0;
                    reader.ReadLine(teamId, points);
                    if (teamId != -1 && points < 0)
                        mPenaltyPointsMap[teamId] = points;
                }
                else
                    reader.SkipLine();
            }
        }
    }
    {
        Array<String, FifamTranslation::NUM_TRANSLATIONS + 1> translations = { L"", L"de", L"en", L"fr", L"es", L"it", L"pl" };
        for (UInt i = 0; i < translations.size(); i++) {
            String fileName = L"fifam_names";
            if (!translations[i].empty())
                fileName += L"_" + translations[i];
            std::wcout << L"Reading " << fileName << "..." << std::endl;
            FifamReader reader(infoPath / (fileName + L".txt"), 0);
            if (reader.Available()) {
                reader.SkipLine();
                while (!reader.IsEof()) {
                    if (!reader.EmptyLine()) {
                        Int teamId = -1;
                        String longName, shortName, abbr;
                        String d;
                        reader.ReadLineWithSeparator(L'\t', d, d, d, d, d, d, d, d, d, teamId, longName, shortName, abbr);
                        if (teamId != -1) {
                            if (!longName.empty())
                                mNamesMap[i][teamId] = longName;
                            if (!shortName.empty())
                                mShortNamesMap[i][teamId] = shortName;
                            if (!abbr.empty())
                                mAbbreviationMap[i][teamId] = Utils::GetStringWithoutUnicodeChars(abbr);
                        }
                    }
                    else
                        reader.SkipLine();
                }
            }
        }
    }
    {
        std::wcout << L"Reading fm_free_agents.txt..." << std::endl;
        FifamReader reader(infoPath / L"fm_free_agents.txt", 0);
        if (reader.Available()) {
            while (!reader.IsEof()) {
                if (!reader.EmptyLine()) {
                    Int FootballManagerID;
                    reader.ReadLine(FootballManagerID);
                    mFreeAgentsToAdd.insert(FootballManagerID);
                }
                else
                    reader.SkipLine();
            }
        }
    }
    {
        std::wcout << L"Reading fm_history_nations.txt..." << std::endl;
        FifamReader reader(infoPath / L"fm_history_nations.txt", 0);
        if (reader.Available()) {
            while (!reader.IsEof()) {
                if (!reader.EmptyLine()) {
                    Int nationID = -1;
                    Int historyNationID = -1;
                    reader.ReadLineWithSeparator(L'\t', nationID, historyNationID);
                    if (nationID > 0 && historyNationID > 0) {
                        foom::nation *n = mFoomDatabase->get<foom::nation>(nationID);
                        if (n) {
                            foom::nation *h = mFoomDatabase->get<foom::nation>(historyNationID);
                            if (h)
                                n->mConverterData.mHistoryNation = h;
                        }
                    }
                }
                else
                    reader.SkipLine();
            }
        }
    }
    {
        std::wcout << L"Reading fifam_country_strength.txt..." << std::endl;
        FifamReader reader(infoPath / L"fifam_country_strength.txt", 0);
        if (reader.Available()) {
            while (!reader.IsEof()) {
                if (!reader.EmptyLine()) {
                    Int countryID = -1;
                    UInt strength = 0;
                    reader.ReadLineWithSeparator(L'\t', countryID, strength);
                    if (countryID >= 1 && countryID <= 207)
                        mIPCountryStrength[countryID - 1] = strength;
                }
                else
                    reader.SkipLine();
            }
        }
    }
    {
        if (mToFifa07Database) {
            std::wcout << L"Reading fm-fifa-leagues.txt..." << std::endl;
            FifamReader reader(infoPath / L"fm-fifa-leagues.txt", 0);
            if (reader.Available()) {
                reader.SkipLine();
                while (!reader.IsEof()) {
                    if (!reader.EmptyLine()) {
                        UInt fifamId = 0, leagueId = 0, tournamentId = 0;
                        reader.ReadLineWithSeparator(L'\t', Hexadecimal(fifamId), leagueId, tournamentId);
                        if (fifamId != 0) {
                            if (leagueId != 0)
                                mFifamCompIdToFifa07LeagueId[fifamId] = leagueId;
                            if (tournamentId != 0)
                                mFifamCompIdToFifa07TournamentId[fifamId] = tournamentId;
                        }
                    }
                    else
                        reader.SkipLine();
                }
            }
        }
    }
}
